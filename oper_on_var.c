#include "oper_on_var.h"
#include "osculant.h"

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

	temp = str + 6; //длина read + -i
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