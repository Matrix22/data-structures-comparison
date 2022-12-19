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

DELIMETER = "-------------------------------------------------------------------"

build: all

all: build_dynamic_lib build_tests

build_dynamic_lib:
	@cd build && make --no-print-directory all
	@printf "%0.61s\n\n" $(DELIMETER)

build_tests:
	@cd test && make --no-print-directory all
	@printf "%0.61s\n\n" $(DELIMETER)

run-p1:
	@printf "Running Algorithm No: 1\n\n"
	@./test/exec/avl_test

run-p2:
	@printf "Running Algorithm No: 2\n\n"
	@./test/exec/rbk_test

run-p3:
	@printf "Running Algorithm No: 3\n\n"
	@./test/exec/hash_table_test

run-p4:
	@printf "Running Algorithm No: 4\n\n"
	@./test/exec/hash_table_nosize

run-best:
	@printf "Running Best Algorithm\n\n"
	@./test/exec/hash_table_test

run-all: run-p1 run-p2 run-p3 run-p4

clean:
	@cd test && make --no-print-directory clean
	@rm -rf libs
	