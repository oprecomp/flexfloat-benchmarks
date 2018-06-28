/*
   Copyright 2018 - The OPRECOMP Project Consortium, Alma Mater Studiorum
   Università di Bologna. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "flexfloat.hpp"

FILE *open_data(const char *file, int *rows, int *columns)
{
    FILE *data = fopen(file, "r");
    if (data == NULL) {
        perror(file);
        exit(1);
    }

    // read data size
    if (fscanf(data, "%d %d\n", rows, columns) != 2) {
        perror("Error reading header");
        exit(1);
    }
    return data;
}

char ***read_category(FILE *data, int columns)
{
    char buffer[1024];
    char ***cat = (char***) malloc(sizeof(char **) * columns);
    for (int i = 0; i < columns; i++) {
        if (fgets(buffer, sizeof(buffer), data) == NULL) {
            perror("Error reading categories");
            exit(1);
        }
        char *t = strtok(buffer, ",\n");
        if (t == NULL) {
            fprintf(stderr, "Error reading categories\n");
            exit(1);
        }
        int n = atoi(t);
        if (n <= 0) cat[i] = NULL;
        else {
            cat[i] = (char **)malloc(sizeof(char *) * n);
            for (int j = 0; j < n; j++) {
                t = strtok(NULL, ",\n");
                if (t == NULL) {
                    fprintf(stderr, "Error reading categories\n");
                    exit(1);
                }
                cat[i][j] = strcpy((char *)malloc((strlen(t) + 1) * sizeof(char)), t);
            }
        }
    }
    return cat;
}

flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *read_data(FILE *data, int rows, int columns)
{
    double temp;
    flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *x = (flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *) malloc(rows * columns * sizeof(flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X>));
    if (x == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (fscanf(data, j == 0 ? "%lf" : ",%lf", &temp) != 1) {
                fprintf(stderr, "Error reading data row %d column %d\n", i, j);
                exit(1);
            }
            x[i * columns + j] = temp;
        }
    }
    return x;
}

struct aux {
    flexfloat<EXP_STRUCT_AUX_DIST, FRAC_STRUCT_AUX_DIST> dist;
    int index;
};

int compar(const void *a, const void *b)
{
    flexfloat<EXP_COMPAR_C, FRAC_COMPAR_C> c = ((struct aux *)a)->dist;
    flexfloat<EXP_COMPAR_C, FRAC_COMPAR_C> d = ((struct aux *)b)->dist;
    if (flexfloat<EXP_COMPAR_TEMP1, FRAC_COMPAR_TEMP1>(c) < flexfloat<EXP_COMPAR_TEMP1, FRAC_COMPAR_TEMP1>(d))  return -1;
    else if (flexfloat<EXP_COMPAR_TEMP1, FRAC_COMPAR_TEMP1>(c) > flexfloat<EXP_COMPAR_TEMP1, FRAC_COMPAR_TEMP1>(d))  return 1;
    else return 0;
}

int vote(int K, flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *x, int rows, int columns, flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *data, struct aux *v)
{
    for (int i = 0; i < rows; i++) {
        flexfloat<EXP_VOTE_A, FRAC_VOTE_A> a = 0.0;
        // flexfloat_vectorization = true;
        for (int j = 0; j < columns; j++) {
            flexfloat<EXP_VOTE_B, FRAC_VOTE_B> b = (flexfloat<EXP_VOTE_TEMP1, FRAC_VOTE_TEMP1>(data[i * columns + j]) - flexfloat<EXP_VOTE_TEMP1, FRAC_VOTE_TEMP1>(x[j]));
            a += b * b;
        }
        // flexfloat_vectorization = false;
        v[i].dist = flexfloat<EXP_STRUCT_AUX_DIST, FRAC_STRUCT_AUX_DIST>(a);
        v[i].index = i;
    }

    qsort(v, rows, sizeof(struct aux), compar);

    int c[1]; c[0] = 0; c[1] = 0;
    for (int i = 1; i < K + 1; i++) { // first element is himself
        // ballot (assumes binary clasification)
        if (data[v[i].index * columns + columns - 1] == 0) c[0]++;
        else c[1]++;
    }
    if (c[0] >= c[1]) return 0;
    else return 1;
}

int main(int argc, const char* argv[])
{

    int K, rows, columns, samples;
    K = 10;
    samples = 1000;
    FILE *f = open_data("./adult.data", &rows, &columns);

    read_category(f, columns);
    flexfloat<EXP_MALLOC_X, FRAC_MALLOC_X> *x = read_data(f, rows, columns);
    fclose(f);

    struct aux *aux = (struct aux *) malloc(rows * sizeof(struct aux));
    if (aux == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    int TP = 0, TN = 0, FP = 0, FN = 0;

/*
    printf("# K: %d\n", K);
    printf("# datafile: %s\n", argv[2]);
    printf("# problem_size: %d\n", rows);
    printf("# dimension: %d\n", columns);
    printf("# samples: %d\n", samples);
*/
    for (int k = 0; k < samples; k++) {
        int r = vote(K, x + k * columns, rows, columns, x, aux); // ballot result
        int c = double(x[k * columns + columns - 1]); // true value
        if (r == c) {
            if (r == 0) TN++;
            else TP++;
        } else {
            if (r == 0) FN++;
            else FP++;
        }
    }

/*
    printf("# TP: %d\n", TP);
    printf("# TN: %d\n", TN);
    printf("# FP: %d\n", FP);
    printf("# FN: %d\n", FN);
    printf("# Sensitiviy:  %f\n", (double)TP / (TP + FN));
    printf("# Specificity: %f\n", (double)TN / (TN + FP));
*/
    printf("%.16f,%.16f,", (double)TP / (TP + FN), (double)TN / (TN + FP));

    // print_flexfloat_stats();

    return 0;
}
