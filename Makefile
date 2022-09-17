# This Makefile was created by EXL: 15-Sep-2022
# Edited: 18-Sep-2022 (add windows support using MSYS2)

# RESOLUTION = -D_240x320
RESOLUTION = -D_320x480

all: build-sdl2-linux

build-sdl1-linux:
	$(CC) ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -o Magic-Sushi-SDL1 \
		`sdl-config --cflags --libs` -lSDL_image -lSDL_mixer
	strip -s Magic-Sushi-SDL1

build-sdl2-linux:
	$(CC) ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -o Magic-Sushi-SDL2 \
		`sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer
	strip -s Magic-Sushi-SDL2

build-sdl1-web:
	emcc ${RESOLUTION} -O2 --use-preload-plugins --preload-file Assets \
		Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -s USE_SDL=1 -s USE_SDL_MIXER=1 \
		-s USE_SDL_IMAGE=1 -o Magic-Sushi.html
	emstrip -s Magic-Sushi.wasm

build-sdl2-web:
	emcc ${RESOLUTION} -O2 --use-preload-plugins --preload-file Assets \
		Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -s USE_SDL=2 -s USE_SDL_MIXER=2 \
		-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -o Magic-Sushi.html
	emstrip -s Magic-Sushi.wasm

build-sdl1-ezx:
	arm-linux-gnu-gcc -D_240x320 -D_EZX -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -o Magic-Sushi-EZX \
               `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer
	arm-linux-gnu-strip -s Magic-Sushi-EZX

build-sdl1-windows:
	windres -i Windows/Magic-Sushi.rc -o Magic-Sushi_res.o --include-dir=.
	$(CC) ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -o Magic-Sushi-SDL1.exe \
		Magic-Sushi_res.o `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer
	strip -s Magic-Sushi-SDL1.exe

build-sdl1-windows-static:
	windres -i Windows/Magic-Sushi.rc -o Magic-Sushi_res.o --include-dir=.
	$(CC) -static -static-libgcc ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL1.c -o Magic-Sushi-SDL1.exe \
		Magic-Sushi_res.o -lSDL_image -lSDL_mixer -lmad `sdl-config --cflags --static-libs` -ltiff \
		-lzstd -ldeflate -lwebp -lpng -lz -ljpeg -llzma -ljbig -llerc -lstdc++ -lvorbisfile -lvorbis -logg
	strip -s Magic-Sushi-SDL1.exe

build-sdl2-windows:
	windres -i Windows/Magic-Sushi.rc -o Magic-Sushi_res.o --include-dir=.
	$(CC) ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -o Magic-Sushi-SDL2.exe \
		Magic-Sushi_res.o `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer
	strip -s Magic-Sushi-SDL2.exe

build-sdl2-windows-static:
	windres -i Windows/Magic-Sushi.rc -o Magic-Sushi_res.o --include-dir=.
	$(CC) -static -static-libgcc ${RESOLUTION} -O2 Magic-Sushi-Engine.c Magic-Sushi-SDL2.c -o Magic-Sushi-SDL2.exe \
		Magic-Sushi_res.o `sdl2-config --cflags --static-libs` -lSDL2_image -ltiff -lwebp -ljpeg -llzma -ljbig -llerc \
		-ljxl -lhwy -lzstd -lbrotlidec -lbrotlicommon -ldeflate -lpng -lz -lstdc++ \
		-lSDL2_mixer -lwinmm -lmpg123 -lopusfile -logg -lopus -lshlwapi -lssp
	strip -s Magic-Sushi-SDL2.exe

clean:
	-rm -f Magic-Sushi_res.o
	-rm -f Magic-Sushi-EZX
	-rm -f Magic-Sushi-SDL1
	-rm -f Magic-Sushi-SDL2
	-rm -f Magic-Sushi-SDL1.exe
	-rm -f Magic-Sushi-SDL2.exe
	-rm -f Magic-Sushi.data
	-rm -f Magic-Sushi.html
	-rm -f Magic-Sushi.js
	-rm -f Magic-Sushi.wasm
