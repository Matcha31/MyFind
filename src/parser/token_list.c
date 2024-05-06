#include <string.h>

#include "./parser.h"
#include "./stack/stack.h"

struct function functions[] = { {
                                    .type = PRINT,
                                    .name = "print",
                                    .fun = print_fun,
                                },
                                {
                                    .type = NAME,
                                    .name = "name",
                                    .fun = name_fun,
                                },
                                {
                                    .type = TYPE,
                                    .name = "type",
                                    .fun = type_fun,
                                },
                                {
                                    .type = NEWER,
                                    .name = "newer",
                                    .fun = newer_fun,
                                },
                                {
                                    .type = PERM,
                                    .name = "perm",
                                    .fun = perm_fun,
                                },
                                {
                                    .type = USER,
                                    .name = "user",
                                    .fun = user_fun,
                                },
                                {
                                    .type = GROUP,
                                    .name = "group",
                                    .fun = group_fun,
                                } };

int is_actions(char **args)
{
    int i = 0;
    while (*args)
    {
        i++;
        if (!strcmp(*args, "-print"))
            return 1;
        args++;
    }
    args -= i;
    return 0;
}

int nb_tokens(char **args)
{
    int n = 0;
    int i = 0;
    while (*args)
    {
        if (**args == '-')
            n++;
        args++;
        i++;
    }
    args -= i;
    return n;
}

int is_operator(char *str)
{
    if (!strcmp(str, "!") || !strcmp(str, "-a") || !strcmp(str, "-o"))
        return 1;
    return 0;
}

enum expr_type type_token(char *str)
{
    if (!strcmp(str, "-print"))
        return PRINT;
    if (!strcmp(str, "-name"))
        return NAME;
    if (!strcmp(str, "-type"))
        return TYPE;
    if (!strcmp(str, "-newer"))
        return NEWER;
    if (!strcmp(str, "-perm"))
        return PERM;
    if (!strcmp(str, "-user"))
        return USER;
    if (!strcmp(str, "-group"))
        return GROUP;
    if (!strcmp(str, "!"))
        return NOT;
    if (!strcmp(str, "-a"))
        return AND;
    if (!strcmp(str, "-o"))
        return OR;
    if (!strcmp(str, "("))
    {
        return LEFT_P;
    }
    if (!strcmp(str, ")"))
        return RIGHT_P;
    errx(1, "myfind: invalid argument: %s\n", str);
}

char *token_arg(enum expr_type type, char **args, int *j)
{
    if (type < NAME || type > GROUP)
    {
        char *arg = malloc(sizeof(char));
        arg[0] = '\0';
        *j = 1;
        return arg;
    }
    args++;
    if (!*args || !strcmp(*args, "(") || !strcmp(*args, ")") || **args == '!')
    {
        fprintf(stderr, "myfind: syntax error");
        exit(1);
    }
    char *arg = calloc(1, sizeof(char) * (strlen(*args) + 1));
    strcat(arg, *args);
    if (!is_valid_arg(type, arg))
        errx(1, "myfind: invald argument: %s", arg);
    args++;
    *j = 2;
    return arg;
}

struct function *token_fun(enum expr_type type)
{
    if (type > GROUP)
        return NULL;
    struct function *fun = malloc(sizeof(struct function));
    for (int i = 0; i < 7; i++)
    {
        if (type == functions[i].type)
        {
            fun->type = functions[i].type;
            fun->name = functions[i].name;
            fun->fun = functions[i].fun;
        }
    }
    return fun;
}

int prio_token(enum expr_type type)
{
    if (type == OR)
        return 10;
    if (type == AND)
        return 11;
    if (type == NOT)
        return 13;
    return 0;
}

enum associativity asso_token(enum expr_type type)
{
    if (type == NOT)
        return RIGHT;
    if (type > NOT)
        return LEFT;
    return NONE;
}

struct token *create_token(char *str, char **args, int *j)
{
    struct token *tok = malloc(sizeof(struct token));
    struct ast_node *ast = malloc(sizeof(struct ast_node));
    tok->ast = ast;
    tok->ast->type = type_token(str);
    tok->ast->left_child = NULL;
    tok->ast->right_child = NULL;
    tok->ast->arg = token_arg(tok->ast->type, args, j);
    tok->ast->fun = token_fun(tok->ast->type);
    tok->prio = prio_token(tok->ast->type);
    tok->asso = asso_token(tok->ast->type);
    return tok;
}

struct token **list_token(char **args, int argc)
{
    int actions = is_actions(args);
    struct token **list = calloc(1, sizeof(struct token) * (argc + 2));
    struct token **r = list;
    int last_test = 0;
    int j = 0;
    char *current;
    while ((current = *args) != NULL)
    {
        if (is_operator(current) || !strcmp(current, "(")
            || !strcmp(current, ")"))
        {
            *list = create_token(current, args, &j);
            list++;
            args += j;
            last_test = 0;
        }
        else if (last_test)
        {
            *list = create_token("-a", args, &j);
            list++;
            last_test = 0;
        }
        else
        {
            last_test = 1;
            *list = create_token(current, args, &j);
            list++;
            args += j;
        }
    }
    j = -1;
    if (!actions)
    {
        *list = create_token("-a", args, &j);
        (*list)->prio = 1;
        list++;
        *list = create_token("-print", args, &j);
        list++;
    }
    *list = NULL;
    return r;
}
