#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define NAME_LENGTH 256

void file_contents(char path[]) {
    char line[32];
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("error: cant open file %s\n", path);
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
}



void make_dir(char path[]) {
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH) != 0) {
        perror("mkdir");
    }
}

void make_file(char path[]) {
    int fd2 = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG | S_IROTH);
    if (fd2 == -1) {
        printf("enable to create a file + %s\n", path);
        perror("open fd2");
    }
    close(fd2);
}

void make_symlink(char src[], char dest[]) {
    if (symlink(src, dest) != 0) {
        perror("symlink");
    }
}

void hard_link(char src[], char dest[]) {
    if (link(src, dest) != 0) {
        perror("link");
    }
}

void know_mod(char path[]) {
    struct stat stat_info;
    if (stat(path, &stat_info) != 0) {
        perror("stat");
        return;
    }
    printf("mode: 0%o\n", stat_info.st_mode & 0777);
    printf("number of hard links %ld\n", stat_info.st_nlink);
}

void change_mod(char path[], mode_t new_mode) {
    if (chmod(path, new_mode) != 0) {
        perror("chmod");
    }
}


void rm_dir(char path[]) {
    if (rmdir(path)) {
        perror("rmdir");
    }
}

void rm_file(char path[]) {
    if (remove(path) != 0 ) {
        perror("remove");
    }
}


void dir_contents(char path[]) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    if(dir == NULL) {
        perror("diropen");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}



void read_by_symlink(char path[]) {
    char buffer[BUF_SIZE];
    int name_len = (int) readlink(path, buffer, BUF_SIZE);
    if (name_len == -1) {
        perror("readlink");
        return;
    }
    buffer[name_len] = '\0';
    file_contents(buffer);
}


void symlink_contents(char* filePath) {
    char buffer[BUF_SIZE];
    int name_len = (int) readlink(filePath, buffer, BUF_SIZE);
    if (name_len == -1) {
        perror("readlink");
        return;
    }
    buffer[name_len] = '\n';
    int bytes_written = (int) write(1, buffer, name_len + 1);

    if(bytes_written != name_len + 1) {
        perror("write");
        return;
    }
}


void convert_func_name(char *src, char *dest) {
    int len = strlen(src);
    int count = len;
    for (int i = 1; i <= len && (src[len - i] != '/'); ++i) {
        count = i;
    }
    for (int i = 0; i < NAME_LENGTH; ++i) {dest[i] = '\0';}
    for (int i = len - count; i < len; i++) {
        dest[i - (len - count)] = src[i];
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enough args");
        exit(EXIT_FAILURE);
    }
    char file_name[NAME_LENGTH];
    convert_func_name(argv[0], file_name);


    if (argc == 2) {
        if (strcmp(file_name, "file_contents") == 0) {
            file_contents(argv[1]);
        } else if (strcmp(file_name, "make_dir") == 0) {
            make_dir(argv[1]);
        } else if (strcmp(file_name, "make_file") == 0) {
            make_file(argv[1]);
        } else if (strcmp(file_name, "rm_dir") == 0) {
            rm_dir(argv[1]);
        } else if (strcmp(file_name, "rm_file") == 0) {
            rm_file(argv[1]);
        } else if (strcmp(file_name, "dir_contents") == 0) {
            dir_contents(argv[1]);
        } else if (strcmp(file_name, "rm_symlink") == 0) {
            rm_file(argv[1]);
        } else if (strcmp(file_name, "rm_hardlink") == 0) {
            rm_file(argv[1]);
        } else if (strcmp(file_name, "know_mode") == 0) {
            know_mod(argv[1]);
        } else if (strcmp(file_name, "symlink_contents") == 0) {
            symlink_contents(argv[1]);
        } else if (strcmp(file_name, "real_symlink_contents") == 0) {
            read_by_symlink(argv[1]);
        }
        return 0;
    }

    if (argc == 3) {
        if (strcmp(file_name, "make_symlink") == 0) {
            make_symlink(argv[1], argv[2]);
        } else if (strcmp(file_name, "hard_link") == 0) {
            hard_link(argv[1], argv[2]);
        } else if (strcmp(file_name, "change_mode") == 0) {
            change_mod(argv[1], strtoll( argv[2], NULL, 8));
        }
    }
    printf("\n");
    return 0;
}
