/**
 * @file avl_tree.h
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

#ifndef AVLTREE_UTILS_H_
#define AVLTREE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "config.h"

/**
 * @brief Adelson-Velsky-Landis Tree Node object definition
 * 
 */
typedef struct avl_tree_node_s {
    int32_t data;                                               /* Pointer to data */
    struct avl_tree_node_s *parent;                             /* Pointer to parent node */
    struct avl_tree_node_s *left;                               /* Pointer to left child node */
    struct avl_tree_node_s *right;                              /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    uint32_t height;                                            /* Height of a node */
} avl_tree_node_t;

/**
 * @brief Adelson-Velsky-Landis Tree object definition
 * 
 */
typedef struct avl_tree_s {
    avl_tree_node_t *root;                                      /* Pointer to tree root */
    avl_tree_node_t *nil;                                       /* Black hole pointer */
    size_t size;                                                /* Size of the avl tree */
} avl_tree_t;

avl_tree_t*             create_avl                          (void);
error_t                 free_avl                            (avl_tree_t * const __restrict__ tree);

error_t                 avl_insert                          (avl_tree_t * const __restrict__ tree, int32_t data);
error_t                 avl_delete                          (avl_tree_t * const __restrict__ tree, int32_t data);
error_t                 avl_modify_data                     (avl_tree_t * const __restrict__ tree, int32_t old_data, int32_t new_data);

uint8_t                 is_avl_empty                        (const avl_tree_t * const __restrict__ tree);
int32_t                 get_avl_root                        (const avl_tree_t * const __restrict__ tree);
size_t                  get_avl_size                        (const avl_tree_t * const __restrict__ tree);

int32_t                 avl_max_data                        (const avl_tree_t * const __restrict__ tree);
int32_t                 avl_min_data                        (const avl_tree_t * const __restrict__ tree);
error_t                 avl_delete_max                      (avl_tree_t * const __restrict__ tree);
error_t                 avl_delete_min                      (avl_tree_t * const __restrict__ tree);

error_t                 avl_traverse_inorder                (const avl_tree_t * const __restrict__ tree);

#endif /* AVLTREE_UTILS_H_ */