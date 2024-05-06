#include <err.h>
#include <stdio.h>

#include "./ast.h"

bool print_fun(struct file *file, char *arg)
{
    if (!file->name)
    {
        err(1, "Should not happen\n");
    }
    printf("%s\n", file->path);
    arg++;
    return true;
}

bool name_fun(struct file *file, char *arg)
{
    return !fnmatch(arg, file->name, 0);
}

bool is_valdid_type(char t)
{
    if (t == 'd')
        return true;
    if (t == 'f')
        return true;
    if (t == 'l')
        return true;
    if (t == 's')
        return true;
    if (t == 'c')
        return true;
    if (t == 'b')
        return true;
    if (t == 'p')
        return true;
    return false;
}

bool type_fun(struct file *file, char *arg)
{
    while (*arg)
    {
        if (*arg != ',')
        {
            if (file->type == *arg)
                return true;
        }
        arg++;
    }
    return false;
}

bool newer_fun(struct file *file, char *arg)
{
    if (file->path && arg)
        return true;
    return false;
}

bool perm_fun(struct file *file, char *arg)
{
    char c = '\0';
    if (strlen(arg) == 4)
        c = *(arg++);
    int perm = 0;
    perm += *(arg++) - '0';
    perm = perm << 3;
    perm += *(arg++) - '0';
    perm = perm << 3;
    perm += *(arg++) - '0';
    arg -= 3;
    if (strlen(arg) == 3 && c == '\0')
    {
        if (file->perm == perm)
            return true;
        return false;
    }
    if (c == '-')
    {
        return (file->perm & perm) == perm;
    }
    return file->perm & perm;
}

bool user_fun(struct file *file, char *arg)
{
    if (!strcmp(file->user, arg))
        return true;
    return false;
}

bool group_fun(struct file *file, char *arg)
{
    if (!strcmp(file->group, arg))
        return true;
    return false;
}
