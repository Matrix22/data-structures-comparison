/**
 * @file hash_table_test_nosize.c
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2022-12-18
 * 
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of data-structures-comparison.
 *
 * data-structures-comparison is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * data-structures-comparison is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with data-structures-comparison.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "../../algorithms/src/include/hash_table_linked.h"
#include <time.h>

#define MAX_FILE_NAME 35
#define MAX_TITLE_NAME 15

#define COMPUTE_TIME(time_clock) (((double)time_clock) / CLOCKS_PER_SEC)

static const char padding[] = "........................";

size_t hash(int32_t key) {
    return (size_t)key;
}

static uint8_t run_test(char *load_filename, char *res_filename, char *in_filename) {
    FILE *file = NULL;

    if ((file = fopen(load_filename, "r")) == NULL) {
        printf("File <%s> could NOT be opened for data loaded\n", load_filename);
        
        return 1;
    }

    double insert_time = 0;
    double delete_time = 0;
    double includes_time = 0;
    double modify_time = 0;
    double free_time = 0;

    clock_t temp;

    hash_table_linked_t *ht = create_hash_table(0, hash);

    if (ht == NULL) {
        printf("Table could NOT be created for testing\n");
        fclose(file);

        return 1;
    }

    /* Data loader */

    int32_t data_size = 0;
    fscanf(file, "%d", &data_size);

    for (int32_t iter = 0; iter < data_size; ++iter) {
        int32_t query = 0, data = 0;

        fscanf(file, "%d%d", &query, &data);

        if (query != 0) {
            printf("Loading data: Wrong query for loading data <%d>\n", query);
        } else {
            temp = clock();
            error_t err = hash_table_insert(ht, data);
            temp = clock() - temp;

            if (err != SCL_OK) {
                printf("Loading data: Could not insert <%d> in the table\n", data);
            } else {
                insert_time += COMPUTE_TIME(temp);
            }
        }
    }

    fclose(file);

    /* Query execution */
    if ((file = fopen(in_filename, "r")) == NULL) {
        printf("Query: Could not open file for reading queries\n");
        free_hash_table(ht);

        return 1;
    }

    fscanf(file, "%d", &data_size);
    for (int32_t iter = 0; iter < data_size; ++iter) {
        int32_t query = 0;

        fscanf(file, "%d", &query);

        if (query == 0) {
            int32_t data = 0;

            fscanf(file, "%d", &data);

            temp = clock();
            error_t err = hash_table_insert(ht, data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                insert_time += COMPUTE_TIME(temp);
            }
        } else if (query == 1) {
            int32_t data = 0;

            fscanf(file, "%d", &data);

            temp = clock();
            error_t err = hash_table_delete(ht, data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                delete_time += COMPUTE_TIME(temp);
            }
        } else if (query == 2) {
            int32_t data = 0;

            fscanf(file, "%d", &data);

            temp = clock();
            hash_table_includes(ht, data);
            temp = clock() - temp;
            includes_time += COMPUTE_TIME(temp);
        } else if (query == 3) {
            int32_t old_data = 0;
            int32_t new_data = 0;

            fscanf(file, "%d%d", &old_data, &new_data);

            temp = clock();
            error_t err = hash_table_modify(ht, old_data, new_data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                modify_time += COMPUTE_TIME(temp);
            }
        } else if (query == 4) {
            // Not checking
        } else {
            printf("Queries: Unknown query number <%d>\n", query);
        }
    }

    fclose(file);

    /* Freeing memory */
    temp = clock();
    error_t err = free_hash_table(ht);
    temp = clock() - temp;

    if (err != SCL_OK) {
        printf("Free: Could not free the tree\n");
    } else {
        free_time += COMPUTE_TIME(temp);
    }

    if ((file = fopen(res_filename, "w")) == NULL) {
        printf("Could not open file for statistics\n");

        return 1;
    }

    fprintf(file, "Insert time: %lfsec\n", insert_time);
    fprintf(file, "Delete time: %lfsec\n", delete_time);
    fprintf(file, "Includes time: %lfsec\n", includes_time);
    fprintf(file, "Modify time: %lfsec\n", modify_time);
    fprintf(file, "Free time: %lfsec\n", free_time);

    fclose(file);

    return 0;
}

static void run_tests(void) {
    for (int32_t test_idx = 1; test_idx <= 30; ++test_idx) {
        char load_filename[MAX_FILE_NAME];
        char res_filename[MAX_FILE_NAME];
        char in_filename[MAX_FILE_NAME];
        
        snprintf(load_filename, MAX_FILE_NAME, "load/insert%d.in", test_idx);
        snprintf(res_filename, MAX_FILE_NAME, "res/hash_table_nosize/res%d.out", test_idx);
        snprintf(in_filename, MAX_FILE_NAME, "in/test%d.in", test_idx);

        char title[MAX_TITLE_NAME];
        snprintf(title, MAX_TITLE_NAME, "Test %d", test_idx);
        
        printf("%s %s ", title, padding + strlen(title));
        uint8_t err = run_test(load_filename, res_filename, in_filename);

        if (err == 1) {
            printf("Error\n");
        } else {
            printf("Done\n");
        }
    }
}

int main(void) {
    run_tests();

    printf("Hash Table No Size Test Solved\n");

    return 0;
}