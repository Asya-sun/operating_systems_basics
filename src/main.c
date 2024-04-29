#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#define NAME_LENGTH 256
#define BUF_SIZE 1024

void reverse_file(int src, int dst,  struct stat st) {
    lseek(src, 0, SEEK_END);
    // Определяем размер файла
    long fileSize = st.st_size;
    lseek(src, 0, SEEK_SET);

    char buffer[BUF_SIZE];
    for (long i = 0; i < fileSize / 2; i += BUF_SIZE) {
        lseek(src, i, SEEK_SET);
        size_t bytesRead = read(src, buffer, BUF_SIZE);

        for (size_t j = 0; j < bytesRead / 2; j++) {
            char temp = buffer[j];
            buffer[j] = buffer[bytesRead - j - 1];
            buffer[bytesRead - j - 1] = temp;
        }

        lseek(src, i, SEEK_SET);
        write(dst, buffer, bytesRead);
    }
}

void make_new_file_path(char res[], char path[], char file_name[]) {
    strcpy(res, path);
    res[strlen(path)] = '/';
    strcpy(res + strlen(path) + 1, file_name);
    return;
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

void reverse_directory(char* path, char* already_rev) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if(dir == NULL) {
        perror("diropen");
        exit(1);
    }

    int len = strlen(path);
    int count = len;
    for (int i = 1; i <= len && (path[len - i] != '/'); ++i) {
        count = i;
    }
    if (count == len) {
        count = -1;
    }

    char reversed_dir_name[NAME_LENGTH];
    for (int i = 0; i < NAME_LENGTH; ++i) {reversed_dir_name[i] = '\0';}
    for (int i = len; i > len - count; --i) {
        reversed_dir_name[len - i] = path[i - 1];
    }

    char whole_reversed_dir_name[NAME_LENGTH];
    if (already_rev != NULL) {
        make_new_file_path(whole_reversed_dir_name, already_rev, reversed_dir_name);
    } else {
        strcpy(whole_reversed_dir_name, reversed_dir_name);
    }

    if (mkdir(whole_reversed_dir_name, S_IRWXU | S_IRWXG | S_IROTH) != 0) {
        perror("mkdir");
        return;
    }

    char current_path[512];
    getcwd(current_path, 512);


    char reverse_path[NAME_LENGTH];
    make_new_file_path(reverse_path, current_path, whole_reversed_dir_name);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
                char file_path[NAME_LENGTH];
                make_new_file_path(file_path, path, entry->d_name);
                reverse_directory(file_path, whole_reversed_dir_name);
            }
            continue;
        } else if (entry->d_type == DT_REG) {
            char file_path[NAME_LENGTH];
            make_new_file_path(file_path, path, entry->d_name);

            struct stat stat_info;
            stat(file_path, &stat_info);
            int fd1 = open(file_path, O_RDWR);
            if (fd1 == -1){
                printf("can't open file + %s\n", file_path);
                continue;
            }


            char reverse_file_name[NAME_LENGTH];
            for (int i = 0; i < NAME_LENGTH; ++i) { reverse_file_name[i] = '\0';}

            strcpy(reverse_file_name, entry->d_name);
            reverse_name(reverse_file_name);

            char reverse_file_path[NAME_LENGTH];
            make_new_file_path(reverse_file_path, reverse_path, reverse_file_name);

            int fd2 = open(reverse_file_path, O_CREAT | O_WRONLY, stat_info.st_mode);
            if (fd2 == -1) {
                printf("enable to create a file + %s\n", reverse_file_path);
                perror("open fd2");
                continue;
            }

            reverse_file(fd1, fd2, stat_info);
            //copy_file_with_reverse(file_path, reverse_file_path);

            close(fd2);
            close(fd1);
        }
    }
    closedir(dir);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("not enough args");
        exit(EXIT_FAILURE);
    }
    reverse_directory(argv[1], NULL);
    return 0;
}
