/**
 * @file avl_tree.c
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

#include "./include/avl_tree.h"

/**
 * @brief Max function directive for countable numbers
 * of the same type
 * 
 */
#define _MAX(A, B) (((A) >= (B))?(A):(B))

avl_tree_t* create_avl(void) {
    avl_tree_t *new_tree = malloc(sizeof(*new_tree));

    if (NULL != new_tree) {
        new_tree->nil = malloc(sizeof(*new_tree->nil));

        if (NULL != new_tree->nil) {
            new_tree->nil->data = INT32_MIN;
            new_tree->nil->count = 1;
            new_tree->nil->height = 0;
            new_tree->nil->left = new_tree->nil->right = new_tree->nil;
            new_tree->nil->parent = new_tree->nil;
        } else {
            errno = ENOMEM;
            perror("Not enough memory for nil avl-black allocation");
        }

        new_tree->root = new_tree->nil;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for avl allocation");
    }
    
    return new_tree;
}

static avl_tree_node_t* create_avl_node(const avl_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        errno = EINVAL;
        perror("Invalid avl-black tree object");

        return NULL;
    }

    avl_tree_node_t *new_node = malloc(sizeof(*new_node));

    if (NULL != new_node) {
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;
        new_node->height = 1;
        new_node->data = data;
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node avl allocation");
    }

    return new_node;
}

static void free_avl_helper(const avl_tree_t * const __restrict__ tree, avl_tree_node_t ** const __restrict__ root) {
    if (tree->nil == *root) {
        return;
    }

    free_avl_helper(tree, &(*root)->left);
    free_avl_helper(tree, &(*root)->right);

    if (tree->nil != *root) {
        free(*root);

        *root = tree->nil;
    }
}

error_t free_avl(avl_tree_t * const __restrict__ tree) {
    if (NULL != tree) {
        free_avl_helper(tree, &tree->root);
        
        free(tree->nil);
        tree->nil = NULL;        
        free(tree);

        return SCL_OK;
    }

    return SCL_NULL_AVL;
}

static void avl_update_node_height(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * const __restrict__ fix_node) {
    if (tree->nil != fix_node) {
        fix_node->height = _MAX(fix_node->left->height, fix_node->right->height) + 1;
    }
}

static void avl_rotate_left(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const fix_node) {
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    if (tree->nil == fix_node->right) {
        return;
    }

    avl_tree_node_t * const rotate_node = fix_node->right;

    fix_node->right = rotate_node->left;

    if (tree->nil != rotate_node->left) {
        rotate_node->left->parent = fix_node;
    }

    rotate_node->left = fix_node;
    rotate_node->parent = fix_node->parent;
    fix_node->parent = rotate_node;

    if (tree->nil != rotate_node->parent) {
        if (rotate_node->data > rotate_node->parent->data) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        tree->root = rotate_node;
    }

    avl_update_node_height(tree, fix_node);
    avl_update_node_height(tree, rotate_node);
}

static void avl_rotate_right(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const fix_node) {
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    if (tree->nil == fix_node->left) {
        return;
    }

    avl_tree_node_t * const rotate_node = fix_node->left;

    fix_node->left = rotate_node->right;

    if (tree->nil != rotate_node->right) {
        rotate_node->right->parent = fix_node;
    }

    rotate_node->right = fix_node;
    rotate_node->parent = fix_node->parent;
    fix_node->parent = rotate_node;

    if (tree->nil != rotate_node->parent) {
        if (rotate_node->data > rotate_node->parent->data) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        tree->root = rotate_node;
    }

    avl_update_node_height(tree, fix_node);
    avl_update_node_height(tree, rotate_node);
}

static int32_t avl_get_node_balance(const avl_tree_node_t * const __restrict__ fix_node) {
    return (fix_node->left->height - fix_node->right->height);
}

static error_t avl_insert_fix_node_up(avl_tree_t * const __restrict__ tree, avl_tree_node_t *fix_node) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    while (tree->nil != fix_node) {
        avl_update_node_height(tree, fix_node);

        int32_t avl_node_balance_factor = avl_get_node_balance(fix_node);
        int32_t avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int32_t avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        if ((2 == avl_node_balance_factor) && (1 == avl_node_left_balance_factor)) {
            avl_rotate_right(tree, fix_node);
        }

        if ((-2 == avl_node_balance_factor) && (-1 == avl_node_right_balance_factor)) {
            avl_rotate_left(tree, fix_node);
        }

        if ((2 == avl_node_balance_factor) && (-1 == avl_node_left_balance_factor)) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        if ((-2 == avl_node_balance_factor) && (1 == avl_node_right_balance_factor)) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        fix_node = fix_node->parent;
    }

    return SCL_OK;
}

