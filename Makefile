CC			= gcc
LD			= gcc

SRC         = src
OBJ         = obj
BIN         = bin

CINCLUDES	= -Iinclude/
CWARNINGS	= -Wall -Wextra
CCFLAGS		= -O2 -DDEBUG $(CINCLUDES) $(CWARNINGS)

ifeq ($(OS), Windows_NT)
	LDFLAGS	= -lmingw32 -lopengl32 -lSDL2main -lSDL2
else
	LDFLAGS	= -lGL -lSDL2
endif

SOURCES		:= $(wildcard $(SRC)/*.c)
OBJECTS		:= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

all: $(OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking executable..."
	@$(LD) $^ -o $(BIN)/quark $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p $(OBJ)
	@echo "Compiling $<..."
	@$(CC) $(CCFLAGS) -c $< -o $@

clean:
	@echo "Deleting binaries..."
	@rm -f $(BIN)/*
	@echo "Deleting objects..."
	@rm -f $(OBJECTS)
