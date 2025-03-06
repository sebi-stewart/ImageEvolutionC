//
// Created by Sebastian Stewart on 02/03/2025.
//

#include "PPMMacros.h"

#define TESTING_FILE "testing.txt"
#define MAX_LINE 256
#define CALL_TO_FILE_FORMAT_NEWLINE "%d::%s\n"


void print_call_to_file(int exit_code, char* function_name){
    printf("\n\texit() called from %s with exit code %d\t", function_name, exit_code);

    FILE *file = fopen(TESTING_FILE, "r");
    char line[MAX_LINE];
    int found = false;

    // Temporary storage for updated data
    char buffer[10000] = "";
    char temp_function[MAX_LINE];
    int count;
    if (file){
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, CALL_TO_FILE_FORMAT_NEWLINE, &count, temp_function) == 2) {
                if (strcmp(temp_function, function_name) == 0) {
                    count++;
                    found = true;
                }
                char temp_line[MAX_LINE];
                sprintf(temp_line, CALL_TO_FILE_FORMAT_NEWLINE, count, temp_function);
                strcat(buffer, temp_line);
            }
        }
        fclose(file);
    }

    if (!found) {
        char new_entry[MAX_LINE];
        sprintf(new_entry, CALL_TO_FILE_FORMAT_NEWLINE, 1, function_name);
        strcat(buffer, new_entry);
    }

    file = fopen(TESTING_FILE, "w");
    if (!file){
        fprintf(stderr, "print_call_to_file: File was unable to be created");
        exit(1);
    }
    fputs(buffer, file);
    fclose(file);
}

void clear_call_file(){
    FILE *file = fopen(TESTING_FILE, "w");
    if (!file){
        fprintf(stderr, "clear_call_file: File was unable to be created");
        exit(1);
    }
    fclose(file);
}

CallList* retrieve_call_list(){
    FILE *file = fopen(TESTING_FILE, "r");
    if (!file){
        fprintf(stderr, "retrieve_call_list: File was unable to be created");
        exit(1);
    }

    CallList* list = malloc(sizeof(CallList));
    if (list == NULL){
        fprintf(stderr, "retrieve_call_list: Unable to allocate memory for list");
        exit(1);
    }
    list->size = 0;
    list->data = malloc(sizeof(CallData));
    if (list->data == NULL){
        fprintf(stderr, "retrieve_call_list: Unable to allocate memory for list->data");
        exit(1);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        CallData data;
        if (sscanf(line, CALL_TO_FILE_FORMAT_NEWLINE, &data.count, data.function_name) == 2) {
            list->data = realloc(list->data, sizeof(CallData) * (list->size + 1));
            if (list->data == NULL){
                fprintf(stderr, "retrieve_call_list: Unable to allocate memory for list->data");
                exit(1);
            }
            list->data[list->size] = data;
            list->size++;
        }
    }
    fclose(file);

    CallList list2 = {.data= {&(CallData){.count=2, .function_name="retrieve_call_list"},
                              &(CallData){.count=2, .function_name="retrieve_call_list"}},
                      .size= list->size};

    return list;
}

bool has_called_function_times(CallList* list, CallData call){
    char* function_name = call.function_name;
    int count = call.count;
    if (list->data == NULL){
        return false;
    }
    for (int i = 0; i < list->size; i++){
        if (strcmp(list->data[i].function_name, function_name) == 0){
            return list->data[i].count == count;
        }
    }
    return false;
}

bool has_function_calls(CallList* expected, CallList* actual){
    if (expected->size != actual->size){
        return false;
    }

    for (int i = 0; i < expected->size; i++){
        if (actual->data == NULL){
            return false;
        }
        bool result = has_called_function_times(expected, actual->data[i]);
        if (!result){
            return false;
        }
    }
    return true;
}

void free_call_list(CallList* list){
    free(list->data);
    free(list);
}

bool retrieve_err_call_list_and_compare(CallList* expected){
    CallList* actual = retrieve_call_list();
    bool result = has_function_calls(expected, actual);
    free_call_list(actual);
    clear_call_file();
    return result;
}

bool retrieve_err_call_and_compare(CallData expected){
    CallList* list = retrieve_call_list();
    bool result = has_called_function_times(list, expected);
    free_call_list(list);
    clear_call_file();
    return result;
}
