#ifndef MYFIND_H
#define MYFIND_H

#include "../ast/ast.h"
#include "../exprtypes.h"
#include "../file/file.h"
#include "../parser/parser.h"

struct myfind
{
    struct ast_node *ast;
    char *entry_point;
};

void print_ast(struct ast_node *ast, int i);
void print_list_token(struct token **token_list);

#endif /* !MYFIND_H */
