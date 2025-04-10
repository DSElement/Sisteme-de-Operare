#ifndef CUSTOMS_H__
#define CUSTOMS_H__

#include "treasure.h"
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>

#define ERROR_BUFFER_SIZE 20

void abandonCSTM();

DIR* openDirectoryCSTM(char *DirName);
void closeDirectoryCSTM(DIR *DirID);
void createDirectoryCSTM(char *DirName);
bool runThroughCheckDirCSTM(char *DirName);




#endif