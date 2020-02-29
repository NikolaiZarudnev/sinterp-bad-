#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


enum tokenID {
	ASSIGN,
	PLUS,
	MINUS,
	LARGER,
	LESS,
	EQUAL,
	READ,
	TYPE_READ_I,
	TYPE_READ_F,
	IF,
	THEN,
	ELSE,
	FI,
	WHILE,
	DO,
	DONE,
	PRINT,
	NUMBER_I,
	NUMBER_F,
	VAR
};

#define INTEGER_FLAG 0
#define FLOAT_FLAG 1

struct varlist{
	char key[16];
	int value_type;
	float value;
} var_data[32];

char *key_words[] = {
	"=",
	"+",
	"-",
	">",
	"<",
	"==",
	"read",
	"-i",
	"-f",
	"if",
	"then",
	"else",
	"fi",
	"while",
	"do",
	"done",
	"print"
};

int do_command(FILE *script, char *str, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines);

char *skip_space(char *str)
{
	int i, j;
	char* dest = (char *)malloc(sizeof(char) * 64);
	for(i = 0, j = 0; str[i] != '\0' && str[i] != '\n'; i++) {
        if(str[i] != ' ') {
            dest[j++] = str[i];
        }
	}
    dest[j] = '\0';
	return dest;
}

int get_id(char *str)
{
	int j = 0;
	while (str[j] == key_words[READ][j]) {
		if (key_words[READ][j+1] == '\0')
			return READ;
		j++;
	}
	j = 0;
	while (str[j] == key_words[IF][j]) {
		if (key_words[IF][j+1] == '\0')
			return IF;
		j++;
	}
	j = 0;
	while (str[j] == key_words[THEN][j]) {
		if (key_words[THEN][j+1] == '\0')
			return THEN;
		j++;
	}
	j = 0;
	while (str[j] == key_words[ELSE][j]) {
		if (key_words[ELSE][j+1] == '\0')
			return ELSE;
		j++;
	}
	j = 0;
	while (str[j] == key_words[FI][j]) {
		if (key_words[FI][j+1] == '\0')
			return FI;
		j++;
	}
	j = 0;
	while (str[j] == key_words[WHILE][j]) {
		if (key_words[WHILE][j+1] == '\0')
			return WHILE;
		j++;
	}
	j = 0;
	while (str[j] == key_words[DO][j]) {
		if (key_words[DO][j+1] == '\0')
			return DO;
		j++;
	}
	j = 0;
	while (str[j] == key_words[DONE][j]) {
		if (key_words[DONE][j+1] == '\0')
			return DONE;
		j++;
	}
	j = 0;
	while (str[j] == key_words[PRINT][j]) {
		if (key_words[PRINT][j+1] == '\0')
			return PRINT;
		j++;
	}
	j = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == key_words[ASSIGN][j])
			return ASSIGN;
	}
	return -1;
}

int search_var(struct varlist *var_data, int *var_count_ptr, char *variable)
{
	for (int k = 0; k != *var_count_ptr; k++) {
		if (strcmp(variable, var_data[k].key) == 0) {
			return k;
		}
	}
	return *var_count_ptr;
}

int get_id_oper(char *value)
{
	for (int i = 0; value[i] != '\0'; ++i) {
		if (value[i] == '+') {
			return PLUS;
		} else if (value[i] == '-'){
			return MINUS;
		}
	}
	return 0;
}

float ident_value(char *value, struct varlist *var_data, int *var_count_ptr, int *flag_p)
{
	for (int i = 0; value[i] != '\0'; ++i) {
		if (value[i] == '.') {
			*flag_p += 1;
			return atof(value);
		}
	}
	for (int i = 0; value[i] != '\0'; ++i) {
	    if (value[i] >= '0' && value[i] <= '9') {
	    	*flag_p = 0;
			return atoi(value);
		}
	}
	for (int i = 0; i != *var_count_ptr; i++) {
		if (strcmp(value, var_data[i].key) == 0) {
			if (var_data[i].value_type == FLOAT_FLAG) {
				*flag_p += 1;
				return var_data[i].value;
			} else {
				*flag_p = 0;
				return var_data[i].value;
			}
		}
	}
	return INT_MAX;
}



