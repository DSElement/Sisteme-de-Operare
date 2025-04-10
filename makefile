CC = gcc
CFLAGS = -Wall -c -O2
TARGET = operations.c customs.c treasure_manager.c
OBJECT = operations.o customs.o treasure_manager.o

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
