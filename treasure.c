#include "treasure.h"
#include "customs.h"
#include <stdlib.h>

//typedef enum{userName,coordX,coordY,clue,value}Mode_t;

/*void add_treasure(Treasure_t* buff_struct){
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
}*/

void add_treasure(Treasure_t* buff_struct) {
    if (buff_struct == NULL || sizeof(*(buff_struct)) != sizeof(Treasure_t)) {
        abandonCSTM();
    }
    
    char temp[TEXT_BUFFER];
    char input_buffer[TEXT_BUFFER];
    
    // Get treasure ID
    sprintf(temp, "%s\n", "Give treasure id: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    strncpy(buff_struct->treasure_id, input_buffer, sizeof(buff_struct->treasure_id) - 1);
    buff_struct->treasure_id[sizeof(buff_struct->treasure_id) - 1] = '\0';
    
    // Get username
    sprintf(temp, "%s\n", "Give treasure username: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    strncpy(buff_struct->user_name, input_buffer, sizeof(buff_struct->user_name) - 1);
    buff_struct->user_name[sizeof(buff_struct->user_name) - 1] = '\0';
    
    // Get coordinate X
    sprintf(temp, "%s\n", "Give treasure coordX: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    buff_struct->coordinateX = atof(input_buffer);
    
    // Get coordinate Y
    sprintf(temp, "%s\n", "Give treasure coordY: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    buff_struct->coordinateY = atof(input_buffer);
    
    // Get clue
    sprintf(temp, "%s\n", "Give treasure clue: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    strncpy(buff_struct->clue, input_buffer, sizeof(buff_struct->clue) - 1);
    buff_struct->clue[sizeof(buff_struct->clue) - 1] = '\0';
    
    // Get value
    sprintf(temp, "%s\n", "Give treasure value: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    buff_struct->value = atoi(input_buffer);
}
