#include <stdio.h>

#include "./parser.h"
#include "./stack/stack.h"

int nb_token_stor(struct token **list_tks)
{
    int n = 0;
    int i = 0;
    while (list_tks[i])
    {
        if (list_tks[i]->ast->type != RIGHT_P
            && list_tks[i]->ast->type != LEFT_P)
            n++;
        i++;
    }
    return n;
}

void free_token_list(struct token **list_token)
{
    int i = 0;
    while (list_token[i])
    {
        if (list_token[i]->ast->type >= LEFT_P)
        {
            free_ast(list_token[i]->ast);
        }
        free(list_token[i]);
        i++;
    }
    free(list_token);
}

void print_stack(struct stack *stack)
{
    if (stack)
    {
        if (stack->data->ast->type)
            printf("%d", stack->data->ast->type);
        if (stack->next)
            print_stack(stack->next);
    }
}

void merge(struct stack **stackp, struct stack **outputp)
{
    struct stack *stack = *stackp;
    struct stack *output = *outputp;
    if (!output || !stack)
        errx(1, "myfind: invalid syntax\n");
    top(stack)->ast->right_child = top(output)->ast;
    output = pop(output);
    if (top(stack)->ast->type != NOT)
    {
        if (!output || !stack)
            errx(1, "myfind: invalid syntax 1\n");
        top(stack)->ast->left_child = top(output)->ast;
        output = pop(output);
    }
    output = push(output, top(stack));
    stack = pop(stack);
    *stackp = stack;
    *outputp = output;
}

int cmp_op(struct token *t, struct token *k)
{
    if (k->ast->type == LEFT_P)
        return 0;
    if (k->prio < t->prio)
        return 0;
    if (k->prio == t->prio && t->asso != LEFT)
        return 0;
    return 1;
}

void operator_to_stack(struct stack **stackp, struct stack **outputp,
                       struct token *current)
{
    struct stack *stack = *stackp;
    struct stack *output = *outputp;
    if (stack)
    {
        while (stack && cmp_op(current, top(stack)))
        {
            merge(&stack, &output);
        }
    }
    stack = push(stack, current);
    *stackp = stack;
    *outputp = output;
}

struct ast_node *shunting_yard(struct token **list)
{
    struct token **list_token = list;
    struct stack *stack = NULL;
    struct stack *output = NULL;
    while (*list_token)
    {
        struct token *current = *list_token;
        if (current->ast->type >= NOT && current->ast->type < LEFT_P)
        {
            operator_to_stack(&stack, &output, current);
        }
        else if (current->ast->type == LEFT_P)
        {
            stack = push(stack, current);
        }
        else if (current->ast->type == RIGHT_P)
        {
            while (stack && stack->data->ast->type != LEFT_P)
            {
                merge(&stack, &output);
            }
            if (!stack)
                errx(1, "myfind: Not finding parenthese\n");
            stack = pop(stack);
        }
        else
        {
            output = push(output, current);
        }
        list_token++;
    }
    while (stack)
    {
        merge(&stack, &output);
    }
    struct ast_node *ast = top(output)->ast;
    output = pop(output);
    free(output);
    free(stack);
    free_token_list(list);
    return ast;
}