char *operations(char *value, struct varlist *var_data, int *var_count_ptr)
{
	char* left_val = (char *)malloc(sizeof(char) * 16);
	char* right_val = (char *)malloc(sizeof(char) * 16);
	char* temp_val = (char *)malloc(sizeof(char) * 16);
	char* float_to_string = (char *)malloc(sizeof(char) * 16);
	char* int_to_string = (char *)malloc(sizeof(char) * 16);

	int flag = 0;
	int* flag_p = &flag;
	float left_val_num, right_val_num, value_num;
	int j = 0;
	char *error1 = "error**";
	char *error2 = "error***";
	switch (get_id_oper(value)) {
		case PLUS:
			while (value[j] != '+') {
				j++;
			}
			strncpy(left_val, value, j);
			left_val_num = ident_value(left_val, var_data, var_count_ptr, flag_p);

			temp_val = value + j + 1;
			strcpy(right_val, temp_val);
			right_val_num = ident_value(right_val, var_data, var_count_ptr, flag_p);	

			if ((left_val_num == INT_MAX) || (right_val_num == INT_MAX)) {
				return error1;
			}

			value_num = left_val_num + right_val_num;
			sprintf(float_to_string, "%f", value_num);
			if (flag == INTEGER_FLAG) {
				int i;
				for (i = 0; float_to_string[i] != '.'; i++);
				strncpy(int_to_string, float_to_string, i);
				return int_to_string;
			} else if (flag == 1) {
				return error2;
			}

			*var_count_ptr -= 1;
			return float_to_string;
		case MINUS:
			while (value[j] != '-') {
				j++;
			}
			strncpy(left_val, value, j);
			left_val_num = ident_value(left_val, var_data, var_count_ptr, flag_p);

			temp_val = value + j + 1;
			strcpy(right_val, temp_val);
			right_val_num = ident_value(right_val, var_data, var_count_ptr, flag_p);	

			if ((left_val_num == INT_MAX) || (right_val_num == INT_MAX)) {
				return error1;
			}

			value_num = left_val_num + right_val_num;
			sprintf(float_to_string, "%f", value_num);
			if (flag == INTEGER_FLAG) {
				int i;
				for (i = 0; float_to_string[i] != '.'; i++);
				strncpy(int_to_string, float_to_string, i);
				return int_to_string;
			} else if (flag == 1) {
				return error2;
			}

			*var_count_ptr -= 1;
			return float_to_string;
	}
	return value;
}

int push_var(struct varlist *var_data, char *variable, char *value, int *var_count_ptr)
{
	int var_place;
	var_place = search_var(var_data, var_count_ptr, variable);
	for (int i = 0; value[i] != '\0'; ++i) {
		if (value[i] == '.') {
			strcpy(var_data[var_place].key, variable);
			var_data[var_place].value = atof(value);
			var_data[var_place].value_type = FLOAT_FLAG;
			*var_count_ptr += 1;
			return 1;
		}
	}
	for (int i = 0; value[i] != '\0'; ++i) {
	    if (value[i] >= '0' && value[i] <= '9') {
			strcpy(var_data[var_place].key, variable);
			var_data[var_place].value_type = INTEGER_FLAG;
			var_data[var_place].value = atoi(value);
			*var_count_ptr += 1;
			return 1;
		}
	}
	for (int i = 0; i != *var_count_ptr; i++) {
		if (strcmp(value, var_data[i].key) == 0) {
			strcpy(var_data[var_place].key, variable);
			var_data[var_place].value_type = var_data[i].value_type;
			var_data[var_place].value = var_data[i].value;
			return 1;
		}
	}
	return 0;
}

