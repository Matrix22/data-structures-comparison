CC 						:=		gcc
CFLAGS 					:= 		-g -Wall -Wextra -Wpedantic \
								-Wformat=2 -Wno-unused-parameter \
								-Wshadow -Wwrite-strings -Wstrict-prototypes \
								-Wold-style-definition -Wredundant-decls \
								-Wnested-externs -Wmissing-include-dirs \
								-Wjump-misses-init -Wlogical-op -O2

# Source files info
SRC_PATH 				:= 		./src
SRC_FILES 				:= 		$(wildcard $(SRC_PATH)/*.c)
EXEC_FILES 				:= 		$(patsubst $(SRC_PATH)/%.c,%,$(SRC_FILES))

# Libraries files info
LIBS_PATH 				:= 		../libs

# Dynamic Library info
DYNAMIC_LIB_NAME 		:= 		setsalgos

# Maximum Padding String
PADDING 				:= 		............................................................

all: avl_test rbk_test hash_table_test
	@printf "Finished building tests "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n\n" " PASSED"

avl_test: $(SRC_PATH)/avl_test.c
	@$(CC) -L$(LIBS_PATH) $(CFLAGS) $< -o $@ -l$(DYNAMIC_LIB_NAME)
	@printf "Building AVL Tree test "
	@printf "%0.31s" $(PADDING)
	@printf "%s\n\n" " PASSED"

rbk_test: $(SRC_PATH)/rbk_test.c
	@$(CC) -L$(LIBS_PATH) $(CFLAGS) $< -o $@ -l$(DYNAMIC_LIB_NAME)
	@printf "Building Red-Black Tree test "
	@printf "%0.25s" $(PADDING)
	@printf "%s\n\n" " PASSED"

hash_table_test: $(SRC_PATH)/hash_table_test.c
	@$(CC) -L$(LIBS_PATH) $(CFLAGS) $< -o $@ -l$(DYNAMIC_LIB_NAME)
	@printf "Building Hash Table test "
	@printf "%0.29s" $(PADDING)
	@printf "%s\n\n" " PASSED"

clean:
	@rm -rf $(EXEC_FILES)