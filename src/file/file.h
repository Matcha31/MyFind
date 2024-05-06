#ifndef FILETREE_H
#define FILETREE_H

#define _POSIX_C_SOURCE 200112L

#include "../exprtypes.h"

struct file
{
    char type;
    char *path;
    char *name;
    int perm;
    char *user;
    char *group;
};

struct file **list_files(char *path);
void free_list_file(struct file **list);

#endif /* !FILETREE_H */
