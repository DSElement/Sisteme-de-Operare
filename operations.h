#ifndef OPERATIONS_H__
#define OPERATIONS_H__

#include "treasure.h"

#define TEXT_BUFFER 60
#define HUNTS_DIR "HUNTS"

/*add <hunt_id>: Add a new treasure to the specified hunt (game session). Each hunt is stored in a separate directory.
list <hunt_id>: List all treasures in the specified hunt. First print the hunt name, the (total) file size and last modification time of its treasure file(s), then list the treasures.
view <hunt_id> <id>: View details of a specific treasure
remove_treasure <hunt_id> <id>: Remove a treasure 
remove_hunt <hunt_id>: Remove an entire hunt
*/


void add(char *hunt_id);


#endif