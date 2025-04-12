#include "treasure.h"
#include "operations.h"
#include <stdio.h>

int main(){
    //add("primul_hunt");
    //add("primul_hunt");
    //add("second_hunt");
    list("primul_hunt");
    view("primul_hunt","test");
    remove_treasure("primul_hunt","test");
    view("primul_hunt","test");
    list("primul_hunt");
    remove_hunt("primul_hunt");
    return 0;
}