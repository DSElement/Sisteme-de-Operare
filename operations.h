#ifndef OPERATIONS_H__
#define OPERATIONS_H__

#include "treasure.h"
#include "customs.h"

/*add <hunt_id>: Add a new treasure to the specified hunt (game session). Each hunt is stored in a separate directory.
list <hunt_id>: List all treasures in the specified hunt. First print the hunt name, the (total) file size and last modification time of its treasure file(s), then list the treasures.
view <hunt_id> <id>: View details of a specific treasure
remove_treasure <hunt_id> <id>: Remove a treasure 
remove_hunt <hunt_id>: Remove an entire hunt
*/


void add(char *hunt_id);
void list(char *hunt_id);
void view(char *hunt_id,char *treasure_id);
void remove_treasure(char *hunt_id, char *treasure_id);


#endif