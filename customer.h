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

void create_costumers_file();

#define FILE_PATH_CUSTOMERS "./files/customers.dat"

Customer *new_customer(int cod, char *name) ;

int customer_size_in_bytes();

void print_list(int index_in_file);

Customer *find_customer(int cod, int initial_position, int *last_position_read, int *is_first, FILE *f);

Customer *read_customer(int index_in_file, FILE *f);

int insert_customer(Customer *c, int initial_position);

int last_index_stored();

int count_records();

#endif