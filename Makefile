# RESOLUTION = -D_240x320
RESOLUTION = -D_320x480

all: build-sdl1-linux

build-sdl1-linux:
	$(CC) ${RESOLUTION} Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -o Magic-Sushi-SDL1 \
		`sdl-config --cflags --libs` -lSDL_image -lSDL_mixer
	strip -s Magic-Sushi-SDL1

build-sdl2-linux:
	$(CC) ${RESOLUTION} Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -o Magic-Sushi-SDL2 \
		`sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer
	strip -s Magic-Sushi-SDL2

build-sdl1-web:
	emcc ${RESOLUTION} --use-preload-plugins --preload-file Assets \
		Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -s USE_SDL=2 -s USE_SDL_MIXER=2 \
		-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -o Magic-Sushi.html
	emstrip -s Magic-Sushi.wasm

build-sdl2-web:
	emcc ${RESOLUTION} --use-preload-plugins --preload-file Assets \
		Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -s USE_SDL=1 -s USE_SDL_MIXER=1 \
		-s USE_SDL_IMAGE=1 -s SDL_IMAGE_FORMATS='["png"]' -o Magic-Sushi.html
	emstrip -s Magic-Sushi.wasm

clean:
	-rm -f Magic-Sushi-SDL1
	-rm -f Magic-Sushi-SDL2
	-rm -f Magic-Sushi.data
	-rm -f Magic-Sushi.html
	-rm -f Magic-Sushi.js
	-rm -f Magic-Sushi.wasm
