#pragma once

#include "stdbool.h"
#include <stdint.h>

#include "hashmap.h"

#define STACK_DEFAULT 15

typedef struct {
    // indicates the top of the stack :)
    int32_t top;

    // map for variables, storing its key and value (only strings for now)
    // allocate a new map each time a scope is created
    // free the map when scope is popped
    // up to 15 scopes can exist at the same time due to STACK_DEFAULT macro
    Map *data[STACK_DEFAULT];
} ScopeStack;

typedef struct {
    int32_t scope_count;

    // global data is separated from local data
    // global data is of ScopeStack type as global data might as well have multiples scopes
    // within conditional and loop blocks
    ScopeStack global;

    // stack storing function data
    // each function receives its own stack data
    // for now up to 15 functions can be called at the same time due to STACK_DEFAULT macro
    ScopeStack *fn_scopes[STACK_DEFAULT];
} SymbolTable;

// should be called only once at program start
// populate the program stack with its intial values
void init_stack();

// call at the end of the program to clean all global data initially allocated
void global_stack_cleanup();

// simple function to print all environment variables
void print_env();

// push a new scope at a *ScopeStack struct
// returns true on success and false at failure
bool scope_stack_push(ScopeStack *st);

// remove a single scope at the given stack
// individual data from the map must be freed individually
// as the free_map function only frees data allocated to the keys
void scope_stack_pop(ScopeStack *st);

// this function returns the index to a new funciton stack data
// the function should not access data below the given index as it would access another functions stack data
int32_t main_stack_push_local();

// remove a single entry from stack
// must be called whenever a function returns
// the data allocated by single entities must be freed internally
// returns the former index of the function stack data
void main_stack_pop_local(uint32_t index);

// get a value from the stack starting from the top
// search the lowers levels of the stack if the value is not at the top
// search global values if the value is not it stack
char *get_value_from_stack(ScopeStack *st, char *key);

// push a single value at the top of the pointed stack
void scope_stack_push_data(ScopeStack *st, char *key, char *data);