int check_variable(char *variable)
{
	if (!variable[0]) {
			return 0;
		}
	if (get_id(variable) != -1) {
		return 0;
	}
	for (int j = 0; variable[j] != '\0'; j++) {
		if ((variable[j] > '0' && variable[j] < '9') && (variable[j+1] > 'a' && variable[j+1] < 'z') && variable[j+1] != '\0') {
			printf("WRONG NAME VARIABLE\n");
			return 0;
		} else if (strcmp(variable, key_words[READ]) == 0) {
			return 0;
		} else if (strcmp(variable, key_words[TYPE_READ_I]) == 0) {
			return 0;
		} else if (strcmp(variable, key_words[TYPE_READ_F]) == 0) {
			return 0;
		} else if (strcmp(variable, key_words[IF]) == 0) {
			return 0;
		} else if (strcmp(variable, key_words[WHILE]) == 0) {
			return 0;
		} else if (strcmp(variable, key_words[PRINT]) == 0) {
			return 0;
		}
	}
	return 1;
}

int var_val(char *str, struct varlist *var_data, int *var_count_ptr)
{
	int i = 0;
	int check_pushvar;
	char* value = (char *)malloc(sizeof(char) * 16);
	char* variable = (char *)malloc(sizeof(char) * 16);
	char* temp = (char *)malloc(sizeof(char) * 16);

	while (str[i] != '=') {
		i++;
	}
	strncpy(variable, str, i);
	if (check_variable(variable) == 0) {
		return -3;
	}
	temp = str + i + 1;
	strcpy(value, temp);
	strcpy(value, operations(value, var_data, var_count_ptr));
	if (strcmp(value, "error**") == 0) {
		return -2;
	} else if (strcmp(value, "error***") == 0) {
		return -5;
	}
	check_pushvar = push_var(var_data, variable, value, var_count_ptr);
	if (check_pushvar == 0) {
		return -4;
	}
	return 1;
}



void push_read(struct varlist *var_data, char *str, int flag, int *var_count_ptr)
{
	char* temp = (char *)malloc(sizeof(char) * 16);
	int var_place;

	temp = str + 6;
	var_place = search_var(var_data, var_count_ptr, temp);
	if (var_place == *var_count_ptr) {
		*var_count_ptr += 1;
	}
	strcpy(var_data[var_place].key, temp);
	if (flag == INTEGER_FLAG) {
		var_data[var_place].value_type = INTEGER_FLAG;
	} else if (flag == FLOAT_FLAG) {
		var_data[var_place].value_type = FLOAT_FLAG;
	}
	scanf("%f", &var_data[var_place].value);
}

void do_print(char *temp, struct varlist *var_data, int *var_count_ptr)
{
	int var_place_print = search_var(var_data, var_count_ptr, temp);
	if (var_place_print == *var_count_ptr) {
		printf("%s\n", temp);
	} else {
		if (var_data[var_place_print].value_type == INTEGER_FLAG) {
			printf("%d\n", (int)var_data[var_place_print].value);
		} else {
			printf("%f\n", var_data[var_place_print].value);
		}
	}
}



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



