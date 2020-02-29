#pragma once

#include "const.h"
char *skip_space(char *str);
int get_id(char *str);
int search_var(struct varlist *var_data, int *var_count_ptr, char *variabl);
int get_id_oper(char *value);
float ident_value(char *value, struct varlist *var_data, int *var_count_ptr, int *flag_p);
