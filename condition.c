#include "condition.h"

int get_id_operations_logic(char *str)
{
	int j = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == key_words[LARGER][j]) {
			return LARGER;
		} else if (str[i] == key_words[LESS][j]) {
			return LESS;
		} else if ((str[i] == key_words[EQUAL][j]) && (str[i + 1] == key_words[EQUAL][j + 1])) {
			return EQUAL;
		}
	}
	return -1;
}

int do_cond_log(char *l_cond_var, char *r_cond_var, struct varlist *var_data, int *var_count_ptr, int flag_cond)
{
	float num_l, num_r;
	int flag_l = 0;
	int flag_r = 0;
	int *flag_lp = &flag_l;
	int *flag_rp = &flag_r;
	num_l = ident_value(l_cond_var, var_data, var_count_ptr, flag_lp);
	num_r = ident_value(r_cond_var, var_data, var_count_ptr, flag_rp);
	if (flag_r == flag_l) {
		if (flag_cond == EQUAL) {
			if (num_l == num_r) {
				return 1;
			}
		}
		if (num_l > num_r) {
			return 1;
		}
	}
	return 0;
}

int do_oper_log(int check_id_log, char *condition, struct varlist *var_data, int *var_count_ptr)
{
	char* temp = (char *)malloc(sizeof(char) * 16);
	char* r_cond_var = (char *)malloc(sizeof(char) * 16);
	char* l_cond_var = (char *)malloc(sizeof(char) * 16);
	int tr_or_fls;
	int i;
	if (check_id_log == LARGER) {
		for (i = 0; condition[i] != key_words[LARGER][0]; i++);
		strncpy(l_cond_var, condition, i);
		temp = condition + i + 1;
		strcpy(r_cond_var, temp);
		tr_or_fls = do_cond_log(l_cond_var, r_cond_var, var_data, var_count_ptr, LARGER);
		return tr_or_fls;
	} else if (check_id_log == LESS) {
		for (i = 0; condition[i] != key_words[LESS][0]; i++);
		strncpy(l_cond_var, condition, i);
		temp = condition + i + 1;
		strcpy(r_cond_var, temp);
		tr_or_fls = do_cond_log(r_cond_var, l_cond_var, var_data, var_count_ptr, LESS);
		return tr_or_fls;
	} else if (check_id_log == EQUAL) {
		for (i = 0; condition[i] != key_words[EQUAL][0]; i++);
		strncpy(l_cond_var, condition, i);
		temp = condition + i + 1;
		strcpy(r_cond_var, temp);
		tr_or_fls = do_cond_log(l_cond_var, r_cond_var, var_data, var_count_ptr, EQUAL);
		return tr_or_fls;
	}
	return -1;
}