#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"
#include <errno.h>

void createEicarFile(const char* dir_path);
void checkEicarFiles(const char* dir_path);

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

    printf("[+] EICAR files found at...\n");
    checkEicarFiles(dir_path);

    printf("[+] Done!");
    return 0;
}

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

void checkEicarFiles(const char* dir_path)
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
        if (entry->d_type == DT_REG && strcmp(entry->d_name, "eicar.txt") == 0)
        {
            snprintf(path, sizeof(path), "%s\\%s", dir_path, entry->d_name);

            // Replace "\\" with "\"
            char* ptr = path;
            while (*ptr != '\0')
            {
                if (*ptr == '\\' && *(ptr + 1) == '\\')
                {
                    memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
                }
                ptr++;
            }

            printf("%s\n", path);
        }
        else if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            snprintf(path, sizeof(path), "%s\\%s", dir_path, entry->d_name);

            checkEicarFiles(path); // Recursively check subfolders
        }
    }

    closedir(dir);
}
