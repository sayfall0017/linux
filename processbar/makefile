CC = gcc
FLAGES = -c
LFLAGES = -o
GDB = -g
RM = rm -f
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
BIN = process.exe


$(BIN):$(OBJ)
	@$(CC) $(GDB) $(LFLAGES) $@ $^
	@echo "linking... $^ to $@\n"

%.o:%.c
	@$(CC) $(FLAGES) $<

.PHONY:clean
clean:
	$(RM) $(BIN) $(OBJ)
	@echo "Deleted!"
	

