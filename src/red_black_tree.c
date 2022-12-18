/**
 * @file red_black_tree.c
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

#include "./include/red_black_tree.h"

rbk_tree_t* create_rbk() {
    rbk_tree_t *new_tree = malloc(sizeof(*new_tree));

    if (NULL != new_tree) {
        new_tree->nil = malloc(sizeof(*new_tree->nil));

        if (NULL != new_tree->nil) {
            new_tree->nil->data = INT32_MIN;
            new_tree->nil->color = BLACK;
            new_tree->nil->count = 1;
            new_tree->nil->left = new_tree->nil->right = new_tree->nil;
            new_tree->nil->parent = new_tree->nil;
        } else {
            errno = ENOMEM;
            perror("Not enough memory for nil red-black allocation");
        }

        new_tree->root = new_tree->nil;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for red-black allocation");
    }
    
    return new_tree;
}

static rbk_tree_node_t* create_rbk_node(const rbk_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        errno = EINVAL;
        perror("Invalid red-black tree object");

        return NULL;
    }

    rbk_tree_node_t *new_node = malloc(sizeof(*new_node));

    if (NULL != new_node) {
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;
        new_node->color = RED;
        new_node->data = data;
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node red-black allocation");
    }

    return new_node;
}

static void free_rbk_helper(const rbk_tree_t * const __restrict__ tree, rbk_tree_node_t ** const __restrict__ root) {
    if (tree->nil == *root) {
        return;
    }

    free_rbk_helper(tree, &(*root)->left);
    free_rbk_helper(tree, &(*root)->right);

    if (tree->nil != *root) {
        free(*root);

        *root = tree->nil;
    }
}

error_t free_rbk(rbk_tree_t * const __restrict__ tree) {
    if (NULL != tree) {
        free_rbk_helper(tree, &tree->root);
        
        free(tree->nil);
        tree->nil = NULL;
        free(tree);

        return SCL_OK;
    }

    return SCL_NULL_RBK;
}

static void rbk_rotate_left(rbk_tree_t * const __restrict__ tree, rbk_tree_node_t * const fix_node) {
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    if (tree->nil == fix_node->right) {
        return;
    }

    rbk_tree_node_t * const rotate_node = fix_node->right;

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
}

static void rbk_rotate_right(rbk_tree_t * const __restrict__ tree, rbk_tree_node_t * const fix_node) {
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    if (tree->nil == fix_node->left) {
        return;
    }

    rbk_tree_node_t * const rotate_node = fix_node->left;

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
}

static error_t rbk_insert_fix_node_up(rbk_tree_t * const __restrict__ tree, rbk_tree_node_t *fix_node) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    rbk_tree_node_t *parent_fix_node = tree->nil;

    while ((tree->root != fix_node) && (BLACK != fix_node->color) && (BLACK != fix_node->parent->color)) {
        parent_fix_node = fix_node->parent;
        rbk_tree_node_t *brother_node = tree->nil;

        if (parent_fix_node->parent->left == parent_fix_node) {
            brother_node = parent_fix_node->parent->right;
        } else {
            brother_node = parent_fix_node->parent->left;
        }

        if (BLACK == brother_node->color) {      
            if (parent_fix_node->left == fix_node) {
                if (parent_fix_node->parent->left == parent_fix_node) {
                    parent_fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    rbk_rotate_right(tree, parent_fix_node->parent);

                    fix_node = parent_fix_node;
                } else {
                    fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;

                    rbk_rotate_right(tree, parent_fix_node);
                    rbk_rotate_left(tree, fix_node->parent);
                }
            } else {
                if (parent_fix_node->parent->left == parent_fix_node) {
                    fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    rbk_rotate_left(tree, parent_fix_node);
                    rbk_rotate_right(tree, fix_node->parent);
                } else {
                    parent_fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    rbk_rotate_left(tree, parent_fix_node->parent);

                    fix_node = parent_fix_node;
                }
            }
        } else if (RED == brother_node->color) {
            parent_fix_node->parent->color = RED;
            brother_node->color = BLACK;
            parent_fix_node->color = BLACK;

            fix_node = parent_fix_node->parent;
        } else {
            return SCL_UNKNOWN_RBK_COLOR;
        }
    }

    tree->root->color = BLACK;

    return SCL_OK;
}

error_t rbk_insert(rbk_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    rbk_tree_node_t *iterator = tree->root;
    rbk_tree_node_t *parent_iterator = tree->nil;

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

    rbk_tree_node_t *new_node = create_rbk_node(tree, data);

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

        err = rbk_insert_fix_node_up(tree, new_node);
    } else {
        tree->root = new_node;
        new_node->color = BLACK;
    }

    ++(tree->size);

    return err;
}

static rbk_tree_node_t* rbk_find_node(const rbk_tree_t * const __restrict__ tree, int32_t data) {
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    rbk_tree_node_t *iterator = tree->root;

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

static error_t rbk_swap_nodes(rbk_tree_t * const __restrict__ tree, rbk_tree_node_t * const __restrict__ dest_node, rbk_tree_node_t * const __restrict__ src_node) {
    if ((tree->nil == dest_node) || (tree->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    rbk_tree_node_color_t temp_color = dest_node->color;
    dest_node->color = src_node->color;
    src_node->color = temp_color;

    rbk_tree_node_t *temp = dest_node->right;

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

uint8_t is_rbk_empty(const rbk_tree_t * const __restrict__ tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

int32_t get_rbk_root(const rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return tree->root->data;
}

size_t get_rbk_size(const rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SIZE_MAX;
    }

    return tree->size;
}

uint8_t rbk_includes(const rbk_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return 0;
    }

    return (tree->nil != rbk_find_node(tree, data));
}

static rbk_tree_node_t* rbk_max_node(const rbk_tree_t * const __restrict__ tree, rbk_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->right) {
            root = root->right;
        }
    }

    return root;
}

static rbk_tree_node_t* rbk_min_node(const rbk_tree_t * const __restrict__ tree, rbk_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

int32_t rbk_max_data(const rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return rbk_max_node(tree, tree->root)->data;
}

int32_t rbk_min_data(const rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return INT32_MIN;
    }

    return rbk_min_node(tree, tree->root)->data;
}

static error_t rbk_delete_fix_node_up(rbk_tree_t * const __restrict__ tree, rbk_tree_node_t *fix_node, rbk_tree_node_t *parent_fix_node) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == parent_fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    rbk_tree_node_t *brother_node = tree->nil;

    while ((tree->root != fix_node) && (BLACK == fix_node->color)) {
        if (parent_fix_node->left == fix_node) {
            brother_node = parent_fix_node->right;

            if (RED == brother_node->color) {
                brother_node->color = BLACK;
                parent_fix_node->color = RED;

                rbk_rotate_left(tree, parent_fix_node);

                brother_node = parent_fix_node->right;
            }

            if ((BLACK == brother_node->left->color) && (BLACK == brother_node->right->color)) {
                brother_node->color = RED;

                fix_node = parent_fix_node;
            } else {
                if (BLACK == brother_node->right->color) {
                    brother_node->left->color = BLACK;
                    brother_node->color = RED;

                    rbk_rotate_right(tree, brother_node);
                    
                    brother_node = parent_fix_node->right;
                }
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = BLACK;
                brother_node->right->color = BLACK;

                rbk_rotate_left(tree, parent_fix_node);

                fix_node = tree->root;
            }
        } else {
            brother_node = parent_fix_node->left;

            if (RED == brother_node->color) {
                brother_node->color = BLACK;
                parent_fix_node->color = RED;

                rbk_rotate_right(tree, parent_fix_node);

                brother_node = parent_fix_node->left;
            }

            if ((BLACK == brother_node->right->color) && (BLACK == brother_node->left->color)) {
                brother_node->color = RED;

                fix_node = parent_fix_node;
            } else {
                if (BLACK == brother_node->left->color) {
                    brother_node->right->color = BLACK;
                    brother_node->color = RED;

                    rbk_rotate_left(tree, brother_node);

                    brother_node = parent_fix_node->left;
                }
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = BLACK;
                brother_node->left->color = BLACK;

                rbk_rotate_right(tree, parent_fix_node);

                fix_node = tree->root;
            }
        }
    }

    fix_node->color = BLACK;

    return SCL_OK;
}

error_t rbk_delete(rbk_tree_t * const __restrict__ tree, int32_t data) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    rbk_tree_node_t *delete_node = rbk_find_node(tree, data);

    if (tree->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {
        rbk_tree_node_t *delete_successor = rbk_min_node(tree, delete_node->right);
                
        error_t err = rbk_swap_nodes(tree, delete_node, delete_successor);

        if (SCL_OK != err) {
            return err;
        }
    }

    uint8_t need_fixing_tree = 1;

    rbk_tree_node_t *delete_node_child = tree->nil;

    if (tree->nil != delete_node->left) {
        delete_node_child = delete_node->left;

        if ((RED == delete_node_child->color) && (BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            delete_node_child->color = BLACK;
        }

        delete_node_child->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {         
            tree->root = delete_node_child;
        }
    } else if (tree->nil != delete_node->right) {
        delete_node_child = delete_node->right;

        if ((RED == delete_node_child->color) && (BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            delete_node_child->color = BLACK;
        }

        delete_node_child->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {
            tree->root = delete_node_child;
        }
    } else {
        if (RED == delete_node->color) {
            need_fixing_tree = 0;
        }

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

    rbk_tree_node_t *parent_delete_node = delete_node->parent;

    if (tree->nil != delete_node) {
        free(delete_node);
    }

    delete_node = tree->nil;

    --(tree->size);

    if ((0 != need_fixing_tree) && (tree->nil != parent_delete_node)) {
        return rbk_delete_fix_node_up(tree, delete_node_child, parent_delete_node);
    }

    return SCL_OK;
}

error_t rbk_modify_data(rbk_tree_t * const __restrict__ tree, int32_t old_data, int32_t new_data) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    error_t err = rbk_delete(tree, old_data);

    if (SCL_OK != err) {
        return err;
    }

    err = rbk_insert(tree, new_data);

    return err;
}

error_t rbk_delete_max(rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    rbk_tree_node_t *max_node = rbk_max_node(tree, tree->root);

    error_t err = rbk_delete(tree, max_node->data);

    return err;
}

error_t rbk_delete_min(rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    rbk_tree_node_t *min_node = rbk_min_node(tree, tree->root);

    error_t err = rbk_delete(tree, min_node->data);

    return err;
}

static void rbk_traverse_inorder_helper(const rbk_tree_t * const __restrict__ tree, const rbk_tree_node_t * const __restrict__ root) {
    if (tree->nil == root) {
        return;
    }

    rbk_traverse_inorder_helper(tree, root->left);
    printf("%d ", root->data);
    rbk_traverse_inorder_helper(tree, root->right);
}

error_t rbk_traverse_inorder(const rbk_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == tree->root) {
        printf("(Null)\n");
    }
    else {
        rbk_traverse_inorder_helper(tree, tree->root);
    }

    return SCL_OK;
}
