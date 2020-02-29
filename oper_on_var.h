#pragma once
#include "const.h"

char *operations(char *value, struct varlist *var_data, int *var_count_ptr);
int push_var(struct varlist *var_data, char *variable, char *value, int *var_count_ptr);
int check_variable(char *variable);
int var_val(char *str, struct varlist *var_data, int *var_count_ptr);
void push_read(struct varlist *var_data, char *str, int flag, int *var_count_ptr);
void do_print(char *temp, struct varlist *var_data, int *var_count_ptr);