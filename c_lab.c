#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int day;
    char month[20];
    int year;
} date;
typedef struct {
    int number_section;
    char name[20];
    double cost;
    int col;
    date dates;
    int expiration_date;
} product;

size_t N;
int col_products = 0;
char *filename = "db.txt";

int FindPosition(product *products, const char *name);

void AddProduct(product *products, const char *name, double cost, date dates, int expiration_date);

// print functions
void PrintMainDatabaseInfo();

void PrintProductInfo(product item, int i);

void PrintAllProductsInfo(product *products);

void PrintProductsInfoByNumberSection(product *products, int number_section);

void PrintProductsQuantityLess(product *products, int quantity);

void TestAddProduct() {
    const char *name1 = "Apple";
    const char *name2 = "Grape";
    double cost1 = 1.0, cost2 = 2.0;

    // adding one item
    {
        col_products = 0;
        product products[2];
        AddProduct(products, name1, cost1,
                   (date) {1, "may", 12}, 5);
        assert(!strcmp(products[0].name, name1));
        assert(products[0].cost == cost1);
        assert(products[0].col == 1);
        AddProduct(products, name1, cost1, (date) {1, "may", 12}, 5);
        assert(products[0].col == 2);
    }
    // adding two items
    {
        col_products = 0;
        product products[2];
        AddProduct(products, name1, cost1, (date) {1, "may", 12}, 5);
        AddProduct(products, name2, cost2, (date) {1, "may", 12}, 5);
        assert(!strcmp(products[0].name, name1));
        assert(!strcmp(products[1].name, name2));
    }
}

void TestPrintSystem() {
    col_products = 0;
    product products[5];
    AddProduct(products, (char *) "Grape", 2.11, (date) {1, "may", 12}, 5);
    AddProduct(products, (char *) "Apple", 24.11, (date) {5, "sep", 4}, 1);
    AddProduct(products, (char *) "Apple", 22.11, (date) {1, "june", 7}, 4);
    AddProduct(products, (char *) "PineApple", 5.11, (date) {3, "may", 12}, 3);

    PrintAllProductsInfo(products);
    PrintProductsInfoByNumberSection(products, 0);
    PrintProductsQuantityLess(products, 2);
}

void TestProductSystem() {
    TestAddProduct();
    TestPrintSystem();
    col_products = 0;
    printf("Search server testing finished\n");
}

void Start() {
    printf("Database \"warehouse with products\"\n");
    FILE *fp;
    fopen_s(&fp, filename, "r");
    if (fp == NULL) {
        printf("Enter the number of products: ");
        scanf_s("%d", &N);
    } else {
        N = fgetc(fp);
    }
    product products[N];
    if (fp != NULL) {
        col_products = fgetc(fp);
        printf("%d %d", col_products, N);
        for (int i = 0; i < col_products; i++) {
            fread(&products[i], sizeof(product), 1, fp);
        }
        fclose(fp);
    }
    while (1) {
        printf("Select an option:\n");
        printf("1-Add Product\n");
        printf("2-Print All Products\n");
        printf("3-Print products by Number Section\n");
        printf("4-Print products Quantity Less\n");
        printf("0-Finish and save\n");
        printf("Input number: ");
        int choice;
        scanf_s("%d", &choice);
        switch (choice) {
            case 0:
                fopen_s(&fp, filename, "w");
                fwrite(&N, sizeof(int), 1, fp);
                fwrite(&col_products, sizeof(int), 1, fp);
                for (int i = 0; i < col_products; i++) {
                    fwrite(&products[i], sizeof(product), 1, fp);
                }
                fclose(fp);
                return;
            case 1:
                printf("Input cost: ");
                float cost;
                scanf_s("%f", &cost);

                scanf_s("%c");
                printf("Input name product: ");
                char *name = (char *) malloc(20);
                gets(name);
                int day, year;
                printf("Input day: ");
                scanf_s("%d", &day);
                printf("Input month: ");
                char month[20];
                scanf_s("%s", &month);
                printf("Input year: ");
                scanf_s("%d", &year);
                date dates = {day, "", year};
                strcpy(dates.month, month);
                printf("Input expiration date: ");
                int expiration_date;
                scanf_s("%d", &expiration_date);

                AddProduct(products, name, cost,
                           dates, expiration_date);
                printf("Product added\n");
                break;
            case 2:
                PrintAllProductsInfo(products);
                break;
            case 3:
                printf("Input number sections: ");
                int number_section;
                scanf_s("%d", &number_section);
                PrintProductsInfoByNumberSection(products, number_section);
                break;
            case 4:
                printf("Input quantity: ");
                int quantity;
                scanf_s("%d", &quantity);
                PrintProductsQuantityLess(products, quantity);
            default:
                printf("Incorrect input");
                break;
        }
    }
}

