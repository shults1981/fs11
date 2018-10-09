


all: funny_snake11.c
	gcc `pkg-config --cflags gtk+-3.0` -g  -o fs11 funny_snake11.c `pkg-config --libs gtk+-3.0`

