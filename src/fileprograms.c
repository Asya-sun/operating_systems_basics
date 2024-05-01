#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024

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
    int fd2 = open(path, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG | S_IROTH);
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
    printf("mode: %d\n", stat_info.st_mode);
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


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enough args");
        exit(EXIT_FAILURE);
    }
    if (argc == 3) {
        if (strcmp(argv[1], "file_contents") == 0) {
            file_contents(argv[2]);
        } else if (strcmp(argv[1], "make_dir") == 0) {
            make_dir(argv[2]);
        } else if (strcmp(argv[1], "make_file") == 0) {
            make_file(argv[2]);
        } else if (strcmp(argv[1], "rm_dir") == 0) {
            rm_dir(argv[2]);
        } else if (strcmp(argv[1], "rm_file") == 0) {
            rm_file(argv[2]);
        } else if (strcmp(argv[1], "dir_contents") == 0) {
            dir_contents(argv[2]);
        } else if (strcmp(argv[1], "rm_symlink") == 0) {
            dir_contents(argv[2]);
        } else if (strcmp(argv[1], "rm_hardlink") == 0) {
            rm_file(argv[2]);
        } else if (strcmp(argv[1], "know_mod") == 0) {
            know_mod(argv[2]);
        } else if (strcmp(argv[1], "symlink_contents") == 0) {
            //file_contents(argv[2]);
            symlink_contents(argv[2]);
        } else if (strcmp(argv[1], "real_symlink_contents") == 0) {
            read_by_symlink(argv[2]);
        }
    }

    if (argc == 4) {
        if (strcmp(argv[1], "make_symlink") == 0) {
            make_symlink(argv[2], argv[3]);
        } else if (strcmp(argv[1], "hard_link") == 0) {
            hard_link(argv[2], argv[3]);
        } else if (strcmp(argv[1], "change_mod") == 0) {
            change_mod(argv[2], strtoll( argv[3], NULL, 8));
        }
    }

    printf("\n");
    printf("\n");
    return 0;
}
