#include "treasure.h"
#include "customs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdbool.h>

void abandonCSTM(){
    char temp[ERROR_BUFFER_SIZE];
    sprintf(temp, "%s", strerror(errno));
    if (write(STDERR_FILENO, temp, sizeof(temp)) == 0){
        ;
    }
    exit(EXIT_FAILURE);
}

DIR* openDirectoryCSTM(char *DirName){
    DIR *dirID = NULL;
    if ((dirID = opendir(DirName)) == NULL){
        abandonCSTM();
    }
    return dirID;
}

void closeDirectoryCSTM(DIR* DirID){
    if (closedir(DirID) == -1){
        abandonCSTM();
    }
}

void createDirectoryCSTM(char *DirName){
    DIR *DirID = NULL;
    if ((DirID = opendir(DirName)) == NULL){
        if (errno == ENOENT){
            int check = 0;
            if ((check = mkdir(DirName, 0775)) == -1)
            {
                abandonCSTM();
            }
        }
    }
    else {
        closeDirectoryCSTM(DirID);
    }
}

/*bool runThroughCheckDirCSTM(char *path, char *Name)
{
    DIR *DirID;
    struct dirent *in;
    char *nume;  
    DirID = openDirectoryCSTM(path);
    while((in = readdir(DirID))>0)
    {
        nume = in->d_name;
        if(strcmp(nume, ".") == 0 || strcmp(nume, "..")==0){
           continue;
        }
        if (strcmp(Name,nume) == 0){
            return true;
        }
    }
    closeDirectoryCSTM(DirID);
    return false;
}*/

bool runThroughCheckDirCSTM(char *DirName)
{
    DIR *DirID = NULL;
    if ((DirID = opendir(DirName)) == NULL){
        if (errno == ENOENT){
            return false;
        }
    }
    else {
        closeDirectoryCSTM(DirID);
        return true;
    }
    return false;
}