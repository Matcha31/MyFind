#ifndef EXPRTYPES_H
#define EXPRTYPES_H

enum expr_type
{
    PRINT = 0,
    NAME,
    TYPE,
    NEWER,
    PERM,
    USER,
    GROUP,

    NOT = 100,
    AND,
    OR,
    LEFT_P,
    RIGHT_P
};

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fnmatch.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif /* !EXPRTYPES_H */
