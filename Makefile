DELIMETER = "-------------------------------------------------------------------"

build: all

all: build_dynamic_lib build_tests export_dynamic_lib

build_dynamic_lib:
	@cd build && make --no-print-directory all
	@printf "%0.61s\n\n" $(DELIMETER)

build_tests:
	@cd test && make --no-print-directory all
	@printf "%0.61s\n\n" $(DELIMETER)

export_dynamic_lib:
	@export LD_LIBRARY_PATH=libs/:setsalgos
	@printf "Exporting Dynamic Library\n\n"
	@printf "%0.61s\n" $(DELIMETER)

run-p1:
	@printf "Running Algorithm No: 1\n\n"
	@./test/avl_test

run-p2:
	@printf "Running Algorithm No: 2\n\n"
	@./test/rbk_test

run-p3:
	@printf "Running Algorithm No: 3\n\n"
	@./test/hash_table_test

run-best:
	@printf "Running Best Algorithm\n\n"
	@./test/hash_table_test

clean:
	@cd test && make --no-print-directory clean
	@rm -rf libs
	