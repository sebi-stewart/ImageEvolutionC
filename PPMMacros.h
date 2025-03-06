//
// Created by Sebastian Stewart on 02/03/2025.
//

#ifndef IMAGE_EVOLUTION_PPM_MACROS_H
#define IMAGE_EVOLUTION_PPM_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int count;
    char function_name[256];
} CallData;

typedef struct {
    CallData* data;
    int size;
} CallList;


void print_call_to_file(int exit_code, char* function_name);
void clear_call_file();
bool retrieve_err_call_list_and_compare(CallList* expected);
bool retrieve_err_call_and_compare(CallData expected);

#ifndef PPM_TESTING
#define BREAK_OUT(err_code) exit(err_code)
#define BREAK_OUT_VOID(err_code) exit(err_code)

#else
#define BREAK_OUT(err_code) print_call_to_file(err_code, __func__); return NULL;
#define BREAK_OUT_VOID(err_code) print_call_to_file(err_code, __func__); return;
#endif

#endif //IMAGE_EVOLUTION_PPM_MACROS_H
