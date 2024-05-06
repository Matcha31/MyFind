#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

#include "../parser.h"

struct stack *push(struct stack *s, struct token *e)
{
    struct stack *st = malloc(sizeof(struct stack));
    st->data = e;
    st->next = s;
    return st;
}

struct stack *pop(struct stack *s)
{
    if (s == NULL)
        return NULL;
    struct stack *tmp = s->next;
    free(s);
    return tmp;
}

struct token *top(struct stack *s)
{
    return s->data;
}
