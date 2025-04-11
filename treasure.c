#include "treasure.h"
#include "customs.h"
#include <stdlib.h>

//typedef enum{userName,coordX,coordY,clue,value}Mode_t;

void add_treasure(Treasure_t* buff_struct){
    if (buff_struct == NULL || sizeof(*(buff_struct)) != sizeof(Treasure_t)){
        abandonCSTM();
    }
    char temp[TEXT_BUFFER];
    double temp_float = 0;
    int temp_int = 0;

    sprintf(temp,"%s\n","Give treasure id: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,temp,TEXT_BUFFER) == -1){
        abandonCSTM();
    }
    temp[strcspn(temp,"\n")] = '\0';
    strcpy(buff_struct->treasure_id,temp);

    sprintf(temp,"%s\n","Give treasure username: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,temp,TEXT_BUFFER) == -1){
        abandonCSTM();
    }
    temp[strcspn(temp,"\n")] = '\0';
    strcpy(buff_struct->user_name,temp);

    sprintf(temp,"%s\n","Give treasure coordX: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,&temp_float,__SIZEOF_DOUBLE__) == -1){
        abandonCSTM();
    }
    buff_struct->coordinateX = temp_float;

    sprintf(temp,"%s\n","Give treasure coordY: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,&temp_float,__SIZEOF_DOUBLE__) == -1){
        abandonCSTM();
    }
    buff_struct->coordinateY = temp_float;

    sprintf(temp,"%s\n","Give treasure clue: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,temp,TEXT_BUFFER) == -1){
        abandonCSTM();
    }
    temp[strcspn(temp,"\n")] = '\0';
    strcpy(buff_struct->clue,temp);

    sprintf(temp,"%s\n","Give treasure value: ");
    if (write(STDOUT_FILENO,temp,strlen(temp)) == -1){
        abandonCSTM();
    }
    if (read(STDIN_FILENO,&temp_int,__SIZEOF_INT__) == -1){
        abandonCSTM();
    }
    buff_struct->coordinateX = temp_int;
}
