### CONSTANT DEFINITIONS ###

# C language compiler and flags
CC 						:=		gcc
CFLAGS 					:= 		-g -Wall -Wextra -Wpedantic \
								-Wformat=2 -Wno-unused-parameter \
								-Wshadow -Wwrite-strings -Wstrict-prototypes \
								-Wold-style-definition -Wredundant-decls \
								-Wnested-externs -Wmissing-include-dirs \
								-Wjump-misses-init -Wlogical-op -O2

# Linux commands for basic routines
COPY					:=		cp
LIB_CONF				:=		ldconfig
REMOVE					:=		rm

# Maximum Padding String
PADDING 				:= 		............................................................

# Source files info
SRC_PATH 				:= 		../src
SRC_FILES 				:= 		$(wildcard $(SRC_PATH)/*.c)

# Libraries files info
LIBS_PATH 				:= 		../libs

# Dynamic Library info
DYNAMIC_LIB_NAME 		:= 		libsetsalgos.so
DYNAMIC_LIB 			:= 		$(LIBS_PATH)/$(DYNAMIC_LIB_NAME)
DYNAMIC_OBJ_PATH 		:= 		dynamic_obj
DYNAMIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(DYNAMIC_OBJ_PATH)/%.o,$(SRC_FILES))

# Static Library info
STATIC_LIB_NAME 		:= 		libsetsalgos.a
STATIC_LIB 				:= 		$(LIBS_PATH)/$(STATIC_LIB_NAME)
STATIC_OBJ_PATH 		:= 		static_obj
STATIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(STATIC_OBJ_PATH)/%.o,$(SRC_FILES))

# Header files info
INCLUDE_PATH 			:= 		$(SRC_PATH)/include
HEADER_FILES			:=		$(wildcard $(INCLUDE_PATH)/*.h)

.PHONY: all build clear_term clean

### END OF CONSTANT DEFINITIONS ###

### DEFAULT TARGET ###

# Target to Build and Install the project
all: build clean
	@printf "Finished building libraries "
	@printf "%0.26s" $(PADDING)
	@printf "%s\n\n" " PASSED"

### END OF DEFAULT TARGET ###

### BUILDING ###

# Target to build the libraries from source files
build: clear_term $(LIBS_PATH) $(DYNAMIC_OBJ_PATH) $(DYNAMIC_LIB) $(STATIC_OBJ_PATH) $(STATIC_LIB)

# Target to clear the terminal screen
clear_term:
	@clear

# Create a directory for the libraries
$(LIBS_PATH):
	@mkdir -p $@

# Create a directory for the dynamic object files
$(DYNAMIC_OBJ_PATH):
	@mkdir -p $@

# Create the dynamic library
$(DYNAMIC_LIB): $(DYNAMIC_OFILES)
	@printf "\n"
	
	@$(CC) -shared -o $@ $^

	@printf "%s" "Building Dynamic Library "
	@printf "%0.29s" $(PADDING)
	@printf "%s\n\n" " PASSED"

# Create all object files from the source code for dynamic library
$(DYNAMIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_PATH)/%.h
	@gcc -fPIC $(CFLAGS) -o $@ -c $<

	@printf "%s" "Building dynamic $(shell basename $@ .o) "
	@file=$@ && printf "%0.$$((50 - $${#file}))s" $(PADDING)
	@printf "%s\n" " PASSED"

# Create a directory for the static object files
$(STATIC_OBJ_PATH):
	@mkdir -p $@

# Create the static library
$(STATIC_LIB): $(STATIC_OFILES)
	@printf "\n"
	
	@ar -rc $@ $^

	@printf "%s" "Building Static Library "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n\n" " PASSED"

# Create all object files from the source code for static library
$(STATIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_PATH)/%.h
	@gcc $(CFLAGS) -o $@ -c $<

	@printf "%s" "Building static $(shell basename $@ .o) "
	@file=$@ && printf "%0.$$((50 - $${#file}))s" $(PADDING)
	@printf "%s\n" " PASSED"

### END OF BUILDING ###

### CLEANING ###

# Target to clean just object files
clean:
	@$(REMOVE) -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH)

# Target to clean everything obtained from all or build target
cleanall:
	@$(REMOVE) -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH) $(LIBS_PATH)

### END OF CLEANING ###