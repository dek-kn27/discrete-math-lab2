// #include <bits/valarray_before.h>
#include <bits/stdc++.h>
#define BLOCK_SIZE 16

using namespace std;

typedef unsigned short int block;

unsigned char get_set_bit(block* st, unsigned long int bit_number) {
    return (st[bit_number / BLOCK_SIZE] >> (bit_number % BLOCK_SIZE)) % 2;
};

void add_set_bit(block* st, unsigned long int bit_number) {
    st[bit_number / BLOCK_SIZE] |= 1 << (bit_number % BLOCK_SIZE);
};

unsigned char increment_set(block* st, unsigned long int more_int_number) {
    (*st)++;
    if (!*st) {
        if (more_int_number) {
            increment_set(st + 1, more_int_number - 1);
        };
        return more_int_number;
    };
    return 1;
};

void print_set(block* st, long int start, long int stop) {
    unsigned long int u_length = stop - start;
    unsigned long int counter = 0;
    printf("{");
    for (unsigned long int i = 0; i < u_length; i++) {
        if (get_set_bit(st, i)) {
            counter++;
            printf("%4d,", start + i);
        };
    };
    printf("} (power: %d)\n", counter);
};

block* prepare_set(long int start, long int stop) {
    return (block*)calloc((stop - start) / BLOCK_SIZE, sizeof(block));
};

void reset_set(block* st, long int start, long int stop) {
    unsigned long int_number = (stop - start) / BLOCK_SIZE + 1;
    for (unsigned long i = 0; i < int_number; i++) {
        st[i] = 0;
    };
};

void add_to_set(block* st, long int elem, long int start, long int stop) {
    if (elem < stop) {
        add_set_bit(st, elem - start);
    };
};

block* extension(block* st, long int start, long int stop) {
    unsigned long int int_number = (stop - start) / BLOCK_SIZE + 1;
    block* new_set = prepare_set(start, stop);
    for (unsigned long int i = 0; i < int_number; i++) {
        new_set[i] = ~st[i];
    };
    return new_set;
};

block* intersection(block* a, block* b, long int start, long int stop) {
    unsigned long int int_number = (stop - start) / BLOCK_SIZE + 1;
    block* new_set = prepare_set(start, stop);
    for (unsigned long int i = 0; i < int_number; i++) {
        new_set[i] = a[i] & b[i];
    };
    return new_set;
};

block* symmetric_difference(block* a, block* b, long int start, long int stop) {
    block *new_set = prepare_set(start, stop);
    unsigned long int int_number = (stop - start) / BLOCK_SIZE + 1;
    for (unsigned long int i = 0; i < int_number; i++) {
        new_set[i] = a[i] ^ b[i];
    };
    return new_set;
};

unsigned char is_nonzero(block* st, long int start, long int stop) {
    unsigned long int int_number = (stop - start) / BLOCK_SIZE + 1;
    for (unsigned long int i = 0; i < int_number; i++) {
        if (st[i]) {
            return 1;
        };
    };
    return 0;
};

long int* prepare_arr() {
    return (long int*)calloc(0, sizeof(long int));
};

unsigned long int input_arr(long int* arr, long int* min_value_pointer, long int* max_value_pointer) {
    // повертає нову довжину
    unsigned long int n = 0;
    char space;
    long int* min_p = arr;
    long int* max_p = arr;

    do {
        n++;
        realloc(arr, n);
        scanf("%d%c", arr + (n - 1), &space);
        printf("");
        if (arr[n - 1] < *min_p) {
            min_p = arr + (n - 1);
        };
        if (arr[n - 1] > *max_p) {
            max_p = arr + (n - 1);
        };
    } while (space != '\n');

    *min_value_pointer = *min_p;
    *max_value_pointer = *max_p;

    return n;
};

int main() {
    // Вводимо через пробіл першу множину цілих чисел

    printf("Enter first set:\n");
    long int* arr_a = prepare_arr();
    long int min_in_a = 0;
    long int max_in_a = 0;
    unsigned long int arr_a_len = input_arr(arr_a, &min_in_a, &max_in_a);

    printf("Enter second set:\n");
    long int* arr_b = prepare_arr();
    long int min_in_b = 0;
    long int max_in_b = 0;
    unsigned long int arr_b_len = input_arr(arr_b, &min_in_b, &max_in_b);

    long int global_start = min(min_in_a, min_in_b);
    long int global_stop = max(max_in_a, max_in_b) + 1;

    // set 1
    block* set1 = prepare_set(global_start, global_stop);
    reset_set(set1, global_start, global_stop);
    for (unsigned long int i = 0; i < arr_a_len; i++) {
        add_to_set(set1, arr_a[i], global_start, global_stop);
    };

    // set 2
    block* set2 = prepare_set(global_start, global_stop);
    reset_set(set2, global_start, global_stop);
    for (unsigned long int i = 0; i < arr_b_len; i++) {
        add_to_set(set2, arr_b[i], global_start, global_stop);
    };

    block* result = symmetric_difference(set1, set2, global_start, global_stop);
    printf("Symmetric difference is:\n");
    print_set(result, global_start, global_stop);

    printf("\n");
    printf("Bulean is:\n");

    // виводимо булеан
    unsigned long int u_length = global_stop - global_start;
    unsigned long int more_int_number = u_length / BLOCK_SIZE;
    unsigned char t = 1;
    block* result_extension = extension(result, global_start, global_stop);
    block* st = prepare_set(global_start, global_stop);
    unsigned long int counter = 0;
    for (reset_set(st, global_start, global_stop); t; t = increment_set(st, more_int_number)) {
        if (!is_nonzero(intersection(result_extension, st, global_start, global_stop), global_start, global_stop)) {
            // belongs to bulean
            counter++;
            printf("%4d:   ", counter);
            print_set(st, global_start, global_stop);
        };
    };

    return 0;
};