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
	