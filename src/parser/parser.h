#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../exprtypes.h"

enum associativity
{
    NONE,
    LEFT,
    RIGHT
};

struct token
{
    enum associativity asso;
    int prio;
    struct ast_node *ast;
};

struct token **list_token(char **args, int argc);
struct ast_node *shunting_yard(struct token **list_token);

bool is_valid_arg(enum expr_type type, char *arg);

#endif /* !PARSER_H */
