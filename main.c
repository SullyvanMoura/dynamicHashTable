#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "customer.h"
#include "metadata_manager.h"

#define FILE_PATH_HASH "./files/hash.dat"

float max_load_factor = 2.0;

float load_factor(int m) {

    int records_count = count_records();

    return (float) records_count/ (float) m;
}

int needs_expand() {

    float load = load_factor(get_m());

    if (load >= max_load_factor) {

        return 1;
    }

    return 0;
}

void createTable(int m) {  

    if (get_hash_created() == 1) {

        return;
    }

    FILE *f;

    if ((f = fopen(FILE_PATH_HASH, "wb")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    int void_space_value = -1;

    for (int i = 0; i < m; i++)
    {
        fseek(f, i * sizeof(int), SEEK_SET);
        fwrite(&void_space_value, sizeof(int), 1, f);
    }

    set_m(m);
    set_old_m(m);
    set_p(0);
    set_l(0);
    set_hash_created(1);

    fclose(f);
}

void read_integers_from_table() {

    FILE *f;
    int n;

    if ((f = fopen(FILE_PATH_HASH, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    printf("--------------------------------\n");

    for (int i = 0; i < get_m(); i++) {

        fread(&n, sizeof(int), 1, f);
        printf("%d [%d]\n", i, n);
    }
    printf("--------------------------------\n");
    fclose(f);
}

void insert_value_in_table(int value, int pos) {
    
    FILE *f;

    if ((f = fopen(FILE_PATH_HASH, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    // rewind(f);
    fseek(f, pos*sizeof(int), SEEK_SET);
    fwrite(&value, sizeof(int), 1, f);

    fclose(f);
}

int get_hash_value_from_key(int key) {

    FILE *f;
    int value;

    if ((f = fopen(FILE_PATH_HASH, "rb")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fseek(f, key*sizeof(int), SEEK_SET);
    fread(&value, sizeof(int), 1, f);

    fclose(f);

    return value;
}

void expand_list(int hash_index) {

    int list_index = get_hash_value_from_key(hash_index);

    reorgazine_expanded_list(list_index, hash_index);
}

void expand_table() {

    FILE *f;
    int n;

    printf("Expand!!!\n");

    if ((f = fopen(FILE_PATH_HASH, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    //Adiciona novo espaço vazio
    int void_space_value = -1;
    set_m(get_m() + 1);

    rewind(f);
    fseek(f, 0, SEEK_END);
    fwrite(&void_space_value, sizeof(int), 1, f);

    //Reorganiza lista que está sendo expandida
    expand_list(get_p());

    //Avança o P ou retorna ao início da tabela
    if (get_p() + 1 == get_old_m())
        set_p(0);
    else
        set_p(get_p() + 1);

    fclose(f);
}

void insert_customer_in_hash_table(Customer *c) {

    int cod = c->cod;
    
    int pos = hash(cod, get_m(), get_l());

    if (pos < get_p()) {

        pos = hash(cod, get_m(), get_l() + 1);
    }

    int value_in_hash_table = get_hash_value_from_key(pos);

    if (value_in_hash_table != -1) {

        insert_customer(c, value_in_hash_table);
    } else {

        int list_index = insert_customer(c, last_index_stored() + 1);
        insert_value_in_table(list_index, pos);
    }

    if (needs_expand()) {

        expand_table();
    }
}

void print_hash_table() {

    FILE *f;
    int n;

    if ((f = fopen(FILE_PATH_HASH, "rb")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    for (int i = 0; i < get_m(); i++) {

        fread(&n, sizeof(int), 1, f);

        printf("%d [%d]", i, n);
        print_list(n);
    }

    fclose(f);
}


int main() {

    createTable(5);

    //expand_table();
    // expand_table();
    // expand_table();
    // expand_table();
    // expand_table();

    Customer *c1 = new_customer(50, "JOAO");
    insert_customer_in_hash_table(c1);
    free(c1);

    //read_integers_from_table();

    Customer *c2 = new_customer(55, "CARLA");
    insert_customer_in_hash_table(c2);
    free(c2);

    //read_integers_from_table();

    Customer *c3 = new_customer(59, "MARIA");
    insert_customer_in_hash_table(c3);
    free(c3);

    //read_integers_from_table();
    
    Customer *c4 = new_customer(3, "JOSE");
    //insert_customer_in_hash_table(c4);
    free(c4);

    //read_integers_from_table();

    Customer *c5 = new_customer(87, "BIA");
    insert_customer_in_hash_table(c5);
    free(c5);

    //read_integers_from_table();

    Customer *c6 = new_customer(103, "ANA");
    insert_customer_in_hash_table(c6);
    free(c6);

    // Customer *c7 = new_customer(40, "Mary");
    // insert_customer_in_hash_table(c7, f);
    // free(c7);

    // Customer *c8 = new_customer(41, "Mia");
    // insert_customer_in_hash_table(c8, f);
    // free(c8);

    // Customer *c9 = new_customer(88, "Jonas");
    // insert_customer_in_hash_table(c9, f);
    // free(c9);

    // Customer *c10 = new_customer(100, "Baleia");
    // insert_customer_in_hash_table(c10, f);
    // free(c10);

    printf("This is the m's value: %d\n", get_m());

    print_hash_table();

    printf("Records Count: %d\n", count_records());

    printf("Load Factor: %f\n", load_factor(get_m()));

    // while (1) {
    //     printf("Qual valor deseja inserir? ");
    //     scanf("%d", &value);

    //     insert_customer_in_hash_table(value);
    //     read_integers_from_table(f);    
    // }

    printf("M: %d\n", get_m());

    return 0;
}