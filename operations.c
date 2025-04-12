#include "treasure.h"
#include "operations.h"
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
#include <time.h>

#define TIMESTAMP_SIZE 30

void log_operation(const char *hunt_id, const char *operation, const char *details) {
    char cale[PATH_MAX];
    char log_entry[LONG_TEXT];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[TIMESTAMP_SIZE];
    
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", tm_info);
    
    // Format: [TIMESTAMP] OPERATION: DETAILS
    snprintf(log_entry, sizeof(log_entry), "[%s] %s: %s\n", 
             timestamp, operation, details);
    
    snprintf(cale, sizeof(cale), "%s/logged_hunt.txt", hunt_id);
    
    int fileId = open(cale, O_CREAT | O_WRONLY | O_APPEND, 0664);
    if (fileId == -1) {
        abandonCSTM();
    }
    
    if (write(fileId, log_entry, strlen(log_entry)) == -1) {
        close(fileId);
        abandonCSTM();
    }
    
    if (close(fileId) == -1) {
        abandonCSTM();
    }
}

void create_log_symlink(const char *hunt_id) {
    char target_path[PATH_MAX];
    char link_path[PATH_MAX];
    
    // Path to the actual log file
    snprintf(target_path, sizeof(target_path), "%s/logged_hunt.txt", hunt_id);
    
    // Path for the symbolic link
    snprintf(link_path, sizeof(link_path), "logged_hunt--%s", hunt_id);
    
    // Remove any existing symlink
    unlink(link_path);
    
    // Create the new symlink
    if (symlink(target_path, link_path) == -1) {
        abandonCSTM();
    }
}

void add(char *hunt_id){
    if (!(runThroughCheckDirCSTM(hunt_id))){
        createDirectoryCSTM(hunt_id);
    }

    char cale[PATH_MAX];
    char temp[TEXT_BUFFER];
    struct treasure ActiveTreasure;
    
    add_treasure(&ActiveTreasure);
    //strncpy(ActiveTreasure.treasure_id,hunt_id,sizeof(hunt_id));
    snprintf(cale, sizeof(cale), "%s/%s.dat",hunt_id, hunt_id);

    int fileId = 0;
    if ((fileId = open(cale, O_CREAT | O_WRONLY | O_APPEND, 00664)) == -1){
        abandonCSTM();
    }

    if (write(fileId,&ActiveTreasure,sizeof(struct treasure)) == -1){
        abandonCSTM();
    }

    if (close(fileId) == -1){
        abandonCSTM();
    }

    snprintf(temp,TEXT_BUFFER,"Added treasure with id - %s",ActiveTreasure.treasure_id);
    log_operation(hunt_id,"Add hunt",temp);
    create_log_symlink(hunt_id);
}

void list(char *hunt_id){
    char temp[LONG_TEXT];
    if (!(runThroughCheckDirCSTM(hunt_id))){
        sprintf(temp,"%s\n","No such directory");
        if (write(STDERR_FILENO,temp,strlen(temp)) == -1){
            abandonCSTM();
        }
        return;
    }

    struct stat dirStat;
    if (stat(hunt_id,&dirStat) != 0){
        abandonCSTM();
    }

    sprintf(temp,"\nNume: %s\n",hunt_id);
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    sprintf(temp,"Size: %ld\n",dirStat.st_size);
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    sprintf(temp,"Last modified - %s\n",ctime(&dirStat.st_mtime));
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }

    Treasure_t ActiveTreasure;
    sprintf(temp,"%s/%s.dat",hunt_id,hunt_id);
    int fileId = 0;
    if ((fileId = open(temp, O_RDONLY)) == -1){
        abandonCSTM();
    }
    int check = 0;
    while ((check = read(fileId,&ActiveTreasure,sizeof(ActiveTreasure))) != 0){
        if (check == -1){
            abandonCSTM();
        }
        /*sprintf(temp,"Treasure ID: %s\nUser Name: %s\nCoordinate X: %lf\nCoordinate Y: %lf\nClue: %s\nValue: %d\n\n",
                    ActiveTreasure.treasure_id,ActiveTreasure.user_name,ActiveTreasure.coordinateX,ActiveTreasure.coordinateY,ActiveTreasure.clue,ActiveTreasure.value);*/
        snprintf(temp,TEXT_BUFFER,"ID: %s\n",ActiveTreasure.treasure_id);
        if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
            abandonCSTM();
        }
    }

    if (close(fileId) == -1){
        abandonCSTM();
    }

    snprintf(temp,TEXT_BUFFER,"Listed hunt with ID - %s",hunt_id);
    log_operation(hunt_id,"List hunt",temp);
    create_log_symlink(hunt_id);
}

void remove_treasure(char *hunt_id, char *treasure_id){
    char cale[PATH_MAX];
    char oldCale[PATH_MAX];
    char temp[TEXT_BUFFER];
    if (!(runThroughCheckDirCSTM(hunt_id))){
        snprintf(temp,sizeof(temp),"No such directory\n");
        if (write(STDERR_FILENO,temp,strlen(temp)) == -1){
            abandonCSTM();
        }
    }

    snprintf(cale,PATH_MAX,"%s/%s.dat",hunt_id,hunt_id);

    int fileReadID = 0, fileWriteID = 0;
    if ((fileReadID = open(cale,O_RDONLY)) == -1){
        abandonCSTM();
    }

    snprintf(cale,PATH_MAX,"%s/temp.data",hunt_id);

    if ((fileWriteID = open(cale,O_CREAT | O_WRONLY, 0664)) == -1){
        abandonCSTM();
    }

    Treasure_t ActiveTreasure;

    int readCheck = 0, writeCheck = 0;
    while ((readCheck = read(fileReadID,&ActiveTreasure,sizeof(ActiveTreasure))) != 0){
        if (readCheck == -1){
            abandonCSTM();
        }
        if (strcmp(treasure_id,ActiveTreasure.treasure_id) == 0){
            writeCheck = 1;
            continue;
        }
        if (write(fileWriteID,&ActiveTreasure,sizeof(ActiveTreasure)) == -1){
            abandonCSTM();
        }
    }

    if (close(fileReadID) == -1){
        abandonCSTM();
    }

    if (close(fileWriteID) == -1){
        abandonCSTM();
    }

    snprintf(oldCale,PATH_MAX,"%s/%s.dat",hunt_id,hunt_id);
    if (remove(oldCale) == -1){
        abandonCSTM();
    }

    if (rename(cale,oldCale) == -1){
        abandonCSTM();
    }

    if (writeCheck == 1){
        snprintf(temp,TEXT_BUFFER,"Removed treasure with id - %s",treasure_id);
        log_operation(hunt_id,"Remove treasure",temp);
        create_log_symlink(hunt_id);
    }
    else {
        snprintf(temp,TEXT_BUFFER,"No matching treasure ID was found");
        log_operation(hunt_id,"Remove treasure",temp);
        create_log_symlink(hunt_id);
    }
}