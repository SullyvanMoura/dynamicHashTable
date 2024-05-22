#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include "metadata_manager.h"

void create_costumers_file() {

    FILE *f;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "w")) == NULL) {
        printf("Erro ao abrir arquivo no método print_list\n");
        exit(1);
    }

    printf("Customers file created!\n");

    fclose(f);
}

Customer *new_customer(int cod, char *name) {

    Customer *c = (Customer *) malloc(sizeof(Customer));

    if (c) memset(c, 0, sizeof(Customer));

    c->cod = cod;
    strcpy(c->name, name);
    c->next = -1;

    return c;
}

void print_customer(Customer *c) {

    if (c != NULL) {

        printf("**********************************************\n");
        printf("Customer code: %d\n", c->cod);
        printf("Customer name: %s\n", c->name);
        printf("Next list index: %d\n", c->next);
        printf("**********************************************\n");
    }
}

void print_list(int index_in_file) {

    FILE *f;
    Customer *c;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método print_list\n");
        exit(1);
    }

    printf("->");
    for (c = read_customer(index_in_file, f); c != NULL; c = read_customer(c->next, f)) 
        printf("[ Cod: %d | Name: %s | next: %d ] -> ", c->cod, c->name, c->next);

    printf("[NULL]\n");

    fclose(f);
}

int customer_size_in_bytes() {
    
    return sizeof(int) + //Cod
    sizeof(char)*100 + //Name
    sizeof(int); //Next
}

Customer *find_customer(int cod, int index_in_file, int *last_position_read, int *is_first, FILE *f) {

    Customer *c;
    *is_first = 1;
    int position = index_in_file;

    for (c = read_customer(index_in_file, f); c != NULL; c = read_customer(c->next, f)) {

        *last_position_read = (ftell(f)/customer_size_in_bytes()) - 1;

        //print_customer(c);
        if (c->cod == cod) {
            return c;
        }

        *is_first = 0;
        position = c->next;
    }

    return NULL;
}

int insert_customer(Customer *c, int index_in_file) {

    FILE *f;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método insert_customer\n");
        exit(1);
    }

    int *last_position_read = (int *) malloc(sizeof(int));
    *last_position_read = -1;

    int *is_first = (int *) malloc(sizeof(int));
    *is_first = 1;

    int is_saved_at_position;

    Customer *customer_found = find_customer(c->cod, index_in_file, last_position_read, is_first, f);

    if (customer_found != NULL) {
        
        is_saved_at_position = (ftell(f)/customer_size_in_bytes()) - 1;
        return is_saved_at_position;
    }
        
    fseek(f, 0, SEEK_END);
    
    fwrite(&c->cod, sizeof(int), 1, f);
    fwrite(c->name, sizeof(char), sizeof(c->name), f);
    fwrite(&c->next, sizeof(int), 1, f);

    is_saved_at_position = (ftell(f)/customer_size_in_bytes()) - 1;

    //Esse if atualiza o ponteiro do último registro da lista para apontar para o novo
    if (!(*is_first)) {

        is_saved_at_position = (ftell(f)/customer_size_in_bytes()) - 1;

        //Substitui o valor do ponteiro
        fseek(f, (*last_position_read)*customer_size_in_bytes() + sizeof(int) + sizeof(char)*100, SEEK_SET);
        fwrite(&is_saved_at_position, sizeof(int), 1, f);
    }

    free(last_position_read);
    free(is_first);

    fclose(f);

    return is_saved_at_position;
}

Customer *read_customer(int index_in_file, FILE *f) {

    if (index_in_file < 0) {

        return NULL;
    }

    Customer *c = (Customer *) malloc(sizeof(Customer));

    rewind(f);
    fseek(f, index_in_file*customer_size_in_bytes(), SEEK_SET);

    if (fread(&c->cod, sizeof(int), 1, f) <= 0) {

        free(c);
        return NULL;
    }

    fread(c->name, sizeof(char), sizeof(c->name), f);
    fread(&c->next, sizeof(int), 1, f);

    return c;
}

int last_index_stored() {

    FILE *f;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método last_index_stored\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    int index = ftell(f)/customer_size_in_bytes();

    fclose(f);

    return index;
}

int count_records() {

    FILE *f;
    Customer *c;
    int count = 0;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método count_records\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    count = ftell(f)/customer_size_in_bytes();

    fclose(f);

    return count;
}

int test() {

    Customer *c_aux1 = new_customer(5, "Sullyvan");
    int result1 = insert_customer(c_aux1, 0);

    printf("Inseriu o 1° na pos %d\n", result1);

    Customer *c_aux2 = new_customer(6, "Maria");
    int result2 = insert_customer(c_aux2, 0);

    printf("Inseriu o 2° na pos %d\n", result2);

    Customer *c_aux3 = new_customer(8, "Joao");
    int result3 = insert_customer(c_aux3, 0);

    printf("Inseriu o 3° na pos %d\n", result3);

    Customer *c_aux4 = new_customer(8, "Joao");
    int result4 = insert_customer(c_aux4, 0);

    printf("Inseriu o 4° na pos %d\n", result4);

    Customer *c_aux5 = new_customer(10, "Barbara");
    int result5 = insert_customer(c_aux5, 0);

    printf("Inseriu o 5° na pos %d\n", result5);

    Customer *c_aux6 = new_customer(21, "Max");
    int result6 = insert_customer(c_aux6, 0);

    printf("Inseriu o 6° na pos %d\n", result6);

    Customer *c_aux7 = new_customer(23, "Joanna");
    int result7 = insert_customer(c_aux7, 0);

    printf("Inseriu o 7° na pos %d\n", result7);

    Customer *c_aux8 = new_customer(40, "Dinho");
    int result8 = insert_customer(c_aux8, 0);

    printf("Inseriu o 8° na pos %d\n", result8);

    print_list(0);

    Customer *c_aux9 = new_customer(50, "Kurt");
    int result9 = insert_customer(c_aux9, 0);

    printf("Inseriu o 9° na pos %d\n", result9);

    print_list(0);

    Customer *c_aux10 = new_customer(27, "Ashley");
    int result10 = insert_customer(c_aux10, 0);

    printf("Inseriu o 10° na pos %d\n", result10);

    print_list(0);

    free(c_aux1);
    free(c_aux2);
    free(c_aux3);
    free(c_aux4);
    free(c_aux5);
    free(c_aux6);
    free(c_aux7);
    free(c_aux8);
    free(c_aux9);
    free(c_aux10);
    
    //Customer *c = read_customer(10);
    //Customer *c = find_customer(8, 0, NULL, f);
    //print_customer(c);

    return 0;
}

// int main() {
//     test();
//     printf("%d\n", last_index_stored());

//     return 0;
// }