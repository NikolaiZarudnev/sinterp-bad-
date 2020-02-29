
#include "osculant.h"
#include "oper_on_var.h"
#include "condition.h"
#include "const.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int do_command(FILE *script, char *str, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines);

int do_if(FILE *script, char *str, char *temp, struct varlist *var_data, int *var_count_ptr, int num_lines, int cur_nlines)
{
	char* condition = (char *)malloc(sizeof(char) * 16);
	int i;
	int then_len;
	int check_id_log, check_condition, check_command;
	int temp_curlines = cur_nlines;
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
	int temp_curlines = cur_nlines;
	int lafter_while, count_lines_while, search_done;
	char *s_done = (char *)malloc(sizeof(char) * 16);
	long pos_while, pos_done, pos_cur;

	strcpy(s_done, str);
	lafter_while = num_lines - cur_nlines;
	pos_while = ftell(script);
	for (count_lines_while = 0; count_lines_while != lafter_while; count_lines_while++) {
		s_done = skip_space(s_done);
		if (strcmp(s_done, key_words[DONE]) == 0) {
			pos_done = ftell(script);
			count_lines_while--;
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
	int check_while, check_if;
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