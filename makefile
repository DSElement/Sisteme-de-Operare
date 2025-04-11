CC = gcc
CFLAGS = -Wall -c
TARGET = operations.c customs.c treasure_manager.c treasure.c
OBJECT = operations.o customs.o treasure_manager.o treasure.o

EXE = treasure_manager

all: $(EXE) delete

BUILD:
	$(CC) $(CFLAGS) $(TARGET)
	$(CC) -Wall -o $(EXE) $(OBJECT)

$(EXE):
	./$(EXE)

RUN:
	$(BUILD) 
	./$(EXE)

delete:
	$(RM) $(EXE) *.o
