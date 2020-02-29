#include "const.h"
#include "osculant.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
	for (int i = 0, j = 0; value[i] != '\0'; ++i) {
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
			}
			*flag_p = 0;
			return var_data[i].value;
		}
	}
	return INT_MAX;
}
