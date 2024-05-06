#ifndef STACK_H
#define STACK_H

struct stack
{
    struct token *data;
    struct stack *next;
};

struct stack *push(struct stack *s, struct token *e);
struct stack *pop(struct stack *s);
struct token *top(struct stack *s);

#endif /* !STACK_H */
