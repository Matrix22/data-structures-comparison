/**
 * @file red_black_tree.h
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

#ifndef _RED_BLACK_TREE_UTILS_H_
#define _RED_BLACK_TREE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "config.h"

/**
 * @brief Color of one red-black tree node
 * 
 */
typedef enum rbk_tree_node_color_s {
    RED,                                                        /* Red color of the node */
    BLACK                                                       /* Black color of the node */
} rbk_tree_node_color_t;

/**
 * @brief Red-Black Tree Node object definition
 * 
 */
typedef struct rbk_tree_node_s {
    int32_t data;                                               /* Pointer to data */
    struct rbk_tree_node_s *parent;                             /* Pointer to parent node */
    struct rbk_tree_node_s *left;                               /* Pointer to left child node */
    struct rbk_tree_node_s *right;                              /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    rbk_tree_node_color_t color;                                /* Color of a node */
} rbk_tree_node_t;

/**
 * @brief Red-Black Tree object definition
 * 
 */
typedef struct rbk_tree_s {
    rbk_tree_node_t *root;                                      /* Pointer to tree root */
    rbk_tree_node_t *nil;                                       /* Black hole pointer */
    size_t size;                                                /* Size of the red-black tree */
} rbk_tree_t;

rbk_tree_t*             create_rbk                          (void);
error_t                 free_rbk                            (rbk_tree_t * const __restrict__ tree);

error_t                 rbk_insert                          (rbk_tree_t * const __restrict__ tree, int32_t data);
error_t                 rbk_delete                          (rbk_tree_t * const __restrict__ tree, int32_t data);
error_t                 rbk_modify_data                     (rbk_tree_t * const __restrict__ tree, int32_t old_data, int32_t new_data);

uint8_t                 is_rbk_empty                        (const rbk_tree_t * const __restrict__ tree);
int32_t                 get_rbk_root                        (const rbk_tree_t * const __restrict__ tree);
size_t                  get_rbk_size                        (const rbk_tree_t * const __restrict__ tree);

int32_t                 rbk_max_data                        (const rbk_tree_t * const __restrict__ tree);
int32_t                 rbk_min_data                        (const rbk_tree_t * const __restrict__ tree);
error_t                 rbk_delete_max                      (rbk_tree_t * const __restrict__ tree);
error_t                 rbk_delete_min                      (rbk_tree_t * const __restrict__ tree);

error_t                 rbk_traverse_inorder                (const rbk_tree_t * const __restrict__ tree);

#endif /* _RED_BLACK_TREE_UTILS_H_ */