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
//#include <bits/struct_stat_time64_helper.h>
#include <time.h>

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

    snprintf(cale, sizeof(cale), "%s/logs.txt",hunt_id);
    snprintf(temp,TEXT_BUFFER,"Added treasure with id: %s\n",ActiveTreasure.treasure_id);

    if ((fileId = open(cale, O_CREAT | O_WRONLY | O_APPEND, 00664)) == -1){
        abandonCSTM();
    }

    if (write(fileId,&temp,sizeof(temp)) == -1){
        abandonCSTM();
    }

    if (close(fileId) == -1){
        abandonCSTM();
    }
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

    sprintf(temp,"Nume: %s\n",hunt_id);
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
        sprintf(temp,"Treasure ID: %s\nUser Name: %s\nCoordinate X: %lf\nCoordinate Y: %lf\nClue: %s\nValue: %d\n\n",
                    ActiveTreasure.treasure_id,ActiveTreasure.user_name,ActiveTreasure.coordinateX,ActiveTreasure.coordinateY,ActiveTreasure.clue,ActiveTreasure.value);
        if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
            abandonCSTM();
        }
    }
    if (close(fileId) == -1){
        abandonCSTM();
    }

}