#include <stdlib.h>
#include <stdio.h>
#include "metadata_manager.h"

//Ordem do arquivo:
//CREATED
//M
//L
//P
//OLD_M

#define FILE_PATH_METADATA "./files/metadata.dat"

int set_hash_created(int created) {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método set_hash_created\n");
        exit(1);
    }

    rewind(f);
    fwrite(&created, sizeof(int), 1, f);

    fclose(f);
}

int get_hash_created() {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método get_hash_created\n");
        exit(1);
    }

    int created;

    rewind(f);
    fread(&created, sizeof(int), 1, f);

    fclose(f);

    return created;
}

int set_m(int m) {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método set_m\n");
        exit(1);
    }

    fseek(f, sizeof(int), SEEK_SET);
    fwrite(&m, sizeof(int), 1, f);

    fclose(f);
}

int get_m() {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método get_m\n");
        exit(1);
    }

    int m;

    fseek(f, sizeof(int), SEEK_SET);
    fread(&m, sizeof(int), 1, f);

    fclose(f);

    return m;
}

int set_l(int l) {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método set_l\n");
        exit(1);
    }

    fseek(f, 2*sizeof(int), SEEK_SET);
    fwrite(&l, sizeof(int), 1, f);

    fclose(f);
}

int get_l() {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método get_l\n");
        exit(1);
    }

    int l;

    fseek(f, 2*sizeof(int), SEEK_SET);
    fread(&l, sizeof(int), 1, f);

    fclose(f);

    return l;
}

int set_p(int p) {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método set_p\n");
        exit(1);
    }

    fseek(f, 3*sizeof(int), SEEK_SET);
    fwrite(&p, sizeof(int), 1, f);

    fclose(f);
}

int get_p() {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método get_p\n");
        exit(1);
    }

    int p;

    fseek(f, 3*sizeof(int), SEEK_SET);
    fread(&p, sizeof(int), 1, f);

    fclose(f);

    return p;
}

int set_old_m(int old_m) {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb+")) == NULL) {
        printf("Erro ao abrir arquivo no método set_old_m\n");
        exit(1);
    }

    fseek(f, 4*sizeof(int), SEEK_SET);
    fwrite(&old_m, sizeof(int), 1, f);

    fclose(f);
}

int get_old_m() {

    FILE *f;

    if ((f = fopen(FILE_PATH_METADATA, "rb")) == NULL) {
        printf("Erro ao abrir arquivo no método get_old_l\n");
        exit(1);
    }

    int old_m;

    fseek(f, 4*sizeof(int), SEEK_SET);
    fread(&old_m, sizeof(int), 1, f);

    fclose(f);

    return old_m;
}

int test_metadata () {

    // set_hash_created(5);
    // printf("%d\n", get_hash_created());

    // set_m(10);
    // printf("%d\n", get_m());

    // set_l(3);
    // printf("%d\n", get_l());

    set_p(20);
    printf("%d\n", get_p());    

    return 0;
}