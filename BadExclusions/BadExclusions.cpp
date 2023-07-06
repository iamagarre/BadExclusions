#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dirent.h"

void createEicarFile(const char* dir_path)
{
    DIR* dir;
    struct dirent* entry;
    char path[1024];

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        if (errno != ENOENT)
        {
            //printf("Unable to open the directory: %s\n", strerror(errno));
        }
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            snprintf(path, sizeof(path), "%s\\%s", dir_path, entry->d_name);

            createEicarFile(path); // Recursively create eicar.txt in subfolders

            snprintf(path, sizeof(path), "%s\\%s\\eicar.txt", dir_path, entry->d_name);

            FILE* fp = fopen(path, "w");
            if (fp == NULL)
            {
                if (errno != EACCES)
                {
                    //printf("Unable to create %s file: %s\n", path, strerror(errno));
                }
                continue;
            }
                         
            fprintf(fp, "X5O!P%%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*\n");

            fclose(fp);
        }
    }

    closedir(dir);
}

void readEicarFiles(const char* dir_path)
{
    DIR* dir;
    struct dirent* entry;
    char path[1024];

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        if (errno != ENOENT)
        {
            // printf("Unable to open the directory: %s\n", strerror(errno));
        }
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strcmp(entry->d_name, "eicar.txt") == 0)
        {
            snprintf(path, sizeof(path), "%s\\%s", dir_path, entry->d_name);

            FILE* fp = fopen(path, "r");
            if (fp == NULL)
            {
                if (errno != EACCES)
                {
                    // printf("Unable to open %s file: %s\n", path, strerror(errno));
                }
                continue;
            }

            // Seek to the end of the file to get its size
            fseek(fp, 0L, SEEK_END);
            long file_size = ftell(fp);
            fseek(fp, 0L, SEEK_SET);

            // Read the file contents and print them
            char* file_contents = (char*)malloc(file_size + 1); // +1 for null terminator
            fread(file_contents, 1, file_size, fp);
            file_contents[file_size] = '\0'; // null-terminate the string

            printf("%s\n", path);

            // Remember to free the allocated memory
            free(file_contents);

            fclose(fp);
        }
        else if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            snprintf(path, sizeof(path), "%s\\%s", dir_path, entry->d_name);

            readEicarFiles(path); // Recursively read eicar.txt files in subdirectories
        }
    }

    closedir(dir);
}

int main()
{
    const char* dir_path = "C:\\";

    //ASCII art
    printf("__________             .______________             .__               .__                      \n");
    printf("\\______   \\_____     __| _/\\_   _____/__  ___ ____ |  |  __ __  _____|__| ____   ____   ______\n");
    printf(" |    |  _/\\__  \\   / __ |  |    __)_\\  \\/  // ___\\|  | |  |  \\/  ___/  |/  _ \\ /    \\ /  ___/\n");
    printf(" |    |   \\ / __ \\_/ /_/ |  |        \\>    <\\  \\___|  |_|  |  /\\___ \\|  (  <_> )   |  \\\\___ \\ \n");
    printf(" |______  /(____  /\\____ | /_______  /__/\\_ \\\\___  >____/____//____  >__|\\____/|___|  /____  >\n");
    printf("        \\/      \\/      \\/         \\/      \\/    \\/                \\/               \\/     \\/ \n");



    printf("\n[+] Creating EICAR files...\n");
    createEicarFile(dir_path);

    Sleep(5000);

    printf("\n[+] EICAR files found at...\n");
    readEicarFiles(dir_path);

    printf("\n[+] Done!\n");
    return 0;
}