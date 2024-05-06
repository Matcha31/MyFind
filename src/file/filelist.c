#include "./file.h"

char get_type(char *path)
{
    struct stat filestat;
    if (lstat(path, &filestat) < 0)
    {
        exit(1);
        fprintf(stderr, "myfind: Could not access the file %s stat", path);
        return -1;
    }
    if (S_ISDIR(filestat.st_mode))
        return 'd';
    if (S_ISLNK(filestat.st_mode))
        return 'l';
    if (S_ISREG(filestat.st_mode))
        return 'f';
    if (S_ISBLK(filestat.st_mode))
        return 'b';
    if (S_ISCHR(filestat.st_mode))
        return 'c';
    if (S_ISFIFO(filestat.st_mode))
        return 'p';
    return 's';
}

int get_perm(char *path)
{
    int tmp = 0;
    int perm = 0;
    struct stat filestat;
    if (lstat(path, &filestat) < 0)
    {
        exit(1);
        fprintf(stderr, "myfind: Could not access the file %s stat", path);
        return -1;
    }
    if ((filestat.st_mode & S_IXUSR))
        tmp += 1;
    if ((filestat.st_mode & S_IWUSR))
        tmp += 2;
    if ((filestat.st_mode & S_IRUSR))
        tmp += 4;
    perm += tmp;
    perm = perm << 3;
    tmp = 0;
    if ((filestat.st_mode & S_IXGRP))
        tmp += 1;
    if ((filestat.st_mode & S_IWGRP))
        tmp += 2;
    if ((filestat.st_mode & S_IRGRP))
        tmp += 4;
    perm += tmp;
    perm = perm << 3;
    tmp = 0;
    if ((filestat.st_mode & S_IXOTH))
        tmp += 1;
    if ((filestat.st_mode & S_IWOTH))
        tmp += 2;
    if ((filestat.st_mode & S_IROTH))
        tmp += 4;
    perm += tmp;
    return perm;
}

char *get_user(char *path)
{
    struct stat filestat;
    if (lstat(path, &filestat) < 0)
        errx(1, "myfind: Could not access the file %s stat", path);
    struct passwd *user = getpwuid(filestat.st_uid);
    return user->pw_name;
}

char *get_group(char *path)
{
    struct stat filestat;
    if (lstat(path, &filestat) < 0)
        errx(1, "myfind: Could not access the file %s stat", path);
    struct group *group = getgrgid(filestat.st_gid);
    return group->gr_name;
}

int nb_files(char *entry_point)
{
    DIR *dir = opendir(entry_point);
    struct dirent *current = readdir(dir);
    int n = 1;
    while (current)
    {
        if (strcmp(current->d_name, ".") && strcmp(current->d_name, ".."))
        {
            char *path = calloc(
                1,
                sizeof(char)
                    * (2 + strlen(current->d_name) + strlen(entry_point)));
            path = strcat(path, entry_point);
            path[strlen(entry_point)] = '/';
            path = strcat(path, current->d_name);
            if (get_type(path) == 'd')
            {
                n += nb_files(path);
            }
            else
            {
                n++;
            }
            free(path);
        }
        current = readdir(dir);
    }
    closedir(dir);
    return n;
}

void free_list_file(struct file **list)
{
    int i = 0;
    while (list[i])
    {
        free(list[i]->path);
        free(list[i]);
        ++i;
    }
    free(list);
}

void concat_list(struct file **list1, struct file **list2, int *i)
{
    int j = 0;
    while (list2[j])
    {
        list1[(*i)++] = list2[j++];
    }
    free(list2);
}

struct file *create_file(char *path)
{
    struct file *file = malloc(sizeof(struct file));
    file->type = get_type(path);
    file->path = path;
    file->name = basename(path);
    file->perm = get_perm(path);
    file->user = get_user(path);
    file->group = get_group(path);
    return file;
}

struct file **list_files(char *dir_path)
{
    int n = nb_files(dir_path);
    struct file **list = malloc(sizeof(struct file) * (n + 1));
    list[0] = create_file(dir_path);
    DIR *dir = opendir(dir_path);
    struct dirent *current = readdir(dir);
    int i = 1;
    while (current)
    {
        if (strcmp(current->d_name, ".") && strcmp(current->d_name, ".."))
        {
            char *path =
                calloc(1,
                       sizeof(char)
                           * (2 + strlen(current->d_name) + strlen(dir_path)));
            path = strcat(path, dir_path);
            path[strlen(dir_path)] = '/';
            path = strcat(path, current->d_name);
            if (get_type(path) == 'd')
            {
                concat_list(list, list_files(path), &i);
            }
            else
            {
                list[i++] = create_file(path);
            }
        }
        current = readdir(dir);
    }
    list[i] = NULL;
    closedir(dir);
    return list;
}
