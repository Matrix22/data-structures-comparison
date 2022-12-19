##########################################################################################
 # @file Makefile
 # @author Mihai Negru (determinant289@gmail.com)
 # @version 1.0.0
 # @date 2022-12-18
 # 
 # @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 # This file is part of data-structures-comparison.
 #
 # data-structures-comparison is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # data-structures-comparison is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 # GNU Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public License
 # along with data-structures-comparison.  If not, see <http://www.gnu.org/licenses/>.
 # 
###########################################################################################

# Compiling info
CC 						:=		gcc
CFLAGS 					:= 		-g -Wall -Wextra -Wpedantic \
								-Wformat=2 -Wno-unused-parameter \
								-Wshadow -Wwrite-strings -Wstrict-prototypes \
								-Wold-style-definition -Wredundant-decls \
								-Wnested-externs -Wmissing-include-dirs \
								-Wjump-misses-init -Wlogical-op

# Source files info
SRC_PATH 				:= 		./src

# Executable files info
EXEC_PATH 				:= 		./exec

# Libraries files info
LIBS_PATH 				:= 		../libs

# Static Library info
STATIC_LIB_NAME 		:= 		libsetsalgos.a

# Maximum Padding String
PADDING 				:= 		............................................................

all: $(EXEC_PATH) $(EXEC_PATH)/avl_test $(EXEC_PATH)/rbk_test $(EXEC_PATH)/hash_table_test $(EXEC_PATH)/hash_table_nosize
	@printf "Finished building tests "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n\n" " PASSED"

$(EXEC_PATH):
	@mkdir -p $(EXEC_PATH)

$(EXEC_PATH)/avl_test: $(SRC_PATH)/avl_test.c
	@$(CC) $(CFLAGS) $< -o $@ $(LIBS_PATH)/$(STATIC_LIB_NAME)
	@printf "Building AVL Tree test "
	@printf "%0.31s" $(PADDING)
	@printf "%s\n\n" " PASSED"

$(EXEC_PATH)/rbk_test: $(SRC_PATH)/rbk_test.c
	@$(CC) $(CFLAGS) $< -o $@ $(LIBS_PATH)/$(STATIC_LIB_NAME)
	@printf "Building Red-Black Tree test "
	@printf "%0.25s" $(PADDING)
	@printf "%s\n\n" " PASSED"

$(EXEC_PATH)/hash_table_test: $(SRC_PATH)/hash_table_test.c
	@$(CC) $(CFLAGS) $< -o $@ $(LIBS_PATH)/$(STATIC_LIB_NAME)
	@printf "Building Hash Table test "
	@printf "%0.29s" $(PADDING)
	@printf "%s\n\n" " PASSED"

$(EXEC_PATH)/hash_table_nosize: $(SRC_PATH)/hash_table_test_nosize.c
	@$(CC) $(CFLAGS) $< -o $@ $(LIBS_PATH)/$(STATIC_LIB_NAME)
	@printf "Building Hash Table No Size test "
	@printf "%0.21s" $(PADDING)
	@printf "%s\n\n" " PASSED"

clean:
	@rm -rf $(EXEC_PATH)
