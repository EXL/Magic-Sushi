all: build-sdl2-linux

build-sdl2-linux:
	$(CC) Magic-Sushi-SDL2.c -o Magic-Sushi-SDL2 -lSDL2 -lSDL2_mixer
	strip -s Magic-Sushi-SDL2
