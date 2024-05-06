#include "./myfind.h"

#include <stdio.h>
#include <string.h>

char **args_list(int argc, char **argv, struct myfind *myfind)
{
    int j = 1;
    int n = argc + 1;
    if (argc > 1)
    {
        if (*argv[1] == '-' || *argv[1] == '!' || !strcmp(argv[1], "("))
        {
            myfind->entry_point = ".";
            n = argc;
        }
        else
        {
            myfind->entry_point = argv[1];
            j++;
            n = argc - 1;
            if (argc == 2)
                n++;
        }
    }
    char **args = malloc(sizeof(char *) * n);
    if (argc == 1)
    {
        myfind->entry_point = ".";
        args[0] = "-print";
    }
    else if (argc == 2 && (*argv[1] != '-'))
    {
        args[0] = "-print";
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
            args[i] = argv[i + j];
    }
    args[n - 1] = NULL;
    return args;
}

int main(int argc, char **argv)
{
    struct myfind *myfind = malloc(sizeof(struct myfind));
    char **args = args_list(argc, argv, myfind);
    struct token **token_list = list_token(args, argc);
    myfind->ast = shunting_yard(token_list);
    free(args);
    char *dir_path =
        calloc(1, sizeof(char) * (strlen(myfind->entry_point) + 1));
    dir_path = strcat(dir_path, myfind->entry_point);
    struct file **list_file = list_files(dir_path);
    int i = 0;
    while (list_file[i])
    {
        evaluate(myfind->ast, list_file[i]);
        i++;
    }
    free_list_file(list_file);
    free_ast(myfind->ast);
    free(myfind);
    return 0;
}
