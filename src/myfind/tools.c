#include "./myfind.h"

void print_ast(struct ast_node *ast, int i)
{
    if (!ast)
        return;
    i += 10;
    print_ast(ast->right_child, i);
    printf("\n");
    for (int j = 10; j < i; j++)
        printf(" ");
    printf("%d\n", ast->type);
    print_ast(ast->left_child, i);
}

void print_args(char **args)
{
    printf("args = ");
    while (*args)
    {
        printf("%s, ", *args);
        args++;
    }
    printf("\n");
}

void print_list_token(struct token **token_list)
{
    printf("token_list = ");
    while (*token_list)
    {
        printf("%d - ", (*token_list)->ast->type);
        token_list++;
    }
    printf("\n");
}
