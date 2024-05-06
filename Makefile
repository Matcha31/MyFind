CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS=

OBJ=./src/ast/ast_evaluation.o ./src/ast/functions.o ./src/parser/stack/stack.o ./src/parser/shunting_yard.o ./src/parser/token_list.o ./src/myfind/myfind.o ./src/file/filelist.o ./src/parser/check_arg.o ./src/myfind/tools.o

all: myfind

myfind: $(OBJ)
	$(CC) -o myfind $(LDFLAGS) $(OBJ)

check: LDFLAGS += -g -fsanitize=address
check: $(OBJ)
	$(CC) -o myfind $(LDFLAGS) $(OBJ)
	./tests/test_suite.sh

debug: CFLAGS+= -g -fsanitize=address
debug: LDFLAGS+= -g -fsanitize=address
debug: myfind

clean:
	$(RM) $(OBJ) myfind
