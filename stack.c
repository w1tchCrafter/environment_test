#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

// some environment variables used for testing

#ifdef USE_VARS

static const char *env[] = {ENV_VARS};

#else

static const char *env[] = { "USER", "SHELL" };

#endif

// this variable should be static but i'll let it public for testing purposes
SymbolTable program_stack = { 0 };

extern char **environ;

// static function that loads all the environment variables to the global stack
static void init_env() {
    ScopeStack *global = &program_stack.global;
    char **ptr = environ;

	while (*ptr) {
		char *data = *ptr;
		char *value = strchr(data, '=');
		char key[strlen(data) - strlen(value) + 1];
		int idx = value-data;
		
		strncpy(key, data, idx);
		key[idx] = '\0';

		map_insert(global->data[0], key, ++value);
		ptr++;
	}
}

void print_env() {
    ScopeStack *global = &program_stack.global;
    uint64_t env_size = sizeof(env) / sizeof(char*);
    
    for (int i = 0; i < env_size; i++) {
        char *value = map_get(global->data[0], env[i]);
        if (value != NULL) printf("%s: %s\n", env[i], value);
    }
}

void init_stack() {
    ScopeStack *global = &program_stack.global;
    global->top = 0;
    global->data[global->top] = new_map();
    program_stack.scope_count = -1;

    init_env();
}

void global_stack_cleanup() {
    scope_stack_pop(&program_stack.global);
}

inline bool scope_stack_push(ScopeStack *st) {
    // increment the top of the stack and assign a pointer to it
    Map *new_scope = new_map();
    st->data[++st->top] = new_scope;
    
    return st->data[st->top] != NULL;
}

void scope_stack_pop(ScopeStack *st) {
    free_map(st->data[st->top]);
    st->top--;
}

int32_t main_stack_push_local() {
    int32_t count = ++program_stack.scope_count;
    program_stack.fn_scopes[count] = malloc(sizeof(ScopeStack));
    *program_stack.fn_scopes[count] = (ScopeStack) {
        .top = 0,
        .data[0] = new_map()
    };

    // returns the index to function stack data
    return program_stack.scope_count;
}

void main_stack_pop_local(uint32_t index) {
    memset(&program_stack.fn_scopes[index], 0, sizeof(ScopeStack));
    program_stack.scope_count--;
}

char *get_value_from_stack(ScopeStack *st, char *key) {
    int32_t temp_top = st->top;

    while (temp_top >= 0) {
        char *data = map_get(st->data[temp_top], key);
        if (data != NULL) return data;

        temp_top--;
    }
 
    // search in global scope if the provide key was not found in a local scope
    temp_top = program_stack.global.top;
    while (temp_top >= 0) {
        char *data = (char*)map_get(program_stack.global.data[temp_top], key);    
        if (data != NULL) return data;

        temp_top--;
    }

    return NULL;
}

inline void scope_stack_push_data(ScopeStack *st, char *key, char *data) {
    if (st->data[st->top] == NULL) {
        printf("Data at %d is NULL\n", st->top);
        return;
    }

    map_insert(st->data[st->top], key, data);
    puts("pushed data");
}