error_t avl_insert(avl_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    avl_tree_node_t *iterator = tree->root;
    avl_tree_node_t *parent_iterator = tree->nil;

    while (tree->nil != iterator) {
        parent_iterator = iterator;

        if (iterator->data > data) {
            iterator = iterator->left;
        } else if (iterator->data < data) {
            iterator = iterator->right;
        } else {
            ++(iterator->count);
            return SCL_OK;
        }
    }

    avl_tree_node_t *new_node = create_avl_node(tree, data);

    if (tree->nil == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
        
    error_t err = SCL_OK;

    if (tree->nil != parent_iterator) {
        new_node->parent = parent_iterator;

        if (parent_iterator->data > new_node->data) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        err = avl_insert_fix_node_up(tree, parent_iterator);
    } else {
        tree->root = new_node;
    }

    ++(tree->size);

    return err;
}

static avl_tree_node_t* avl_find_node(const avl_tree_t * const __restrict__ tree, int32_t data) {
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    avl_tree_node_t *iterator = tree->root;

    while (tree->nil != iterator) {
        if (iterator->data < data) {
            iterator = iterator->right;
        } else if (iterator->data > data) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    return tree->nil;
}

static error_t avl_swap_nodes(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const __restrict__ dest_node, avl_tree_node_t * __restrict__ const src_node) {
    if ((tree->nil == dest_node) || (tree->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    avl_tree_node_t *temp = dest_node->right;

    dest_node->right = src_node->right;

    if (tree->nil != dest_node->right) {
        dest_node->right->parent = dest_node;
    }

    src_node->right = temp;

    if (tree->nil != src_node->right) {
        src_node->right->parent = src_node;
    }

    temp = dest_node->left;

    dest_node->left = src_node->left;

    if (tree->nil != dest_node->left) {
        dest_node->left->parent = dest_node;
    }

    src_node->left = temp;

    if (tree->nil != src_node->left) {
        src_node->left->parent = src_node;
    }

    temp = dest_node->parent;

    dest_node->parent = src_node->parent;

    if (tree->nil != dest_node->parent) {
        if (dest_node->parent->left == src_node) {
            dest_node->parent->left = dest_node;
        } else {
            dest_node->parent->right = dest_node;
        }
    } else {
        tree->root = dest_node;
    }

    src_node->parent = temp;

    if (tree->nil != src_node->parent) {
        if (src_node->parent->left == dest_node) {
            src_node->parent->left = src_node;
        } else {
            src_node->parent->right = src_node;
        }
    } else {
        tree->root = src_node;
    }

    return SCL_OK;
}

uint8_t is_avl_empty(const avl_tree_t * const __restrict__ tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

int32_t get_avl_root(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return tree->root->data;
}

size_t get_avl_size(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SIZE_MAX;
    }

    return tree->size;
}

uint8_t avl_contains(const avl_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return 0;
    }

    return (tree->nil != avl_find_node(tree, data));
}

static avl_tree_node_t* avl_max_node(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->right) {
            root = root->right;
        }
    }

    return root;
}

static avl_tree_node_t* avl_min_node(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

int32_t avl_max_data(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return avl_max_node(tree, tree->root)->data;
}

int32_t avl_min_data(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return avl_min_node(tree, tree->root)->data;
}

static error_t avl_delete_fix_node_up(avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ fix_node) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    while (tree->nil != fix_node) {
        avl_update_node_height(tree, fix_node);

        int32_t avl_node_balance_factor = avl_get_node_balance(fix_node);
        int32_t avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int32_t avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        if ((avl_node_balance_factor > 1) && (avl_node_left_balance_factor >= 0)) {
            avl_rotate_right(tree, fix_node);
        }

        if ((avl_node_balance_factor < -1) && (avl_node_right_balance_factor <= 0)) {
            avl_rotate_left(tree, fix_node);
        }

        if ((avl_node_balance_factor > 1) && (avl_node_left_balance_factor < 0)) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        if ((avl_node_balance_factor < -1) && (avl_node_right_balance_factor > 0)) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        fix_node = fix_node->parent;
    }

    return SCL_OK;
}

error_t avl_delete(avl_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    avl_tree_node_t *delete_node = avl_find_node(tree, data);

    if (tree->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {
        avl_tree_node_t *delete_successor = avl_min_node(tree, delete_node->right);
                
        error_t err = avl_swap_nodes(tree, delete_node, delete_successor);

        if (SCL_OK != err) {
            return err;
        }
    }

    if (tree->nil != delete_node->left) {
        delete_node->left->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node->left;
            } else {
                delete_node->parent->left = delete_node->left;
            }
        } else {
            tree->root = delete_node->left;
        }
    } else if (tree->nil != delete_node->right) {
        delete_node->right->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node->right;
            } else {
                delete_node->parent->left = delete_node->right;
            }
        } else {
            tree->root = delete_node->right;
        }
    } else {
        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = tree->nil;
            } else {
                delete_node->parent->left = tree->nil;
            }
        } else {
            tree->root = tree->nil;
        }
    }

    avl_tree_node_t *parent_delete_node = delete_node->parent;

    if (tree->nil != delete_node) {
        free(delete_node);
    }

    delete_node = tree->nil;

    --(tree->size);
    
    if (tree->nil == parent_delete_node) {
        return SCL_OK;
    }

    return avl_delete_fix_node_up(tree, parent_delete_node);
}

error_t avl_modify_data(avl_tree_t * const __restrict__ tree, int32_t old_data, int32_t new_data) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    error_t err = avl_delete(tree, old_data);

    if (SCL_OK != err) {
        return err;
    }

    err = avl_insert(tree, new_data);

    return err;
}

error_t avl_delete_max(avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    avl_tree_node_t *max_node = avl_max_node(tree, tree->root);

    error_t err = avl_delete(tree, max_node->data);

    return err;
}

error_t avl_delete_min(avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    avl_tree_node_t *min_node = avl_min_node(tree, tree->root);

    error_t err = avl_delete(tree, min_node->data);

    return err;
}

static void avl_traverse_inorder_helper(const avl_tree_t * const __restrict__ tree, const avl_tree_node_t * const __restrict__ root) {
    if (tree->nil == root) {
        return;
    }

    avl_traverse_inorder_helper(tree, root->left);
    printf("%d ", root->data);
    avl_traverse_inorder_helper(tree, root->right);
}

error_t avl_traverse_inorder(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == tree->root) {
        printf("(Null)\n");
    }
    else {
        avl_traverse_inorder_helper(tree, tree->root);
    }

    return SCL_OK;
}
