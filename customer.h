#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdio.h>
#include <stdlib.h>

struct Customer
{
    int cod;
    char name[100];
    int next;
};
typedef struct Customer Customer;

int hash(int x, int m, int l);

Customer *new_customer(int cod, char *name) ;

void print_list(int index_in_file);

Customer *find_customer(int cod, int initial_position, int *last_position_read, int *is_first, FILE *f);

Customer *read_customer(int index_in_file, FILE *f);

int insert_customer(Customer *c, int initial_position);

int last_index_stored();

int count_records();

void reorgazine_expanded_list(int lst_idx, int actual_hash_idx);

#endif