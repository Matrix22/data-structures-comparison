import random;

UPPER_LIMIT = int(10e7)

def generate_unique_random_numbers(length):
    return random.sample(range(0, UPPER_LIMIT), length)

def generate_random_numbers(upper_bound, length):
    return random.choices(range(0, upper_bound), k=length)

def generate_unique_ordered_numbers(length):
    return list(range(0, length))

def generate_insert_test_unique_random_numbers(test_id, length):
    with open('../../in/insert' + str(test_id) + '.in', 'w') as writer:
        writer.write(str(length) + '\n')
        numbers = generate_unique_random_numbers(length)

        for number in numbers:
            writer.write('0 ' + str(number) + '\n')

def generate_insert_test_random_numbers(test_id, length):
    with open('../../in/insert' + str(test_id) + '.in', 'w') as writer:
        writer.write(str(length) + '\n')
        numbers = generate_random_numbers(length // 2, length)

        for number in numbers:
            writer.write('0 ' + str(number) + '\n')

def generate_insert_test_unique_ordered_numbers(test_id, length):
    with open('../../in/insert' + str(test_id) + '.in', 'w') as writer:
        writer.write(str(length) + '\n')
        numbers = generate_unique_ordered_numbers(length)

        for number in numbers:
            writer.write('0 ' + str(number) + '\n')

def main():

    random.seed()

    # Unique random numbers in any order
    # Generate tests just for insertion in the data structure
    # tests needed for populating the data structure
    for i in range(1, 11, 2):
        generate_insert_test_unique_random_numbers(i, int(10 ** (i // 2 + 2)))
        generate_insert_test_unique_random_numbers(i + 1, int(5 * (10 ** (i // 2 + 2))))

    # Non-unique random numbers in any order
    # Generate tests just for insertion in the data structure
    # number inserted are not unique in the data structure
    # however no duplicates may exist at the same time in the data structure
    for i in range(1, 11, 2):
        generate_insert_test_random_numbers(i + 10, int(10 ** (i // 2 + 2)))
        generate_insert_test_random_numbers(i + 11, int(5 * (10 ** (i // 2 + 2))))

    # Unique ordered numbers
    # Generate tests just for insertion in the data structure
    # tests needed for populating the data structure
    for i in range(1, 11, 2):
        generate_insert_test_unique_ordered_numbers(i + 20, int(10 ** (i // 2 + 2)))
        generate_insert_test_unique_ordered_numbers(i + 21, int(5 * (10 ** (i // 2 + 2))))

if __name__ == '__main__':
    main()