#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

void copy_file_with_reverse(char *src_path, const char *dst_path) {
    FILE *src = fopen(src_path, "rb+");
    FILE *dest = fopen(dst_path, "rb+");
    if (src == NULL) {
        printf("for src %s\n", src_path);
        perror("error opening src");
        return;
    }
    if (dest == NULL) {
        printf("for dst %s\n", dst_path);
        perror("error opening dst");
        return;
    }

    // Определяем размер файла
    fseek(src, 0, SEEK_END);
    long fileSize = ftell(src);
    fseek(src, 0, SEEK_SET);

    char buffer[1024];

    for (long i = 0; i < fileSize / 2; i += 1024) {
        fseek(src, i, SEEK_SET);
        size_t bytesRead = fread(buffer, 1, 1024, src);

        for (size_t j = 0; j < bytesRead / 2; j++) {
            char temp = buffer[j];
            buffer[j] = buffer[bytesRead - j - 1];
            buffer[bytesRead - j - 1] = temp;
        }

        fseek(src, i, SEEK_SET);
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);
}

char* make_file_path(char* path, char* file_name) {
    char *res = (char*) malloc(strlen(path) + strlen(file_name) + 2);
    if (res == NULL) {
        perror("error while creating a path to a file");
        exit(1);
    }
    strcpy(res, path);
    res[strlen(path)] = '/';
    strcpy(res + strlen(path) + 1, file_name);
    return res;
}

void reverse_name(char* str){
    int len = strlen(str);
    char* start = str;
    char* end = str + len - 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reserve_directory(char* path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    if(dir == NULL) {
        perror("diropen");
        exit(1);
    }


    int len = strlen(path) - 1;
    int count = len;
    int flag = 0;
    while (count >=0) {
        if (path[count] == '/') {
            flag = 1;
            break;
        }
        count--;
    }
    char* reversed_dir_name = (char*) malloc(len - count );
    if (reversed_dir_name == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int i = len; i >= count; --i) {
        reversed_dir_name[len - i] = path[i];
    }

    if (mkdir(reversed_dir_name, S_IRWXU | S_IRWXG | S_IROTH) != 0) {
        perror("mkdir");
        exit(0);
    }


    char current_path[512];
    getcwd(current_path, 512);
    char* reverse_path = make_file_path(current_path, reversed_dir_name);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char* file_path = make_file_path(path, entry->d_name);
            struct stat stat_info;
            stat(file_path, &stat_info);
            int fd1 = open(file_path, O_RDWR, stat_info.st_mode);
            if (fd1 == -1){
                printf("can't open file + %s\n", file_path);
                free(file_path);
                continue;
            }
            char* reverse_file_name = (char*) malloc(strlen(entry->d_name) + 1);
            strcpy(reverse_file_name, entry->d_name);
            reverse_name(reverse_file_name);
            char* reverse_file_path = make_file_path(reverse_path,reverse_file_name);
            int fd2 = open(reverse_file_path, O_CREAT | O_WRONLY, stat_info.st_mode);
            if (fd2 == -1) {
                printf("enable to create a file + %s\n", reverse_file_path);
                perror("open fd2");
                free(file_path);
                free(reverse_file_name);
                continue;
            }
            copy_file_with_reverse(file_path, reverse_file_path);

            close(fd1);
            close(fd2);
            free(reverse_file_name);
            free(file_path);
        }
    }
    free(reverse_path);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("not enough args");
        exit(EXIT_FAILURE);
    }
    reserve_directory(argv[1]);

    return 0;
}
