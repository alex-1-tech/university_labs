#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

#define FILE_LETTER "data_base_letters.txt"
#define FILE_DELIVERY "data_base_delivery.txt"
using namespace std;

class Fio {
    string first_name;
    string second_name;
    string patronymic;
public:
    Fio() :
            first_name("Error_first_name"),
            second_name("Error_second_name"),
            patronymic("Error_patronymic") {}

    Fio(string s1, string s2, string s3) :
            first_name(std::move(s1)),
            second_name(std::move(s2)),
            patronymic(std::move(s3)) {}

    string get_name() {
        return first_name + " " + second_name + " " + patronymic;
    }

    Fio &operator=(const Fio &right) = default;

    bool operator==(const Fio &rhs) const {
        return this->first_name == rhs.first_name &&
               this->second_name == rhs.second_name &&
               this->patronymic == rhs.patronymic;
    }

    friend istream &operator>>
            (istream &os, Fio &rhs) {
        return os >> rhs.first_name >> rhs.second_name >> rhs.patronymic;
    }

    friend ostream &operator<<
            (ostream &os, const Fio &rhs) {
        return os << rhs.first_name + ' ' + rhs.second_name + ' ' + rhs.patronymic;
    }

    ~Fio() = default;
};

class Date {
public:
    int day;
    int month;
    int year;

    Date() : day(-1), month(-1), year(-1) {}

    Date(int t_day, int t_month, int t_year) : day(t_day), month(t_month), year(t_year) {}

    ~Date() = default;
    Date &operator=(const Date &right) = default;
    friend ostream &operator<<
            (ostream &os, const Date &rhs) {
        return os << rhs.day + ' ' + rhs.month + ' ' + rhs.year;
    }

    friend istream &operator>>
            (istream &os, Date &rhs) {
        return os >> rhs.day >> rhs.month >> rhs.year;
    }
};

enum SortBy {
    All,
    ByEmail,
    ByFIO,
};

class Common {
protected:
    string email;
    Fio fio;

public:
    Common() : email(" "), fio(Fio()) {}

    Common(string t_email, const Fio &t_fio) : email(std::move(t_email)), fio(t_fio) {}

    ~Common() = default;

    bool operator==(const Common &rhs) const {
        return this->email == rhs.email &&
               this->fio == rhs.fio;
    }

    virtual void save(ofstream &f);
    virtual void print_info() const;
};

void Common::save(ofstream &f) {
    f << email << " " << fio << " ";
}

void Common::print_info() const{
    cout << email << ' ' << fio << ' ';
}
class Delivery : public Common {
private:
    Date date;
public:
    void save(ofstream &f) override;

    template<typename Type>
    friend void write_to_db(Type *commons, int num);
    friend void read_from_db(Delivery *&deliveries, int &num);
    static void add_product(Delivery *&deliveries, const string &t_email, const Fio &t_fio, Date &t_date) {
        if (count == 0) {
            deliveries = new Delivery[1];
        } else {
            auto *temp = new Delivery[count + 1];
            for (int i = 0; i < count; i++) temp[i] = deliveries[i];
            delete[] deliveries;
            deliveries = temp;
        }
        deliveries[count].email = t_email;
        deliveries[count].fio = t_fio;
        deliveries[count].date = t_date;
        count += 1;
    }
    static void input_product(Delivery *&deliveries) {
        cout << "Input email: ";
        auto *email = new string;
        cin >> *email;
        auto *first_name = new string, *second_name = new string, *patronymic = new string;
        cout << "Input FIO: ";
        cin >> *first_name >> *second_name >> *patronymic;
        Fio fio(*first_name, *second_name, *patronymic);
        auto date = new Date;
        cout << "Input date: ";
        cin >> *date;
        add_product(deliveries, *email, fio, *date);
    }
    template<typename Type>
    friend void show_products(const Type *commons, int num, SortBy sort_key);
    void print_info() const override;
    static int count;
};
void Delivery::print_info() const {
    Common::print_info();
    cout << date << '\n';
}
void Delivery::save(std::ofstream &f) {
    Common::save(f);
    f << date << " ";
}

class Letter : public Common {
public:
    double cost;
public:
    Letter() : Common(), cost(-1) {};

    Letter(string s, const Fio &f, double c) : Common(std::move(s), f), cost(c) {};

    static int count;

    bool operator==(const Letter &rhs) const {
        return this->email == rhs.email &&
               this->fio == rhs.fio &&
               this->cost == rhs.cost;
    }

    void save(ofstream &f) override;
    void print_info() const override;
    template<typename Type>
    friend void write_to_db(Type *commons, int num);

    friend void read_from_db(Letter *&letters, int &num);

    static void add_product(Letter *&letters, const string &t_email, const Fio &t_fio, double t_cost) {
        if (count == 0) {
            letters = new Letter[1];
        } else {
            auto *temp = new Letter[count + 1];
            for (int i = 0; i < count; i++) temp[i] = letters[i];
            delete[] letters;
            letters = temp;
        }
        letters[count].email = t_email;
        letters[count].fio = t_fio;
        letters[count].cost = t_cost;
        count += 1;
    }

    static void input_product(Letter *&letters) {
        cout << "Input email: ";
        auto *email = new string;
        cin >> *email;
        auto *first_name = new string, *second_name = new string, *patronymic = new string;
        cout << "Input FIO: ";
        cin >> *first_name >> *second_name >> *patronymic;
        Fio fio(*first_name, *second_name, *patronymic);
        auto cost = new double;
        cout << "Input cost: ";
        cin >> *cost;
        add_product(letters, *email, fio, *cost);
    }
    template<typename Type>
    friend void show_products(const Type *commons, int num, SortBy sort_key);


