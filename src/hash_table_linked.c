/**
 * @file hash_table_linked.c
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

#include "./include/hash_table_linked.h"

#define DEFAULT_HASH_CAPACITY 50
#define DEFAULT_HASH_LOAD_FACTOR 0.75
#define DEFAULT_HASH_CAPACITY_RATIO 2


hash_table_linked_t* create_hash_table(size_t init_capacity, hash_func hash) {
    if (NULL == hash) {
        errno = EINVAL;
        perror("hash function undefined in hash_table");
        return NULL;
    }

    if (10 >= init_capacity) {
        init_capacity = DEFAULT_HASH_CAPACITY;
    }

    hash_table_linked_t *new_hash_table = malloc(sizeof(*new_hash_table));

    if (NULL != new_hash_table) {
        new_hash_table->hash = hash;
        new_hash_table->capacity = init_capacity;
        new_hash_table->size = 0;

        new_hash_table->nil = malloc(sizeof(*new_hash_table->nil));

        if (NULL != new_hash_table->nil) {
            new_hash_table->nil->key = INT32_MIN;
            new_hash_table->nil->color = HASH_BLACK;
            new_hash_table->nil->count = 1;
            new_hash_table->nil->left = new_hash_table->nil->right = new_hash_table->nil;
            new_hash_table->nil->parent = new_hash_table->nil;

            new_hash_table->buckets = malloc(sizeof(*new_hash_table->buckets) * init_capacity);

            if (NULL != new_hash_table->buckets) {
                for (size_t iter = 0; iter < init_capacity; ++iter) {
                    new_hash_table->buckets[iter] = new_hash_table->nil;
                }
            } else {
                free(new_hash_table->nil);
                free(new_hash_table);
                new_hash_table = NULL;

                errno = ENOMEM;
                perror("Not enough memory for buckets of hash table");
            }
        } else {
            free(new_hash_table);
            new_hash_table = NULL;

            errno = ENOMEM;
            perror("Not enough memory for nil hash table allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for hash table allocation");
    }

    return new_hash_table;
}

static void free_hash_table_node(const hash_table_linked_t * const __restrict__ ht, hash_table_linked_node_t ** const __restrict__ free_node) {
    if ((NULL != ht) && (NULL != free_node) && (ht->nil != *free_node)) {
        if (ht->nil != *free_node) {
            free(*free_node);
        }

        *free_node = ht->nil;
    }
}

static void free_hash_table_helper(const hash_table_linked_t * const __restrict__ ht, hash_table_linked_node_t ** const __restrict__ bucket) {
    if ((NULL == bucket) || (ht->nil == *bucket)) {
        return;
    }

    free_hash_table_helper(ht, &(*bucket)->left);
    free_hash_table_helper(ht, &(*bucket)->right);
    free_hash_table_node(ht, bucket);
}

error_t free_hash_table(hash_table_linked_t * const __restrict__ ht) {
    if (NULL != ht) {
        if (NULL != ht->buckets) {
            for (size_t iter = 0; iter < ht->capacity; ++iter) {
                free_hash_table_helper(ht, &ht->buckets[iter]);
            }

            free(ht->nil);
            ht->nil = NULL;

            free(ht->buckets);
            ht->buckets = NULL;
        }

        free(ht);

        return SCL_OK;
    }

    return SCL_NULL_HASH_TABLE;
}

static hash_table_linked_node_t* create_hash_table_node(const hash_table_linked_t * const __restrict__ ht, int32_t key) {
    hash_table_linked_node_t *new_node = malloc(sizeof(*new_node));

    if (NULL != new_node) {
        new_node->left = new_node->right = ht->nil;
        new_node->parent = ht->nil;
        new_node->count = 1;
        new_node->color = HASH_RED;
        new_node->key = key;
    } else {
        new_node = ht->nil;

        errno = ENOMEM;
        perror("Not enough memory for node hash table allocation");
    }

    return new_node;
}

static uint8_t hash_table_need_to_rehash(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return 0;
    }

    return ((1.0 * ht->size) / ht->capacity > DEFAULT_HASH_LOAD_FACTOR);
}

static void hash_table_rotate_left(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index, hash_table_linked_node_t * const fix_node) {
    if ((NULL == ht) || (NULL == ht->buckets) || (ht->nil == fix_node)) {
        return;
    }

    if (ht->nil == fix_node->right) {
        return;
    }

    hash_table_linked_node_t * const rotate_node = fix_node->right;

    fix_node->right = rotate_node->left;

    if (ht->nil != rotate_node->left) {
        rotate_node->left->parent = fix_node;
    }

    rotate_node->left = fix_node;
    rotate_node->parent = fix_node->parent;
    fix_node->parent = rotate_node;

    if (ht->nil != rotate_node->parent) {
        if (rotate_node->key > rotate_node->parent->key) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->buckets[bucket_index] = rotate_node;
    }
}

static void hash_table_rotate_right(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index, hash_table_linked_node_t * const fix_node) {
    if ((NULL == ht) || (NULL == ht->buckets) || (ht->nil == fix_node)) {
        return;
    }

    if (ht->nil == fix_node->left) {
        return;
    }

    hash_table_linked_node_t * const rotate_node = fix_node->left;

    fix_node->left = rotate_node->right;

    if (ht->nil != rotate_node->right) {
        rotate_node->right->parent = fix_node;
    }

    rotate_node->right = fix_node;
    rotate_node->parent = fix_node->parent;
    fix_node->parent = rotate_node;

    if (ht->nil != rotate_node->parent) {
        if (rotate_node->key > rotate_node->parent->key) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->buckets[bucket_index] = rotate_node;
    }
}

static error_t hash_table_insert_fix_node_up(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index, hash_table_linked_node_t *fix_node) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (ht->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    hash_table_linked_node_t *parent_fix_node = ht->nil;

    while ((ht->buckets[bucket_index] != fix_node) && (HASH_BLACK != fix_node->color) && (HASH_BLACK != fix_node->parent->color)) {
        parent_fix_node = fix_node->parent;
        hash_table_linked_node_t *brother_node = ht->nil;

        if (parent_fix_node->parent->left == parent_fix_node) {
            brother_node = parent_fix_node->parent->right;
        } else {
            brother_node = parent_fix_node->parent->left;
        }

        if (HASH_BLACK == brother_node->color) {
            if (parent_fix_node->left == fix_node) {
                if (parent_fix_node->parent->left == parent_fix_node) {
                    parent_fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    hash_table_rotate_right(ht, bucket_index, parent_fix_node->parent);

                    fix_node = parent_fix_node;
                } else {
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;

                    hash_table_rotate_right(ht, bucket_index, parent_fix_node);
                    hash_table_rotate_left(ht, bucket_index, fix_node->parent);
                }
            } else {
                if (parent_fix_node->parent->left == parent_fix_node) {
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    hash_table_rotate_left(ht, bucket_index, parent_fix_node);
                    hash_table_rotate_right(ht, bucket_index, fix_node->parent);
                } else {
                    parent_fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;

                    hash_table_rotate_left(ht, bucket_index, parent_fix_node->parent);

                    fix_node = parent_fix_node;
                }
            }
        } else if (HASH_RED == brother_node->color) {
            parent_fix_node->parent->color = HASH_RED;
            brother_node->color = HASH_BLACK;
            parent_fix_node->color = HASH_BLACK;

            fix_node = parent_fix_node->parent;
        } else {
            return SCL_UNKNOWN_HASH_NODE_COLOR;
        }
    }

    ht->buckets[bucket_index]->color = HASH_BLACK;

    return SCL_OK;
}

static error_t hash_table_rehash(hash_table_linked_t * const __restrict__ ht);

error_t hash_table_insert(hash_table_linked_t * const __restrict__ ht, int32_t key) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    size_t bucket_index = ht->hash(key) % ht->capacity;

    hash_table_linked_node_t *iterator = ht->buckets[bucket_index];
    hash_table_linked_node_t *parent_iterator = ht->nil;

    while (ht->nil != iterator) {
        parent_iterator = iterator;

        if (iterator->key > key) {
            iterator = iterator->left;
        } else if (iterator->key < key) {
            iterator = iterator->right;
        } else {
            ++(iterator->count);
            return SCL_OK;
        }
    }

    hash_table_linked_node_t *new_node = create_hash_table_node(ht, key);

    if (ht->nil == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
    
    error_t err = SCL_OK;

    if (ht->nil != parent_iterator) {
        new_node->parent = parent_iterator;

        if (parent_iterator->key > new_node->key) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        err = hash_table_insert_fix_node_up(ht, bucket_index, new_node);
    } else {
        ht->buckets[bucket_index] = new_node;
        new_node->color = HASH_BLACK;
    }

    ++(ht->size);

    if (1 == hash_table_need_to_rehash(ht)) {
        return hash_table_rehash(ht);
    }

    return err;
}

static void hash_table_rehash_helper(hash_table_linked_t * const __restrict__ ht, const hash_table_linked_node_t * const __restrict__ bucket) {
    if (ht->nil == bucket) {
        return;
    }

    hash_table_insert(ht, bucket->key);

    hash_table_rehash_helper(ht, bucket->left);
    hash_table_rehash_helper(ht, bucket->right);
}

static error_t hash_table_rehash(hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == ht->hash) {
        return SCL_NULL_HASH_FUNCTION;
    }

    size_t old_capacity = ht->capacity;
    ht->capacity *= DEFAULT_HASH_CAPACITY_RATIO;

    hash_table_linked_node_t **new_buckets = malloc(sizeof(*new_buckets) * ht->capacity);

    if (NULL == new_buckets) {
        ht->capacity = old_capacity;

        errno = ENOMEM;
        perror("Not enough memory for buckets of hash table");

        return SCL_REHASHING_FAILED;
    }

    hash_table_linked_node_t **old_buckets = ht->buckets;
    ht->buckets = new_buckets;

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        new_buckets[iter] = ht->nil;
    }

    ht->size = 0;

    for (size_t iter = 0; iter < old_capacity; ++iter) {
        if (ht->nil != old_buckets[iter]) {
            hash_table_rehash_helper(ht, old_buckets[iter]);
            free_hash_table_helper(ht, &old_buckets[iter]);
        }
    }

    free(old_buckets);
    old_buckets = NULL;

    return SCL_OK;
}

static hash_table_linked_node_t* hash_table_find_node(const hash_table_linked_t * const __restrict__ ht, int32_t key) {
    if ((NULL == ht) || (NULL == ht->buckets)) {
        return ht->nil;
    }

    size_t bucket_index = ht->hash(key) % ht->capacity;

    hash_table_linked_node_t *iterator = ht->buckets[bucket_index];

    while (ht->nil != iterator) {
        if (iterator->key < key) {
            iterator = iterator->right;
        } else if (iterator->key > key) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    return ht->nil;
}

error_t hash_table_modify(hash_table_linked_t * const __restrict__ ht, int32_t old_key, int32_t new_key) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == ht->hash) {
        return SCL_NULL_HASH_FUNCTION;
    }

    error_t err = hash_table_delete(ht, old_key);

    if (SCL_OK != err) {
        return err;
    }

    return hash_table_insert(ht, new_key);
}

uint8_t hash_table_contains(const hash_table_linked_t * const __restrict__ ht, int32_t key) {
    if (ht->nil == hash_table_find_node(ht, key)) {
        return 0;
    }

    return 1;
}

uint8_t is_hash_table_empty(const hash_table_linked_t * const __restrict__ ht) {
    if ((NULL == ht) || (NULL == ht->buckets) || (0 == ht->capacity) || (0 == ht->size)) {
        return 1;
    }

    return 0;
}

size_t get_hash_table_size(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->size;
}

size_t get_hash_table_capacity(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->capacity;
}

static hash_table_linked_node_t* hash_table_root_min_node(const hash_table_linked_t * const __restrict__ ht, hash_table_linked_node_t * __restrict__ bucket) {
    if (ht->nil != bucket) {
        while (ht->nil != bucket->left) {
            bucket = bucket->left;
        }
    }

    return bucket;
}

static hash_table_linked_node_t* hash_table_root_max_node(const hash_table_linked_t * const __restrict__ ht, hash_table_linked_node_t * __restrict__ bucket) {
    if (ht->nil != bucket) {
        while (ht->nil != bucket->right) {
            bucket = bucket->right;
        }
    }

    return bucket;
}

int32_t hash_table_get_max(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return INT32_MIN;
    }

    if (NULL == ht->buckets) {
        return INT32_MIN;
    }

    if (0 == ht->size) {
        return INT32_MIN;
    }

    hash_table_linked_node_t *max_node = ht->nil;

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        if (ht->nil != ht->buckets[iter]) {
            hash_table_linked_node_t *temp = hash_table_root_max_node(ht, ht->buckets[iter]);

            if (temp->key > max_node->key) {
                max_node = temp;
            }
        }
    }

    return max_node->key;
}

int32_t hash_table_get_min(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return INT32_MAX;
    }

    if (NULL == ht->buckets) {
        return INT32_MAX;
    }

    if (0 == ht->size) {
        return INT32_MAX;
    }

    hash_table_linked_node_t *min_node = hash_table_root_min_node(ht, ht->buckets[0]);

    for (size_t iter = 1; iter < ht->capacity; ++iter) {
        if (ht->nil != ht->buckets[iter]) {
            hash_table_linked_node_t *temp = hash_table_root_min_node(ht, ht->buckets[iter]);

            if (temp->key < min_node->key) {
                min_node = temp;
            }
        }
    }

    return min_node->key;
}

error_t hash_table_delete_max(hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (0 == ht->size) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    int32_t max_key = hash_table_get_max(ht);

    return hash_table_delete(ht, max_key);
}

error_t hash_table_delete_min(hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (0 == ht->size) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    int32_t min_key = hash_table_get_min(ht);

    return hash_table_delete(ht, min_key);
}

static error_t hash_table_swap_nodes(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index, hash_table_linked_node_t * __restrict__ const dest_node, hash_table_linked_node_t * __restrict__ const src_node) {
    if ((ht->nil == dest_node) || (ht->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    hash_table_linked_node_color_t temp_color = dest_node->color;
    dest_node->color = src_node->color;
    src_node->color = temp_color;

    hash_table_linked_node_t *temp = dest_node->right;

    dest_node->right = src_node->right;

    if (ht->nil != dest_node->right) {
        dest_node->right->parent = dest_node;
    }

    src_node->right = temp;

    if (ht->nil != src_node->right) {
        src_node->right->parent = src_node;
    }

    temp = dest_node->left;

    dest_node->left = src_node->left;

    if (ht->nil != dest_node->left) {
        dest_node->left->parent = dest_node;
    }

    src_node->left = temp;

    if (ht->nil != src_node->left) {
        src_node->left->parent = src_node;
    }

    temp = dest_node->parent;

    dest_node->parent = src_node->parent;

    if (ht->nil != dest_node->parent) {
        if (dest_node->parent->left == src_node) {
            dest_node->parent->left = dest_node;
        } else {
            dest_node->parent->right = dest_node;
        }
    } else {
        ht->buckets[bucket_index] = dest_node;
    }

    src_node->parent = temp;

    if (ht->nil != src_node->parent) {
        if (src_node->parent->left == dest_node) {
            src_node->parent->left = src_node;
        } else {
            src_node->parent->right = src_node;
        }
    } else {
        ht->buckets[bucket_index] = src_node;
    }

    return SCL_OK;
}

static error_t hash_table_delete_fix_node_up(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index, hash_table_linked_node_t *fix_node, hash_table_linked_node_t *parent_fix_node) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (ht->nil == parent_fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    hash_table_linked_node_t *brother_node = ht->nil;

    while ((ht->buckets[bucket_index] != fix_node) && (HASH_BLACK == fix_node->color)) {
        if (parent_fix_node->left == fix_node) {
            brother_node = parent_fix_node->right;

            if (HASH_RED == brother_node->color) {
                brother_node->color = HASH_BLACK;
                parent_fix_node->color = HASH_RED;

                hash_table_rotate_left(ht, bucket_index, parent_fix_node);

                brother_node = parent_fix_node->right;
            }

            if ((HASH_BLACK == brother_node->left->color) && (HASH_BLACK == brother_node->right->color)) {
                brother_node->color = HASH_RED;
                fix_node = parent_fix_node;
            } else {
                if (HASH_BLACK == brother_node->right->color) {
                    brother_node->left->color = HASH_BLACK;
                    brother_node->color = HASH_RED;

                    hash_table_rotate_right(ht, bucket_index, brother_node);
                    
                    brother_node = parent_fix_node->right;
                }

                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->right->color = HASH_BLACK;

                hash_table_rotate_left(ht, bucket_index, parent_fix_node);

                fix_node = ht->buckets[bucket_index];
            }
        } else {
            brother_node = parent_fix_node->left;

            if (HASH_RED == brother_node->color) {
                brother_node->color = HASH_BLACK;
                parent_fix_node->color = HASH_RED;

                hash_table_rotate_right(ht, bucket_index, parent_fix_node);

                brother_node = parent_fix_node->left;
            }

            if ((HASH_BLACK == brother_node->right->color) && (HASH_BLACK == brother_node->left->color)) {
                brother_node->color = HASH_RED;
                fix_node = parent_fix_node;
            } else {
                if (HASH_BLACK == brother_node->left->color) {
                    brother_node->right->color = HASH_BLACK;
                    brother_node->color = HASH_RED;

                    hash_table_rotate_left(ht, bucket_index, brother_node);

                    brother_node = parent_fix_node->left;
                }

                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->left->color = HASH_BLACK;

                hash_table_rotate_right(ht, bucket_index, parent_fix_node);

                fix_node = ht->buckets[bucket_index];
            }
        }
    }

    fix_node->color = HASH_BLACK;

    return SCL_OK;
}

error_t hash_table_delete(hash_table_linked_t * __restrict__ const ht, int32_t key) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    size_t bucket_index = ht->hash(key) % ht->capacity;
    hash_table_linked_node_t *delete_node = hash_table_find_node(ht, key);

    if (ht->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    if ((ht->nil != delete_node->left) && (ht->nil != delete_node->right)) {
        hash_table_linked_node_t *delete_succecessor = hash_table_root_min_node(ht, delete_node->right);
                
        error_t err = hash_table_swap_nodes(ht, bucket_index, delete_node, delete_succecessor);

        if (SCL_OK != err) {
            return err;
        }
    }

    uint8_t need_fixing_tree = 1;
    hash_table_linked_node_t *delete_node_child = ht->nil;

    if (ht->nil != delete_node->left) {
        delete_node_child = delete_node->left;

        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;
            delete_node_child->color = HASH_BLACK;
        }

        delete_node_child->parent = delete_node->parent;

        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {            
            ht->buckets[bucket_index] = delete_node_child;
        }
    } else if (ht->nil != delete_node->right) {
        delete_node_child = delete_node->right;

        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;
            delete_node_child->color = HASH_BLACK;
        }

        delete_node_child->parent = delete_node->parent;

        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {
            ht->buckets[bucket_index] = delete_node_child;
        }
    } else {
        if (HASH_RED == delete_node->color) {
            need_fixing_tree = 0;
        }

        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = ht->nil;
            } else {
                delete_node->parent->left = ht->nil;
            }
        } else {
            ht->buckets[bucket_index] = ht->nil;
        }
    }

    hash_table_linked_node_t *parent_delete_node = delete_node->parent;

    free_hash_table_node(ht, &delete_node);

    --(ht->size);

    if (0 != need_fixing_tree) {
        return hash_table_delete_fix_node_up(ht, bucket_index, delete_node_child, parent_delete_node);
    }

    return SCL_OK;
}

static void hash_table_bucket_traverse_inorder_helper(const hash_table_linked_t * const __restrict__ ht, const hash_table_linked_node_t * const __restrict__ bucket) {
    if (ht->nil == bucket) {
        return;
    }

    hash_table_bucket_traverse_inorder_helper(ht, bucket->left);
    printf("%d ", bucket->key);
    hash_table_bucket_traverse_inorder_helper(ht, bucket->right);
}

error_t hash_table_bucket_traverse_inorder(const hash_table_linked_t * const __restrict__ ht, size_t bucket_index) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    bucket_index = bucket_index % ht->capacity;

    if (ht->nil != ht->buckets[bucket_index]) {
        hash_table_bucket_traverse_inorder_helper(ht, ht->buckets[bucket_index]);
    }

    return SCL_OK;
}

error_t hash_table_traverse_inorder(const hash_table_linked_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        error_t err = hash_table_bucket_traverse_inorder(ht, iter);
        
        if (SCL_OK != err) {
            return err;
        }
    }

    return SCL_OK;
}