CCFLAGS := -Wall -Werror

all:
	gcc $(CCFLAGS) -o sinterp sinterp.c

bin/sinterp: build/osculant.o build/oper_on_var.o build/sinterp.o build/condition.o
	gcc $(CCFLAGS) build/osculant.o build/oper_on_var.o build/sinterp.o build/condition.o -o bin/sinterp

build/sinterp.o: src/sinterp.c
	gcc $(CCFLAGS) -c src/sinterp.c -o build/sinterp.o

build/osculant.o: src/osculant.c src/osculant.h
	gcc $(CCFLAGS) -c src/osculant.c -o build/osculant.o

build/oper_on_var.o: src/oper_on_var.c src/oper_on_var.h
	gcc $(CCFLAGS) -c src/oper_on_var.c -o build/oper_on_var.o

build/condition.o: src/condition.c src/condition.h
	gcc $(CCFLAGS) -c src/condition.c -o build/condition.o