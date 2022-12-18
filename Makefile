
build: all

all: build_dynamic_lib build_tests export_dynamic_lib

build_dynamic_lib:
	@cd build && make --no-print-directory all

build_tests:
	@cd test && make --no-print-directory all

export_dynamic_lib:
	@export LD_LIBRARY_PATH=libs/:setsalgos

run-p1:

run-p2:

run-p3:

run-best:

clean:
	@cd test && make --no-print-directory clean
	@rm -rf libs
	