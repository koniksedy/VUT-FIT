# Makefile pro sestavení IJF projektu - překladač
# Napsal Michal Šedý <xsedym02>
# Poslední změna dne: 11.11.2019
# Makefile je opatřen ladívím výpisem, před odevzdáním odstraň -g

CC=gcc
CFLAGS= -g #-Wall -Wextra -pedantic
OBJECT= error.o instr_tape.o math_expr.o param.o parse.o prec_stack.o scanner.o stack.o string_dynamic.o string_generator.o symtable.o generator.o
TARGETS=compiler $(OBJECT)

all: $(TARGETS)

compiler: main.c $(OBJECT)
	$(CC) $(CFLAGS) $^  -o $@

%.o:%.c
	$(CC) $(CFLAGS) $< -c

clean:
	rm -v $(TARGETS) 
