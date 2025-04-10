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

void add(char *hunt_id){
    if (!(runThroughCheckDirCSTM(hunt_id))){
        createDirectoryCSTM(hunt_id);
    }

    char cale[PATH_MAX];
    char temp[TEXT_BUFFER];
    struct treasure_t haida;
    strcpy(haida.treasure_id,hunt_id);
    haida.coordinateX = 2;
    haida.coordinateY = 3;
    haida.value = 10;
    strcpy(haida.user_name,"dan");
    strcpy(haida.clue,"doamneajuta");

    snprintf(cale, sizeof(cale), "%s/%s.dat",hunt_id, hunt_id);

    int fileId = 0;
    if ((fileId = open(cale, O_CREAT | O_WRONLY | O_APPEND, 00664)) == -1){
        abandonCSTM();
    }

    write(fileId,&haida,sizeof(struct treasure_t));

    if (close(fileId) == -1){
        abandonCSTM();
    }

    snprintf(cale, sizeof(cale), "%s/logs.txt",hunt_id);
    snprintf(temp,TEXT_BUFFER,"Added treasure with id: %s\n\0",hunt_id);

    if ((fileId = open(cale, O_CREAT | O_WRONLY | O_APPEND, 00664)) == -1){
        abandonCSTM();
    }

    write(fileId,&temp,sizeof(temp));

    if (close(fileId) == -1){
        abandonCSTM();
    }

}