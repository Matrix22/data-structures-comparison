/**
 * @file hash_table_linked.h
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

#ifndef HASH_TABLE_LINKED_UTILS_H_
#define HASH_TABLE_LINKED_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "config.h"

/**
 * @brief Color of one hash table node
 * 
 */
typedef enum hash_table_linked_node_color_s {
    HASH_RED,                                                   /* Red color of the hash table node */
    HASH_BLACK                                                  /* Black color of the hash table node */
} hash_table_linked_node_color_t;

/**
 * @brief Hash Table Node object definition
 * 
 */
typedef struct hash_table_linked_node_s {
    int32_t key;                                                /* Pointer to a location of a value representing key of the hash */
    struct hash_table_linked_node_s *parent;                    /* Pointer to the parent of the current node */
    struct hash_table_linked_node_s *left;                      /* Pointer to the left child of the current node */
    struct hash_table_linked_node_s *right;                     /* Pointer to the right child of the current node */
    uint32_t count;                                             /* Number of nodes with the same data and key value */
    hash_table_linked_node_color_t color;                       /* Color of the current node */
} hash_table_linked_node_t;

/**
 * @brief Hash Table object definition
 * 
 */
typedef struct hash_table_linked_s {
    hash_table_linked_node_t **buckets;                         /* Array of pointers of red black trees */
    hash_table_linked_node_t *nil;                              /* Black hole pointer once in never out */
    hash_func hash;                                             /* Pointer to a hash function */
    size_t capacity;                                            /* Number of red black trees within the hash table */
    size_t size;                                                /* Number of total nodes from hash table object*/
} hash_table_linked_t;

hash_table_linked_t*    create_hash_table                       (size_t init_capacity, hash_func hash);
error_t                 free_hash_table                         (hash_table_linked_t * const __restrict__ ht);

error_t                 hash_table_insert                       (hash_table_linked_t * const __restrict__ ht, int32_t key);
error_t                 hash_table_delete                       (hash_table_linked_t * const __restrict__ ht, int32_t key);
error_t                 hash_table_modify                       (hash_table_linked_t * const __restrict__ ht, int32_t old_key, int32_t new_key);
uint8_t                 hash_table_contains                     (const hash_table_linked_t * const __restrict__ ht, int32_t key);

uint8_t                 is_hash_table_empty                     (const hash_table_linked_t * const __restrict__ ht);
size_t                  get_hash_table_size                     (const hash_table_linked_t * const __restrict__ ht);
size_t                  get_hash_table_capacity                 (const hash_table_linked_t * const __restrict__ ht);

int32_t                 hash_table_get_max                      (const hash_table_linked_t * const __restrict__ ht);
int32_t                 hash_table_get_min                      (const hash_table_linked_t * const __restrict__ ht);
error_t                 hash_table_delete_max                   (hash_table_linked_t * const __restrict__ ht);
error_t                 hash_table_delete_min                   (hash_table_linked_t * const __restrict__ ht);

error_t                 hash_table_traverse_inorder             (const hash_table_linked_t * const __restrict__ ht);

#endif /* HASH_TABLE_LINKED_UTILS_H_ */