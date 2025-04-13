#include "treasure.h"
#include "customs.h"
#include <stdlib.h>

/*void add_treasure(Treasure_t* buff_struct) {
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
}*/

#include "treasure.h"
#include "customs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Helper functions for validation
int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') 
        return 0;
    
    // Allow negative numbers
    if (*str == '-') 
        str++;
    
    // Check if it's a valid float format
    int has_decimal = 0;
    while (*str) {
        if (*str == '.') {
            if (has_decimal) // More than one decimal point is invalid
                return 0;
            has_decimal = 1;
        } 
        else if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    
    return 1;
}

int is_valid_integer(const char *str) {
    if (str == NULL || *str == '\0') 
        return 0;
    
    // Allow negative numbers
    if (*str == '-') 
        str++;
    
    // Check if it contains only digits
    while (*str) {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    
    return 1;
}

// Trim whitespace from a string
void trim_string(char *str) {
    if (str == NULL)
        return;
        
    char *start = str;
    
    // Trim leading space
    while (isspace(*start)) start++;
    
    // If all spaces, return empty string
    if (*start == 0) {
        *str = 0;
        return;
    }
    
    // Trim trailing space
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    // Write new null terminator
    *(end + 1) = 0;
    
    // Move entire string if we trimmed leading spaces
    if (start != str) {
        while (*start) {
            *str = *start;
            str++;
            start++;
        }
        *str = 0;
    }
}

void add_treasure(Treasure_t* buff_struct) {
    if (buff_struct == NULL || sizeof(*(buff_struct)) != sizeof(Treasure_t)) {
        abandonCSTM();
    }
    
    char temp[TEXT_BUFFER];
    char input_buffer[TEXT_BUFFER];
    int valid_input;
    
    // Get treasure ID
    sprintf(temp, "%s", "Give treasure id: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    trim_string(input_buffer);
    
    if (strlen(input_buffer) == 0) {
        sprintf(temp, "%s", "Error: Treasure ID cannot be empty\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    
    strncpy(buff_struct->treasure_id, input_buffer, sizeof(buff_struct->treasure_id) - 1);
    buff_struct->treasure_id[sizeof(buff_struct->treasure_id) - 1] = '\0';
    
    // Get username
    sprintf(temp, "%s", "Give treasure username: ");
    if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
        abandonCSTM();
    }
    
    if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
        abandonCSTM();
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    trim_string(input_buffer);
    
    if (strlen(input_buffer) == 0) {
        sprintf(temp, "%s", "Error: Username cannot be empty\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    
    strncpy(buff_struct->user_name, input_buffer, sizeof(buff_struct->user_name) - 1);
    buff_struct->user_name[sizeof(buff_struct->user_name) - 1] = '\0';
    
    // Get coordinate X
    valid_input = 0;
    while (!valid_input) {
        sprintf(temp, "%s", "Give treasure coordX: ");
        if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        
        if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
            abandonCSTM();
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        trim_string(input_buffer);
        
        if (is_valid_number(input_buffer)) {
            buff_struct->coordinateX = atof(input_buffer);
            valid_input = 1;
        } else {
            sprintf(temp, "%s", "Error: Please enter a valid number for coordinate X\n");
            if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
                abandonCSTM();
            }
        }
    }
    
    // Get coordinate Y
    valid_input = 0;
    while (!valid_input) {
        sprintf(temp, "%s", "Give treasure coordY: ");
        if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        
        if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
            abandonCSTM();
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        trim_string(input_buffer);
        
        if (is_valid_number(input_buffer)) {
            buff_struct->coordinateY = atof(input_buffer);
            valid_input = 1;
        } else {
            sprintf(temp, "%s", "Error: Please enter a valid number for coordinate Y\n");
            if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
                abandonCSTM();
            }
        }
    }
    
    // Get clue
    sprintf(temp, "%s", "Give treasure clue: ");
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
    valid_input = 0;
    while (!valid_input) {
        sprintf(temp, "%s", "Give treasure value: ");
        if (write(STDOUT_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        
        if (read(STDIN_FILENO, input_buffer, TEXT_BUFFER) == -1) {
            abandonCSTM();
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        trim_string(input_buffer);
        
        if (is_valid_integer(input_buffer)) {
            buff_struct->value = atoi(input_buffer);
            valid_input = 1;
        } else {
            sprintf(temp, "%s", "Error: Please enter a valid integer for treasure value\n");
            if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
                abandonCSTM();
            }
        }
    }
}

// Alternative function that can parse a single string containing all treasure data
void parse_and_add_treasure(Treasure_t* buff_struct, const char* input_string) {
    if (buff_struct == NULL || input_string == NULL) {
        abandonCSTM();
    }
    
    char temp[TEXT_BUFFER];
    char buffer[LONG_TEXT]; // Larger buffer to hold the entire input
    char *lines[6]; // To store pointers to the start of each line
    int line_count = 0;
    
    // Copy input string to buffer
    strncpy(buffer, input_string, LONG_TEXT - 1);
    buffer[LONG_TEXT - 1] = '\0';
    
    // Split the input by newlines
    char *token = strtok(buffer, "\n");
    while (token != NULL && line_count < 6) {
        lines[line_count++] = token;
        token = strtok(NULL, "\n");
    }
    
    // Check if we have enough lines
    if (line_count < 6) {
        sprintf(temp, "%s", "Error: Not enough data provided\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    
    // Process treasure ID
    trim_string(lines[0]);
    if (strlen(lines[0]) == 0) {
        sprintf(temp, "%s", "Error: Treasure ID cannot be empty\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    strncpy(buff_struct->treasure_id, lines[0], sizeof(buff_struct->treasure_id) - 1);
    buff_struct->treasure_id[sizeof(buff_struct->treasure_id) - 1] = '\0';
    
    // Process username
    trim_string(lines[1]);
    if (strlen(lines[1]) == 0) {
        sprintf(temp, "%s", "Error: Username cannot be empty\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    strncpy(buff_struct->user_name, lines[1], sizeof(buff_struct->user_name) - 1);
    buff_struct->user_name[sizeof(buff_struct->user_name) - 1] = '\0';
    
    // Process coordinate X
    trim_string(lines[2]);
    if (!is_valid_number(lines[2])) {
        sprintf(temp, "%s", "Error: Invalid value for coordinate X\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    buff_struct->coordinateX = atof(lines[2]);
    
    // Process coordinate Y
    trim_string(lines[3]);
    if (!is_valid_number(lines[3])) {
        sprintf(temp, "%s", "Error: Invalid value for coordinate Y\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    buff_struct->coordinateY = atof(lines[3]);
    
    // Process clue
    strncpy(buff_struct->clue, lines[4], sizeof(buff_struct->clue) - 1);
    buff_struct->clue[sizeof(buff_struct->clue) - 1] = '\0';
    
    // Process value
    trim_string(lines[5]);
    if (!is_valid_integer(lines[5])) {
        sprintf(temp, "%s", "Error: Invalid value for treasure value\n");
        if (write(STDERR_FILENO, temp, strlen(temp)) == -1) {
            abandonCSTM();
        }
        abandonCSTM();
    }
    buff_struct->value = atoi(lines[5]);
}