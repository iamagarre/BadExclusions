#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <errno.h>
#include <Windows.h>
#include "dirent.h"

#define MAX_PATH_LENGTH 1024

void createEicarFile(const char* dirPath);
void readEicarFiles(const char* dirPath);
void processListFile(const char* listFilePath);
void replaceDoubleBackslash(char* path);
void helpMessage();

//Create EICAR files
void createEicarFile(const char* dirPath) {
    DIR* dir;
    struct dirent* entry;
    char path[MAX_PATH_LENGTH];

    dir = opendir(dirPath);
    if (dir == NULL) {
        if (errno != ENOENT) {
            //fprintf(stderr, "Unable to open the directory %s: %s\n", dirPath, strerror(errno));
        }
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s\\%s", dirPath, entry->d_name);

            //Recursively create eicar.txt in subfolders
            createEicarFile(path); 

            snprintf(path, sizeof(path), "%s\\%s\\eicar.txt", dirPath, entry->d_name);

            FILE* fp = fopen(path, "w");
            if (fp == NULL) {
                if (errno != EACCES) {
                    //fprintf(stderr, "Unable to create %s file: %s\n", path, strerror(errno));
                }
                continue;
            }

            fprintf(fp, "X5O!P%%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*\n");

            fclose(fp);
        }
    }

    closedir(dir);
}

//Read EICAR file contents
void readEicarFiles(const char* dirPath) {
    DIR* dir;
    struct dirent* entry;
    char path[MAX_PATH_LENGTH];

    dir = opendir(dirPath);
    if (dir == NULL) {
        if (errno != ENOENT) {
            //fprintf(stderr, "Unable to open the directory %s: %s\n", dirPath, strerror(errno));
        }
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strcmp(entry->d_name, "eicar.txt") == 0) {
            snprintf(path, sizeof(path), "%s\\%s", dirPath, entry->d_name);

            FILE* fp = fopen(path, "r");
            if (fp == NULL) {
                if (errno != EACCES) {
                    //fprintf(stderr, "Unable to open %s file: %s\n", path, strerror(errno));
                }
                continue;
            }

            //Seek to the end of the file to get its size
            fseek(fp, 0L, SEEK_END);
            long fileSize = ftell(fp);
            fseek(fp, 0L, SEEK_SET);

            //Read the file contents and print them
            //+1 for null terminator
            char* fileContents = (char*)malloc(fileSize + 1); 
            fread(fileContents, 1, fileSize, fp);
            
            //null-terminate the string
            fileContents[fileSize] = '\0'; 

            size_t length = strlen(path);

            replaceDoubleBackslash(path);

            printf("%s\n", path);

            //Remember to free the allocated memory
            free(fileContents);

            fclose(fp);
        }
        else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s\\%s", dirPath, entry->d_name);
            
            //Recursively read eicar.txt files in subdirectories
            readEicarFiles(path); 
        }
    }

    closedir(dir);
}

//Create folders from list file
void processListFile(const char* listFilePath) {
    FILE* listFile = fopen(listFilePath, "r");
    if (listFile == NULL) {
        //fprintf(stderr, "Unable to open list file %s: %s\n", listFilePath, strerror(errno));
        exit(1);
    }

    char folderName[MAX_PATH_LENGTH];
    while (fgets(folderName, sizeof(folderName), listFile) != NULL) {
        //Remove newline characters
        size_t len = strlen(folderName);
        if (len > 0 && (folderName[len - 1] == '\n' || folderName[len - 1] == '\r')) {
            folderName[len - 1] = '\0';
        }

        if (_mkdir(folderName) != 0) {
            //fprintf(stderr, "Unable to create folder %s: %s\n", folderName, strerror(errno));
        }
    }

    fclose(listFile);
}

//Replace double backslash with a single backslash
void replaceDoubleBackslash(char* path) {
    for (int i = 0; path[i] != '\0'; i++) {
        if (path[i] == '\\' && path[i + 1] == '\\') {
            memmove(&path[i + 1], &path[i + 2], strlen(path) - i - 1);
        }
    }
}

//Help message
void helpMessage() {
    fprintf(stderr, "\nUsage: BadExclusions.exe -p <Path> -t <Timer> [-l <ListFile>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -p <Path>:     folder to check for exclusions. It will check all subfolders where you have write permissions.\n");
    fprintf(stderr, " -t <Timer>:    amount of time in seconds to wait between creating and reading EICAR files.\n");
    fprintf(stderr, " -l <ListFile>: specify a txt file that contains a list of folders to create.\n\n");
    fprintf(stderr, "BadExclusions v1.2\n\n");
}

int main(int argc, char* argv[]) {

    const char* path = argv[2];
    unsigned long timer = 0;
    const char* listFilePath = NULL;

    printf("\n__________             .______________             .__               .__                      \n");
    printf("\\______   \\_____     __| _/\\_   _____/__  ___ ____ |  |  __ __  _____|__| ____   ____   ______\n");
    printf(" |    |  _/\\__  \\   / __ |  |    __)_\\  \\/  // ___\\|  | |  |  \\/  ___/  |/  _ \\ /    \\ /  ___/\n");
    printf(" |    |   \\ / __ \\_/ /_/ |  |        \\>    <\\  \\___|  |_|  |  /\\___ \\|  (  <_> )   |  \\\\___ \\ \n");
    printf(" |______  /(____  /\\____ | /_______  /__/\\_ \\\\___  >____/____//____  >__|\\____/|___|  /____  >\n");
    printf("        \\/      \\/      \\/         \\/      \\/    \\/                \\/               \\/     \\/ \n");

    //Parse command-line arguments
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc) {
            if (strcmp(argv[i], "-p") == 0) {
                path = argv[i + 1];
            }
            else if (strcmp(argv[i], "-t") == 0) {
                char* endptr;
                timer = strtoul(argv[i + 1], &endptr, 10);

                if (*endptr != '\0') {
                    helpMessage();
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-l") == 0) {
                listFilePath = argv[i + 1];
            }
            else {
                helpMessage();
                return 1;
            }
        }
        else {
            helpMessage();
            return 1;
        }
    }

    if (path == NULL || (timer == 0 && listFilePath == NULL)) {
        helpMessage();
        return 1;
    }

    if (listFilePath != NULL) {
        printf("\n[+] Creating folders from list.\n");
        processListFile(listFilePath);
    }

    printf("\n[+] Creating EICAR files.\n");
    createEicarFile(path);

    printf("\n[+] Waiting %lu seconds.\n", timer);
    Sleep(timer * 1000);

    printf("\n[+] EICAR files found:\n\n");
    readEicarFiles(path);

    printf("\n[+] Done!\n\n");

    return 0;
}
