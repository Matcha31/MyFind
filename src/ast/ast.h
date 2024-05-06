#ifndef AST_H
#define AST_H

#include "../exprtypes.h"
#include "../file/file.h"

struct function
{
    enum expr_type type;
    char *name;
    bool (*fun)(struct file *file, char *arg);
};

struct ast_node
{
    enum expr_type type;
    struct ast_node *left_child;
    struct ast_node *right_child;
    char *arg;
    struct function *fun;
};

void free_ast(struct ast_node *ast);
bool evaluate(struct ast_node *node, struct file *file);

bool print_fun(struct file *file, char *arg);
bool delete_fun(struct file *file, char *arg);

bool name_fun(struct file *file, char *arg);
bool type_fun(struct file *file, char *arg);
bool newer_fun(struct file *file, char *arg);
bool perm_fun(struct file *file, char *arg);
bool user_fun(struct file *file, char *arg);
bool group_fun(struct file *file, char *arg);

#endif /* !AST_H */
