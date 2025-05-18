CC = gcc
CFLAGS = -Wall -g

# Sources
TM_SOURCES = operations.c customs.c treasure_manager.c treasure.c
MONITOR_SOURCES = monitor.c customs.c monitor_state.c treasure.c
HUB_SOURCES = treasure_hub.c customs.c monitor_state.c

HEADERS = operations.h customs.h treasure.h monitor_state.h

# Targets
TM_TARGET = treasure_manager
MONITOR_TARGET = monitor
HUB_TARGET = treasure_hub

# Object file patterns
TM_OBJECTS = $(TM_SOURCES:.c=.o)
MONITOR_OBJECTS = $(MONITOR_SOURCES:.c=.o)
HUB_OBJECTS = $(HUB_SOURCES:.c=.o)

.PHONY: all clean build rebuild test

all: build

build: $(TM_TARGET) $(MONITOR_TARGET) $(HUB_TARGET)

$(TM_TARGET): $(TM_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(MONITOR_TARGET): $(MONITOR_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(HUB_TARGET): $(HUB_OBJECTS)
	$(CC) $(CFLAGS) -pthread -o $@ $^  # <<== thread support here only

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TM_TARGET) $(MONITOR_TARGET) $(HUB_TARGET)
	$(RM) *.o
	$(RM) -r test_hunt*
	$(RM) logged_hunt--*
	$(RM) .monitor_cmd

rebuild: clean build

# Test only treasure_manager flow
test: $(TM_TARGET)
	@echo "\n===== Running Test Scenario =====\n"

	@echo "1. Adding a treasure to test_hunt1..."
	@echo "test_id1\nuser1\n10.5\n20.3\nHidden under the big rock\n100" | ./$(TM_TARGET) --add test_hunt1

	@echo "\n2. Adding another treasure to test_hunt1..."
	@echo "test_id2\nuser2\n15.7\n30.8\nBehind the waterfall\n200" | ./$(TM_TARGET) --add test_hunt1

	@echo "\n3. Listing treasures in test_hunt1..."
	./$(TM_TARGET) --list test_hunt1

	@echo "\n4. Viewing details of test_id1..."
	./$(TM_TARGET) --view test_hunt1 test_id1

	@echo "\n5. Removing treasure test_id1..."
	./$(TM_TARGET) --remove_treasure test_hunt1 test_id1

	@echo "\n6. Listing treasures after removal..."
	./$(TM_TARGET) --list test_hunt1

	@echo "\n7. Creating a second hunt test_hunt2..."
	@echo "second_id\nuser3\n5.5\n8.3\nInside the hollow tree\n150" | ./$(TM_TARGET) --add test_hunt2

	@echo "\n8. Removing entire hunt test_hunt2..."
	./$(TM_TARGET) --remove_hunt test_hunt2

	@echo "test_id5\nuser1\n10.5\n20.3\nHidden under the big rock\n100" | ./$(TM_TARGET) --add test_hunt1
	@echo "test_id6\nuser1\n10.5\n20.3\nHidden under the big rock\n100" | ./$(TM_TARGET) --add test_hunt1
	@echo "test_id7\nuser1\n10.5\n20.3\nHidden under the big rock\n100" | ./$(TM_TARGET) --add test_hunt2
	@echo "test_id8\nuser1\n10.5\n20.3\nHidden under the big rock\n100" | ./$(TM_TARGET) --add test_hunt3

	@echo "\n===== Test Scenario Complete =====\n"
