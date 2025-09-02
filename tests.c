#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "stack.h"

extern SymbolTable program_stack;

void test_init_stack() {
    printf("Test: test_init_stack\n");
    init_stack();

    assert(program_stack.scope_count == -1);
    assert(program_stack.global.top == 0);
    assert(program_stack.global.data[0] != NULL);

    // test if all environment variables have been initialized
    print_env();

    printf("test_init_stack: OK all tests passed\n");
}

void test_global_stack_cleanup() {
    printf("Test: test_global_stack_cleanup\n");

    global_stack_cleanup();
    assert(program_stack.global.top == -1);

    printf("test_global_stack_cleanup: OK all tests passed\n");
}

void test_main_stack_push_local() {
    printf("Test: test_main_stack_push_local\n");

    int32_t former_count = program_stack.scope_count;
    uint32_t index = main_stack_push_local();

    assert(former_count == program_stack.scope_count - 1);
    assert(program_stack.fn_scopes[index] != NULL);
    assert(program_stack.fn_scopes[index]->top == 0);
    assert(program_stack.fn_scopes[index]->data[0] != NULL);

    printf("test_main_stack_push_local: OK all tests passed\n");
}

void test_scope_stack_push_data() {
    printf("Test: test_scope_stack_push_data\n");

    // 0 is the index of the stack pushed in the above test :)
    // hardcoded it cause its easier to work
    scope_stack_push_data(program_stack.fn_scopes[0], "John", "Doe");
    scope_stack_push_data(program_stack.fn_scopes[0], "foo", "bar");

    assert((char*)map_get(program_stack.fn_scopes[0]->data[0], "John") == "Doe");
    assert((char*)map_get(program_stack.fn_scopes[0]->data[0], "foo") == "bar");

    printf("test_scope_stack_push_data: OK all tests passed\n");
}

void test_scope_stack_push() {
    
}

int main() {
    test_init_stack();
    test_main_stack_push_local();
    test_scope_stack_push_data();

    test_global_stack_cleanup();
}