#pragma once

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
