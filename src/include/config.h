/**
 * @file config.h
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

#ifndef _CONFIG_UTILS_H_
#define _CONFIG_UTILS_H_

#include <stdio.h>
#include <stdint.h>

/**
 * @brief Definition of error table handler
 * 
 */
typedef enum error_s {
    SCL_OK                                      = 0,

    SCL_INVALID_INPUT                           = -1,
    SCL_INVALID_DATA                            = -2,
    
    SCL_CANNOT_SWAP_DATA                        = -3,
    SCL_SWAP_SAME_DATA                          = -4,
    SCL_CANNOT_CHANGE_DATA                      = -5,
    SCL_UNDEFINED_DATA                          = -6,
    SCL_DATA_NOT_FOUND_FOR_DELETE               = -7,
    SCL_DATA_NOT_FOUND                          = -8,

    SCL_NULL_RBK                                = -23,
    SCL_UNKNOWN_RBK_COLOR                       = -24,
    SCL_NULL_AVL                                = -25,
    SCL_FIXING_NULL_TREE_NODE                   = -26,

    SCL_NOT_ENOUGHT_MEM_FOR_NODE                = -28,
    SCL_NOT_ENOUGHT_MEM_FOR_OBJ                 = -29,

    SCL_DELETE_FROM_EMPTY_OBJECT                = -30,
    
    SCL_INDEX_OVERFLOWS_SIZE                    = -31,

    SCL_NULL_HASH_TABLE                         = -36,
    SCL_UNKNOWN_HASH_NODE_COLOR                 = -37,
    SCL_INVALID_KEY                             = -38,
    SCL_NULL_HASH_ROOTS                         = -39,
    SCL_NULL_HASH_FUNCTION                      = -40,
    SCL_REHASHING_FAILED                        = -41,
} error_t;

/**
 * @brief Definition of the most used functions
 * 
 */
typedef         size_t          (*hash_func)            (int32_t);

void                            scl_error_message       (error_t error_message);

#endif /* _CONFIG_UTILS_H_ */
