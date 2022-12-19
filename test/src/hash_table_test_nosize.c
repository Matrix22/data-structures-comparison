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

static uint8_t run_test(char *in_filename, char *out_filename, char *res_filename) {
    FILE *fin = NULL;

    if ((fin = fopen(in_filename, "r")) == NULL) {
        printf("File <%s> could NOT be opened for reading\n", in_filename);

        return 1;
    }

    FILE *fout = NULL;
    if ((fout = fopen(out_filename, "w")) == NULL) {
        printf("Query: Could not open file for writing queries\n");
        fclose(fin);

        return 1;
    }

    hash_table_linked_t *ht = create_hash_table(0, hash);

    if (ht == NULL) {
        printf("Hash Table could NOT be created for testing\n");
        fclose(fin);
        fclose(fout);

        return 1;
    }

    /* Queries */

    double insert_time = 0;
    double delete_time = 0;
    double includes_time = 0;
    double modify_time = 0;
    double print_time = 0;
    double free_time = 0;

    clock_t temp;

    int32_t data_size = 0;
    fscanf(fin, "%d", &data_size);

    for (int32_t iter = 0; iter < data_size; ++iter) {
        int32_t query = 0;

        fscanf(fin, "%d", &query);

        if (query == 0) {
            int32_t data = 0;

            fscanf(fin, "%d", &data);

            temp = clock();
            error_t err = hash_table_insert(ht, data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                insert_time += COMPUTE_TIME(temp);
            }
        } else if (query == 1) {
            int32_t data = 0;

            fscanf(fin, "%d", &data);

            temp = clock();
            error_t err = hash_table_delete(ht, data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                delete_time += COMPUTE_TIME(temp);
            }
        } else if (query == 2) {
            int32_t data = 0;

            fscanf(fin, "%d", &data);

            temp = clock();
            uint8_t check = hash_table_includes(ht, data);
            temp = clock() - temp;
            includes_time += COMPUTE_TIME(temp);

            fprintf(fout, "%hu\n", check);
        } else if (query == 3) {
            int32_t old_data = 0;
            int32_t new_data = 0;

            fscanf(fin, "%d%d", &old_data, &new_data);

            temp = clock();
            error_t err = hash_table_modify(ht, old_data, new_data);
            temp = clock() - temp;

            if (err == SCL_OK) {
                modify_time += COMPUTE_TIME(temp);
            }
        } else if (query == 4) {
            temp = clock();
            error_t err = hash_table_traverse_inorder(ht, fout);
            temp = clock() - temp;

            if (err == SCL_OK) {
                print_time += COMPUTE_TIME(temp);
            }
        } else {
            printf("Queries: Unknown query number <%d>\n", query);
        }
    }

    fclose(fin);
    fclose(fout);

    /* Freeing memory */
    temp = clock();
    error_t err = free_hash_table(ht);
    temp = clock() - temp;

    if (err != SCL_OK) {
        printf("Free: Could not free the table\n");
    } else {
        free_time += COMPUTE_TIME(temp);
    }

    if ((fout = fopen(res_filename, "w")) == NULL) {
        printf("Could not open fin for statistics\n");

        return 1;
    }

    fprintf(fout, "Insert time: %lfsec\n", insert_time);
    fprintf(fout, "Delete time: %lfsec\n", delete_time);
    fprintf(fout, "Includes time: %lfsec\n", includes_time);
    fprintf(fout, "Modify time: %lfsec\n", modify_time);
    fprintf(fout, "Print time: %lfsec\n", print_time);
    fprintf(fout, "Free time: %lfsec\n", free_time);

    fclose(fout);

    return 0;
}

static void run_tests(void) {
    for (int32_t test_idx = 1; test_idx <= 10; ++test_idx) {
        char res_filename[MAX_FILE_NAME];
        char in_filename[MAX_FILE_NAME];
        char out_filename[MAX_FILE_NAME];
        
        snprintf(in_filename, MAX_FILE_NAME, "in/test%d.in", test_idx);
        snprintf(out_filename, MAX_FILE_NAME, "out/test%d.out4", test_idx);
        snprintf(res_filename, MAX_FILE_NAME, "res/hash_table_nosize/res%d.out", test_idx);

        char title[MAX_TITLE_NAME];
        snprintf(title, MAX_TITLE_NAME, "Test %d", test_idx);
        
        printf("%s %s ", title, padding + strlen(title));
        uint8_t err = run_test(in_filename, out_filename, res_filename);

        if (err == 1) {
            printf("Error\n");
        } else {
            printf("Done\n");
        }
    }
}

int main(void) {
    run_tests();

    printf("Hash Table with Unknown Size Test Solved\n\n");

    return 0;
}