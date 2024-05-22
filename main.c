#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "customer.h"
#include "metadata_manager.h"

#define FILE_PATH_HASH "./files/hash.dat"

void print_hash_table();

float max_load_factor = 1.2;

int hash(int x, int m, int l) {

    return x % (int) (m*pow(2, l));
}

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

void create_table(int m) {  

    if (get_hash_created() == 1) {

        return;
    }

    create_costumers_file();

    FILE *f;

    if ((f = fopen(FILE_PATH_HASH, "wb")) == NULL) {
        printf("Erro ao abrir arquivo no método create_table\n");
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

    if (list_index == -1) {

        return;
    }

    FILE *f;

    if ((f = fopen(FILE_PATH_CUSTOMERS, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método reorgazine_expanded_list\n");
        exit(1);
    }

    int next_customer_index_in_file = -1;
    int actual_customer_index_in_file = list_index;
    int last_read_customer_index_in_file = -1;
    int is_first = 1;

    for (Customer *c = read_customer(list_index, f); c != NULL; c = read_customer(next_customer_index_in_file, f)) {
        
        next_customer_index_in_file = c->next;

        if (get_hash_value_from_key(hash_index) == actual_customer_index_in_file) {

            is_first = 1;
        }

        int new_hash_pos = hash(c->cod, get_old_m(), get_l() + 1);

        if (new_hash_pos != hash_index) {

            int value_from_new_space = get_hash_value_from_key(new_hash_pos);

            if (is_first) {

                //move o próximo da lista para a primeira posição da lista
                insert_value_in_table(c->next, hash_index);
            } else {

                //Volta para atualizar o ponteiro do customer anterior e aprontar para o próximo
                fseek(f, customer_size_in_bytes()*(last_read_customer_index_in_file + 1)- sizeof(int) , SEEK_SET);
                fwrite(&c->next, sizeof(int), 1, f);

                fflush(f);

                //avança o ponteiro para o final do customer atual (para não quebrar o código logo ali na frente)
                fseek(f, customer_size_in_bytes()*(actual_customer_index_in_file + 1), SEEK_SET);

            }

            if (value_from_new_space == -1) {
                
                int value = -1;

                //move atual pra nova posição
                insert_value_in_table(actual_customer_index_in_file, new_hash_pos);

                //aproveita a posição final do read_customer pra atualizar o ponteiro do que foi movido
                fseek(f, -sizeof(int), SEEK_CUR);
                fwrite(&value, sizeof(int), 1, f);
                fflush(f);
            } else {

                for (Customer *c2 = read_customer(value_from_new_space, f); c2 != NULL; c = read_customer(c2->next, f)) {
                //TODO

                    if (c2->next == -1) {

                        fseek(f, -sizeof(int), SEEK_CUR);
                        fwrite(&actual_customer_index_in_file, sizeof(int), 1, f);
                        fflush(f);
                        free(c2);
                        break;
                    }
                }
            }
        }

        is_first = 0;
        last_read_customer_index_in_file = actual_customer_index_in_file;
        actual_customer_index_in_file = next_customer_index_in_file;
        free(c);
    }

    fclose(f);
}

void expand_table() {

    FILE *f;
    int n;

    printf("load factor = %f. ", load_factor(get_m()));

    printf("Expanding p = %d\n", get_p());

    if ((f = fopen(FILE_PATH_HASH, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    //Adiciona novo espaço vazio
    int void_space_value = -1;
    set_m(get_m() + 1);

    fseek(f, 0, SEEK_END);
    fwrite(&void_space_value, sizeof(int), 1, f);

    //Tive que colocar pq tava dando problema :p
    fflush(f);

    //Reorganiza lista que está sendo expandida
    expand_list(get_p());

    //Avança o P ou retorna ao início da tabela
    if (get_p() + 1 == (get_old_m()*pow(2, get_l()))){

        set_p(0);
        set_l(get_l() + 1);
    }
    else
        set_p(get_p() + 1);

    fclose(f);
}

void insert_customer_in_hash_table(Customer *c) {

    printf("Inserting customer - Cod: %d | Name: %s - ", c->cod, c->name);

    int cod = c->cod;
    
    int pos = hash(cod, get_old_m(), get_l());

    if (pos < get_p()) {

        pos = hash(cod, get_old_m(), get_l() + 1);
    }

    printf("in position %d! ", pos);
    printf("(m = %d, p = %d)\n", get_m(), get_p());

    int value_in_hash_table = get_hash_value_from_key(pos);

    if (value_in_hash_table != -1) {

        insert_customer(c, value_in_hash_table);
    } else {

        int list_index = insert_customer(c, last_index_stored() + 1);
        insert_value_in_table(list_index, pos);
    }

    print_hash_table();

    if (needs_expand()) {

        expand_table();
        print_hash_table();
    }
}

void print_hash_table() {

    FILE *f;
    int n;

    if ((f = fopen(FILE_PATH_HASH, "rb")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    for (int i = 0; i < get_m(); i++) {

        fread(&n, sizeof(int), 1, f);

        printf("%d [%d]", i, n);
        print_list(n);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fclose(f);
}

int main_test_1() {

    //Esse é o exercício que ta no slide

    create_table(7);
    max_load_factor = 1.0;

    Customer *c1 = new_customer(49, "JOAO");
    insert_customer_in_hash_table(c1);
    free(c1);

    Customer *c2 = new_customer(51, "CARLA");
    insert_customer_in_hash_table(c2);
    free(c2);

    Customer *c3 = new_customer(59, "MARIA");
    insert_customer_in_hash_table(c3);
    free(c3);

    Customer *c4 = new_customer(3, "JOSE");
    insert_customer_in_hash_table(c4);
    free(c4);

    Customer *c5 = new_customer(87, "BIA");
    insert_customer_in_hash_table(c5);
    free(c5);

    Customer *c6 = new_customer(103, "ANA");
    insert_customer_in_hash_table(c6);
    free(c6);

    Customer *c7 = new_customer(7, "CARLOS");
    insert_customer_in_hash_table(c7);
    free(c7);

    Customer *c8 = new_customer(8, "JOAO");
    insert_customer_in_hash_table(c8);
    free(c8);

    Customer *c9 = new_customer(14, "CAROL");
    insert_customer_in_hash_table(c9);
    free(c9);

    Customer *c10 = new_customer(15, "PEDRO");
    insert_customer_in_hash_table(c10);
    free(c10);

    printf("Final table: \n");
    print_hash_table();

    printf("Records Count: %d\n", count_records());
    printf("M: %d\n", get_m());
    printf("Load Factor: %f\n", load_factor(get_m()));

    return 0;
}

int main_test_2() {

    //Esse é o exercício que foi feito em sala antes da prova

    create_table(4);
    max_load_factor = 1.2;

    Customer *c1 = new_customer(5, "JOAO");
    insert_customer_in_hash_table(c1);
    free(c1);

    Customer *c2 = new_customer(6, "PEDRO");
    insert_customer_in_hash_table(c2);
    free(c2);

    Customer *c3 = new_customer(3, "MARIA");
    insert_customer_in_hash_table(c3);
    free(c3);

    Customer *c4 = new_customer(15, "ANTONIO");
    insert_customer_in_hash_table(c4);
    free(c4);

    Customer *c5 = new_customer(4, "ANA");
    insert_customer_in_hash_table(c5);
    free(c5);

    Customer *c6 = new_customer(12, "JOSE");
    insert_customer_in_hash_table(c6);
    free(c6);

    Customer *c7 = new_customer(10, "BIA");
    insert_customer_in_hash_table(c7);
    free(c7);

    Customer *c8 = new_customer(11, "CARLA");
    insert_customer_in_hash_table(c8);
    free(c8);

    Customer *c9 = new_customer(8, "CARLOS");
    insert_customer_in_hash_table(c9);
    free(c9);

    Customer *c10 = new_customer(23, "RAFAEL");
    insert_customer_in_hash_table(c10);
    free(c10);

    Customer *c11 = new_customer(23, "RAFAEL");
    insert_customer_in_hash_table(c11);
    free(c11);

    Customer *c12 = new_customer(36, "FATIMA");
    insert_customer_in_hash_table(c11);
    free(c12);

    printf("Final table: \n");
    print_hash_table();

    printf("Records Count: %d\n", count_records());
    printf("M: %d\n", get_m());
    printf("Load Factor: %f\n", load_factor(get_m()));

    return 0;
}

int main_test_3() {

    create_table(2);
    max_load_factor = 1.0;

    Customer *c1 = new_customer(0, "JOAO");
    insert_customer_in_hash_table(c1);
    free(c1);

    Customer *c2 = new_customer(2, "PEDRO");
    insert_customer_in_hash_table(c2);
    free(c2);

    Customer *c3 = new_customer(4, "MARIA");
    insert_customer_in_hash_table(c3);
    free(c3);

    Customer *c4 = new_customer(6, "ANTONIO");
    insert_customer_in_hash_table(c4);
    free(c4);

    Customer *c5 = new_customer(8, "ANA");
    insert_customer_in_hash_table(c5);
    free(c5);

    Customer *c6 = new_customer(10, "JOSE");
    insert_customer_in_hash_table(c6);
    free(c6);

    Customer *c7 = new_customer(12, "BIA");
    insert_customer_in_hash_table(c7);
    free(c7);

    Customer *c8 = new_customer(14, "CARLA");
    insert_customer_in_hash_table(c8);
    free(c8);

    Customer *c9 = new_customer(200, "CARLOS");
    insert_customer_in_hash_table(c9);
    free(c9);

    printf("Final table: \n");
    print_hash_table();

    printf("Records Count: %d\n", count_records());
    printf("M: %d\n", get_m());
    printf("Load Factor: %f\n", load_factor(get_m()));

    return 0;
}

int main() {

    //Rodar o comando "make rm" antes trocar entre os teste_1 e teste_2
    //para apagar os dados e metadados

    main_test_1();
    //main_test_2();
    //main_test_3();

    return 0;
}