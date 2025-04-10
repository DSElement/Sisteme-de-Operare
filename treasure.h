#ifndef TREASURE_H__
#define TREASURE_H__

struct treasure_t{
    char treasure_id[20];
    char user_name[20];
    double coordinateX;
    double coordinateY;
    char clue[200];
    int value;
};

void add_treasure();


#endif