    friend bool sort_letters(const Letter &lhs, const Letter &rhs);
};
template<typename Type>
void show_products(const Type *commons, int num, SortBy sort_key) {
    string name_key = " ";
    Fio fio_key;
    if (sort_key == SortBy::ByEmail) {
        cout << "Input email" << endl << "> ";
        cin >> name_key;
    } else if (sort_key == SortBy::ByFIO) {
        auto *first_name = new string, *second_name = new string, *patronymic = new string;
        cout << "Input FIO: ";
        cin >> *first_name >> *second_name >> *patronymic;
        Fio fio(*first_name, *second_name, *patronymic);
        fio_key = fio;
    }

    cout << "!~Table info~!";
    cout << "-------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < num; i++) {
        if (sort_key == SortBy::All ||
            (sort_key == SortBy::ByEmail && name_key == commons[i].email) ||
            (sort_key == SortBy::ByFIO && fio_key == commons[i].fio)) {
            commons[i].print_info();
        }
    }
    cout << "-------------------------------------------------------------------------------------" << endl;
}
void Letter::save(std::ofstream &f) {
    Common::save(f);
    f << cost << " ";
}
void Letter::print_info() const {
    Common::print_info();
    cout << cost << '\n';
}
auto sort_letters(const Letter &lhs, const Letter &rhs) -> bool {
    return lhs.cost < rhs.cost;
}


void read_from_db(Letter *&letters, int &num) {
    string t_email;
    Fio t_fio;
    double t_cost;
    ifstream fp(FILE_LETTER);
    if (!fp.is_open()) {
        cout << "Error opening the file" << endl;
        return;
    }
    num = 0;
    while (fp >> t_email >> t_fio >> t_cost) {
        Letter::add_product(letters, t_email, t_fio, t_cost);
    }
    fp.close();
}
void read_from_db(Delivery *&deliveries, int &num) {
    string t_email;
    Fio t_fio;
    Date t_date;
    ifstream fp(FILE_DELIVERY);
    if (!fp.is_open()) {
        cout << "Error opening the file" << endl;
        return;
    }
    num = 0;
    while (fp >> t_email >> t_fio >> t_date) {
        Delivery::add_product(deliveries, t_email, t_fio, t_date);
    }
    fp.close();
}
template<typename Type>
void write_to_db(Type *commons, int num) {
    ofstream file_commons;
    if (typeid(*commons) == typeid(Letter)) {
        file_commons.open(FILE_LETTER);
    } else if (typeid(*commons) == typeid(Delivery)) {
        file_commons.open(FILE_DELIVERY);
    } else
        file_commons.open("data_base.txt");
    if (file_commons.is_open()) {
        for (int i = 0; i < num; i++) {
            commons[i].save(file_commons);
        }
        file_commons.close();
    } else {
        cerr << "Error opening the file" << endl;
        return;
    }
}

int Letter::count = 0;
int Delivery::count = 0;

int main() {
    Letter *letters = nullptr;
    Delivery *deliveries = nullptr;
    read_from_db(letters, Letter::count);
    read_from_db(deliveries, Delivery::count);
    if (Letter::count == 0) {
        cout << "First initialization letters" << endl;
        Letter::input_product(letters);
    } else {
        cout << "Initialization letters from db" << endl;
    }
    if (Delivery::count == 0) {
        cout << "First initialization deliveries" << endl;
        Delivery::input_product(deliveries);
    } else {
        cout << "Initialization deliveries from db" << endl;
    }
    char choice = ' ';
    while (choice != '0') {
        cout << endl;
        cout << "1) Add a new letter" << endl;
        cout << "2) Show list letters" << endl;
        cout << "3) Show letters by Email" << endl;
        cout << "4) Show letters by FIO" << endl;
        cout << "5) Sort letters" << endl;
        cout << "6) Add a new delivery" << endl;
        cout << "7) Show deliveries" << endl;
        cout << "8) Show deliveries by Email" << endl;
        cout << "9) Show deliveries by FIO" << endl;
        cout << "0) Exit" << endl;
        cout << "> ";
        cin >> choice;
        cout << endl;
        switch (choice) {
            case '0': {
                cout << "Save db..";
                write_to_db(letters, Letter::count);
                write_to_db(deliveries, Delivery::count);
                break;
            }
            case '1': {
                Letter::input_product(letters);
                break;
            }
            case '2': {
                show_products(letters, Letter::count, SortBy::All);
                break;
            }
            case '3': {
                show_products(letters, Letter::count, SortBy::ByEmail);
                break;
            }
            case '4': {
                show_products(letters, Letter::count, SortBy::ByFIO);
                break;
            }
            case '5': {
                sort(letters, letters + Letter::count, sort_letters);
                cout << "done..." << endl;
                break;
            }
            case '6': {
                Delivery::input_product(deliveries);
                break;
            }
            case '7': {
                show_products(deliveries, Letter::count, SortBy::All);
                break;
            }
            case '8': {
                show_products(deliveries, Letter::count, SortBy::ByEmail);
                break;
            }
            case '9': {
                show_products(deliveries, Letter::count, SortBy::ByFIO);
                break;
            }
            default:
                cout << endl << "Error value" << endl;
        }
    }
}




