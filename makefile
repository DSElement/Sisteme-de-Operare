CC = gcc
CFLAGS = -Wall -g

# Sources
TM_SOURCES = operations.c customs.c treasure_manager.c treasure.c
MONITOR_SOURCES = monitor.c customs.c monitor_state.c treasure.c
HUB_SOURCES = treasure_hub.c customs.c monitor_state.c
SCORE_CALC_SOURCES = score_calculator.c

HEADERS = operations.h customs.h treasure.h monitor_state.h

# Targets
TM_TARGET = treasure_manager
MONITOR_TARGET = monitor
HUB_TARGET = treasure_hub
SCORE_CALC_TARGET = score_calculator

# Object file patterns
TM_OBJECTS = $(TM_SOURCES:.c=.o)
MONITOR_OBJECTS = $(MONITOR_SOURCES:.c=.o)
HUB_OBJECTS = $(HUB_SOURCES:.c=.o)
SCORE_CALC_OBJECTS = $(SCORE_CALC_SOURCES:.c=.o)

.PHONY: all clean build rebuild test

all: build

build: $(TM_TARGET) $(MONITOR_TARGET) $(HUB_TARGET) $(SCORE_CALC_TARGET)

$(TM_TARGET): $(TM_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(MONITOR_TARGET): $(MONITOR_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(HUB_TARGET): $(HUB_OBJECTS)
	$(CC) $(CFLAGS) -pthread -o $@ $^

$(SCORE_CALC_TARGET): $(SCORE_CALC_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TM_TARGET) $(MONITOR_TARGET) $(HUB_TARGET) $(SCORE_CALC_TARGET)
	$(RM) *.o
	$(RM) -r test_hunt*
	$(RM) logged_hunt--*
	$(RM) .monitor_cmd

rebuild: clean build

# Test only treasure_manager flow
test: $(TM_TARGET)
	@echo "\n===== Running Test Scenario =====\n"

	@echo "1. Creating test_hunt1 with 3 treasures..."
	@echo "t1\nalice\n5.0\n5.0\nUnder the bridge\n50" | ./$(TM_TARGET) --add test_hunt1
	@echo "t2\nbob\n6.0\n6.0\nBehind the statue\n100" | ./$(TM_TARGET) --add test_hunt1
	@echo "t3\nalice\n7.0\n7.0\nNear the tree\n150" | ./$(TM_TARGET) --add test_hunt1

	@echo "\n2. Creating test_hunt2 with 2 treasures..."
	@echo "x1\ncarol\n1.0\n1.0\nIn the cave\n200" | ./$(TM_TARGET) --add test_hunt2
	@echo "x2\ndave\n2.0\n2.0\nUnder the leaves\n120" | ./$(TM_TARGET) --add test_hunt2

	@echo "\n3. Creating test_hunt3 with 2 treasures..."
	@echo "y1\nalice\n3.0\n3.0\nNear the old gate\n300" | ./$(TM_TARGET) --add test_hunt3
	@echo "y2\ncarol\n4.0\n4.0\nBy the river\n50" | ./$(TM_TARGET) --add test_hunt3

	@echo "\n4. Listing all treasures in test_hunt1..."
	./$(TM_TARGET) --list test_hunt1

	@echo "\n5. Listing all treasures in test_hunt2..."
	./$(TM_TARGET) --list test_hunt2

	@echo "\n6. Listing all treasures in test_hunt3..."
	./$(TM_TARGET) --list test_hunt3

	@echo "\n===== Test Scenario Complete =====\n"