int do_if(FILE *script, char *str, char *temp, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines)
{
	char* condition = (char *)malloc(sizeof(char) * 16);
	int i;
	int then_len;
	int check_id_log, check_condition, check_command;
	int lafter_if, count_lines_if;
	int check_else = 0;
	char *s_fi = (char *)malloc(sizeof(char) * 16);
	char *s_else = (char *)malloc(sizeof(char) * 16);
	long pos_if, pos_fi, pos_else, pos_cur;
	strcpy(s_fi, str);
	lafter_if = num_lines - cur_nlines;
	pos_if = ftell(script);
	for (count_lines_if = 0; !feof(script); count_lines_if++) {
		s_fi = skip_space(s_fi);
		if (strcmp(s_fi, key_words[FI]) == 0) {
			pos_fi = ftell(script);
			count_lines_if--;
			break;
		}
		fgets(s_fi, 16, script);
	}
	fseek(script, pos_if, SEEK_SET);
	if (count_lines_if == lafter_if + 1) {
		printf("Отсутствует fi\n");
		return -1;
	}
	strcpy(s_else, str);
	for (count_lines_if = 0; count_lines_if != lafter_if + 1; count_lines_if++) {
		s_else = skip_space(s_else);
		if (strcmp(s_else, key_words[ELSE]) == 0) {
			check_else = 1;
			pos_else = ftell(script);
			break;
		}
		fgets(s_else, 32, script);
	}
	fseek(script, pos_if, SEEK_SET);
	i = strlen(temp);
	then_len = strlen(key_words[THEN]);
	while (temp[i] == key_words[THEN][then_len]) {
		i--;
		then_len--;
	}
	if (then_len + 1 == 0) {
		strncpy(condition, temp, i + 1);
	} else {
		printf("Отсутствует then\n");
		return -1;
	}
	check_id_log = get_id_operations_logic(condition);
	if (check_id_log == -1) {
		return -2;
	}
	check_condition = do_oper_log(check_id_log, condition, var_data, var_count_ptr);
	if (check_condition == 0) {
		if (check_else == 1) {
			fseek(script, pos_else, SEEK_SET);
			pos_cur = pos_else;
			while (pos_cur < pos_fi) {
				fgets(str, 32, script);
				check_command = do_command(script, str, var_data, var_count_ptr, num_lines, cur_nlines);
				if (check_command == -1) {
					return -1;
				} else if (check_command == FI) {
					break;
				}
				pos_cur = ftell(script);
			}
		}
		fseek(script, pos_fi, SEEK_SET);
		return 1;
	}

	if (check_condition == 1) {
		fseek(script, pos_if, SEEK_SET);
		pos_cur = pos_if;
		while (pos_cur < pos_fi) {
			fgets(str, 32, script);
			check_command = do_command(script, str, var_data, var_count_ptr, num_lines, cur_nlines);
			if (check_command == -1) {
				return -1;
			} else if (check_command == FI) {
				break;
			}
			pos_cur = ftell(script);
		}
		fseek(script, pos_fi, SEEK_SET);
		return 1;
	}
	return 1;
}

int do_while(FILE *script, char *str, char *temp, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines)
{
	char* condition = (char *)malloc(sizeof(char) * 16);
	int i;
	int do_len;
	int check_id_log, check_condition, check_command;
	int lafter_while, count_lines_while;
	char *s_done = (char *)malloc(sizeof(char) * 16);
	long pos_while, pos_done, pos_cur;

	strcpy(s_done, str);
	lafter_while = num_lines - cur_nlines;
	pos_while = ftell(script);
	for (count_lines_while = 0; count_lines_while != lafter_while + 1; count_lines_while++) {
		s_done = skip_space(s_done);
		if (strcmp(s_done, key_words[DONE]) == 0) {
			pos_done = ftell(script);
			break;
		}
		fgets(s_done, 16, script);
	}

	fseek(script, pos_while, SEEK_SET);
	if (count_lines_while == lafter_while + 1) {
		printf("Отсутствует done\n");
		return -1;
	}
	i = strlen(temp);
	do_len = strlen(key_words[DO]);
	while (temp[i] == key_words[DO][do_len]) {
		i--;
		do_len--;
	}
	if (do_len + 1 == 0) {
		strncpy(condition, temp, i + 1);
	} else {
		printf("Отсутствует do\n");
		return -1;
	}
	check_id_log = get_id_operations_logic(condition);
	if (check_id_log == -1) {
		return -2;
	}
	check_condition = do_oper_log(check_id_log, condition, var_data, var_count_ptr);
	if (check_condition == 0) {
		fseek(script, pos_done, SEEK_SET);
		return 1;
	}

	while (check_condition == 1) {
		fseek(script, pos_while, SEEK_SET);
		pos_cur = pos_while;
		while (pos_cur < pos_done) {
			fgets(str, 32, script);
			check_command = do_command(script, str, var_data, var_count_ptr, num_lines, cur_nlines);
			if (check_command == -1) {
				return -1;
			} else if (check_command == DONE) {
				break;
			}
			pos_cur = ftell(script);
		}
		check_condition = do_oper_log(check_id_log, condition, var_data, var_count_ptr);
	}
	return 1;
}

