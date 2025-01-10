CC			= gcc
LD			= gcc

CINCLUDES	= -Iinclude/
CWARNINGS	= -Wall -Wextra
CCFLAGS		= -O2 $(CINCLUDES) $(CWARNINGS)

ifeq ($(OS), Windows_NT)
	LDFLAGS	= -lmingw32 -lopengl32 -lSDL2main -lSDL2
else
	LDFLAGS	= -lGL -lSDL2
endif

SOURCES		:= $(wildcard src/*.c)
OBJECTS		:= $(patsubst src/%.c, obj/%.o, $(SOURCES))

all: $(OBJECTS)
	@mkdir -p bin
	@echo "Linking executable..."
	@$(LD) $^ -o bin/quark $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	@echo "Compiling $<..."
	@$(CC) $(CCFLAGS) -c $< -o $@

clean:
	@echo "Deleting binaries..."
	@rm -f bin/*
	@echo "Deleting objects..."
	@rm -f obj/*
