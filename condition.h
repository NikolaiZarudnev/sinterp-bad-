#pragma once
#include "const.h"
int get_id_operations_logic(char *str);
int do_cond_log(char *l_cond_var, char *r_cond_var, struct varlist *var_data, int *var_count_ptr, int flag_cond);
int do_oper_log(int check_id_log, char *condition, struct varlist *var_data, int *var_count_ptr);