int do_command(FILE *script, char *str, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines)
{
	int check_varval;
	int check_type_read;
	int check_while, check_if = 0;
	int str_id;
	char* temp = (char *)malloc(sizeof(char) * 12);
	char* error_line = (char *)malloc(sizeof(char) * 12);
	strcpy(error_line, str);
	str = skip_space(str);
	str_id = get_id(str);
	switch(str_id) {
		case ASSIGN:
			check_varval = var_val(str, var_data, var_count_ptr);
			if (check_varval == -2) {
				printf("Операция с разными типами данных\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			} else if (check_varval == -3) {
				printf("Недопустимое имя переменной\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			} else if (check_varval == -4) {
				printf("Недопустимое значение\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			} else if (check_varval == -5) {
				printf("Неизвестная переменная в операции\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			}
			break;
		case READ:
			temp = str + 4;
			int j = 0;
			while (temp[j] == key_words[TYPE_READ_I][j]) {
				if (j == 1) {
					check_type_read = INTEGER_FLAG;
					push_read(var_data, str, check_type_read, var_count_ptr);
					break;
				}
				j++;
			}
			j = 0;
			while (temp[j] == key_words[TYPE_READ_F][j]) {
				if (j == 1) {
					check_type_read = FLOAT_FLAG;
					push_read(var_data, str, check_type_read, var_count_ptr);
					break;
				}
				j++;
			}
			if (j < 1) {
				printf("Неверный спецификатор\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			}
			break;
		case PRINT:
			temp = str + 5;
			do_print(temp, var_data, var_count_ptr);
			break;
		case WHILE:
			temp = str + 5;
			check_while = do_while(script, str, temp, var_data, var_count_ptr, num_lines, cur_nlines);
			if (check_while == -1) {
				return -1;
			} else if (check_if == -2) {
				printf("Неизвестная логическая операция\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			}
			break;
		case DONE:
			return DONE;
		case IF:
			temp = str + 2;
			check_if = do_if(script, str, temp, var_data, var_count_ptr, num_lines, cur_nlines);
			if (check_if == -1) {
				return -1;
			} else if (check_if == -2) {
				printf("Неизвестная логическая операция\n");
				printf("%d: %s\n", cur_nlines, error_line);
				return -1;
			}
			break;
		case FI:
			return FI;
		case -1:
			printf("Неизвестная переменная\n");
			printf("%d: %s\n", cur_nlines, error_line);
			return -1;
		case -2:
			printf("\n");
			printf("%d: %s\n", cur_nlines, error_line);
			return -1;
	}
	return 0;
}

int walking_script(FILE *script, char *str, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines)
{
	int check_command;
	while(!feof(script)) {
		if (fgets(str, 32, script)) {
			cur_nlines++;
			if (str[0] == '\0') {
				continue;
			}
			check_command = do_command(script, str, var_data, var_count_ptr, num_lines, cur_nlines);
			if (check_command == -1) {
				return -1;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	FILE* script = fopen(argv[1], "r");
	int num_lines = 0;
	int var_count = 0;
	int cur_nlines = 0;
	int* var_count_ptr;
	var_count_ptr = &var_count;
	char* str = (char *)malloc(sizeof(char) * 16);
	int check_walk;
	if (feof(script) != 0) {
		printf("Пустой файл\n");
		return 0;
	}
	while (!feof(script)) {
		if (fgets(str, 32, script)) {
			num_lines++;
		}
	}
	fseek(script, 0, 0);
	check_walk = walking_script(script, str, var_data, var_count_ptr, num_lines, cur_nlines);
	if (check_walk == -1) {
		return 0;
	}
	free(str);
	fclose(script);
	return 0;
}