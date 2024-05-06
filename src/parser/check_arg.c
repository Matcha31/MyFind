#include "./parser.h"

bool type_arg(char *arg)
{
    while (*arg)
    {
        if (!strchr("fdslpbc", *arg))
            errx(1, "myfind: invalid type: %s", arg);
        arg++;
        if (!*arg)
            break;
        if (*arg != ',' || !*(arg + 1))
            errx(1, "myfind: invalid type: %s", arg);
        arg++;
    }
    return true;
}

bool perm_arg(char *arg)
{
    if (strlen(arg) > 4 || strlen(arg) < 3)
        return false;
    if (strlen(arg) == 4)
    {
        if (*arg == '-' || *arg == '/')
            arg++;
        else
            return false;
    }
    for (int i = 0; i < 3; i++)
    {
        if (*arg < '0' || *arg > '7')
        {
            return false;
        }
        arg++;
    }
    return true;
}

bool is_valid_arg(enum expr_type type, char *arg)
{
    switch (type)
    {
    case NAME:
        return true;
    case TYPE:
        return type_arg(arg);
    case PERM:
        return perm_arg(arg);
    default:
        return true;
    }
}