int main() {
    //TestProductSystem();
    Start();
    printf("Finished program");
    return 0;
}


int FindPosition(product *products, const char *name) {
    for (size_t i = 0; i < col_products; i++)
        if (!strcmp(products[i].name, name))
            return (int) i;
    return -1;
}

void AddProduct(product *products, const char *name, double cost, const date dates, int expiration_date) {
    int index_product = FindPosition(products, name);
    if (index_product == -1) {
        products[col_products].number_section = 0;
        strcpy(products[col_products].name, name);
        products[col_products].cost = cost;
        products[col_products].col = 1;
        products[col_products].expiration_date = expiration_date;
        products[col_products].dates = dates;
        col_products += 1;
    } else {
        ++products[index_product].col;
    }
}

void PrintNTimes(char c, int number) {
    for (int i = 0; i < number; i++) printf("%c", c);
    printf("\n");
}

int SortProductsByName(product *lhs, product *rhs) {
    return strcmp(lhs->name, rhs->name);
}


void PrintMainDatabaseInfo() {
    PrintNTimes('-', 70);
    printf("Database \"warehouse with products\"\n\n");
    printf("   Number \t| Name\t\t| Cost\t| Quantity | Total amount | Expiration | Date\n");
    printf("   section \t| product\t| (rub)\t| (units)  | (rub)        | date       | d/m/y\n");
}

void PrintFinalInfo(int col, double sum) {
    printf("Total sum: %.1f\n", sum);
    printf("Total quantity: %d\n", col);
    PrintNTimes('-', 70);
}

void PrintProductInfo(product item, int i) {
    printf("%02d|", i + 1);
    printf("%-15d%-16s%-4.1f   %-11d%-16.1f%-13d%-2d%-4s%-4d\n",
           item.number_section,
           item.name,
           item.cost,
           item.col,
           item.cost * item.col,
           item.expiration_date,
           item.dates.day,
           item.dates.month,
           item.dates.year);
}

void PrintAllProductsInfo(product *products) {
    PrintMainDatabaseInfo();
    int col = 0;
    double sum = 0;
    for (int i = 0; i < col_products; i++) {
        col += products[i].col, sum += products[i].col * products[i].cost;
        PrintProductInfo(products[i], i);
    }
    PrintFinalInfo(col, sum);
}

void PrintProductsQuantityLess(product *products, int quantity) {
    PrintMainDatabaseInfo();
    int col = 0;
    double sum = 0;
    for (int i = 0; i < col_products; i++) {
        if (quantity <= products[i].col) continue;
        col += products[i].col, sum += products[i].col * products[i].cost;
        PrintProductInfo(products[i], i);
    }
    PrintFinalInfo(col, sum);
}

void PrintProductsInfoByNumberSection(product *products, int number_section) {
    PrintMainDatabaseInfo();
    int col = 0;
    double sum = 0;
    qsort(products, col_products, sizeof(product),
          (int (*)(const void *, const void *)) SortProductsByName);
    for (int i = 0; i < col_products; i++) {
        if (number_section != products[i].number_section) continue;
        col += products[i].col, sum += products[i].col * products[i].cost;
        PrintProductInfo(products[i], i);
    }
    PrintFinalInfo(col, sum);
}
