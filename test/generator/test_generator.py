##########################################################################################
 # @file test_generator.py
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

import random;

UPPER_LIMIT = int(1e7)

# Generate random numbers in any order
def generate_random_numbers(length):
    return random.choices(range(0, UPPER_LIMIT), k=length)

# Pick random query number
def pick_query_number():
    return random.choice([0, 1, 2, 3])

# Pick random number from the data set
def pick_number(data_set):
    return random.choice(data_set)

# Pick random number from the range
def pick_range_number():
    return random.choice(range(0, UPPER_LIMIT))

# Generate random queries
def generate_test_random_queries(test_id, length):
    with open('../../in/test' + str(test_id) + '.in', 'w') as writer:
        writer.write(str(2 * length) + '\n')
        numbers = generate_random_numbers(length)

        for number in numbers:
            writer.write('0 ' + str(number) + '\n')

        query_count = 2 * length
        print_data_structure_mutex = query_count // 7
        
        for _ in range(length):
            query_count += 1

            if query_count % print_data_structure_mutex == 0:
                query = 4
            else:
                query = pick_query_number()

            if query == 0:
                insert_number = pick_range_number()
                numbers.append(insert_number)

                writer.write('0 ' + str(insert_number) + '\n')
            elif query == 1:
                delete_number = pick_number(numbers)
                numbers.remove(delete_number)

                writer.write('1 ' + str(delete_number) + '\n')
            elif query == 2:
                check_number = pick_number(numbers)

                if check_number % 5 == 3:
                    check_number = pick_range_number()

                writer.write('2 ' + str(check_number) + '\n')
            elif query == 3:
                old_number = pick_number(numbers)
                numbers.remove(old_number)

                new_number = pick_range_number()
                numbers.append(new_number)

                writer.write('3 ' + str(old_number) + ' ' + str(new_number) + '\n')
            elif query == 4:
                writer.write('4\n')
            else:
                print('Error: Invalid query number: <' + query + '> ' + 'at test: <' + test_id + '>')

# Generator of all tests regarding to the length of the inserted data
# number of queries for this example cannot exceed the length of the inserted data
# we need this condition to be able to test randomly the data structures
def main():
    random.seed()

    # Inserted data : Non-unique random numbers in any order
    for i in range(1, 31, 1):
        generate_test_random_queries(i, int(1e5))

if __name__ == '__main__':
    main()
