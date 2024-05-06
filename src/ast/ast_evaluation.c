#include <stdio.h>

#include "./ast.h"

void free_ast(struct ast_node *ast)
{
    if (ast)
    {
        free(ast->fun);
        free(ast->arg);
        free_ast(ast->left_child);
        free_ast(ast->right_child);
        free(ast);
    }
}

bool evaluate(struct ast_node *node, struct file *file)
{
    if (node)
    {
        if (node->type == AND)
        {
            return evaluate(node->left_child, file)
                && evaluate(node->right_child, file);
        }
        else if (node->type == OR)
        {
            return evaluate(node->left_child, file)
                || evaluate(node->right_child, file);
        }
        else if (node->type == NOT)
        {
            return !evaluate(node->right_child, file);
        }
        else
        {
            return node->fun->fun(file, node->arg);
        }
    }
    return false;
}
