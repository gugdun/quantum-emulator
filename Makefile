CC			= gcc
LD			= gcc
CINCLUDES	= -Iinclude/
CWARNINGS	= -Wall -Wextra
CCFLAGS		= -O3 $(CINCLUDES) $(CWARNINGS)
LDFLAGS		= -lGL -lSDL2
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
