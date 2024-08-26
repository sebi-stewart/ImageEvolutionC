CC = gcc

all: image

image: PPMImage.c PPMPixel.c
	$(CC) -O0 -Wall PPMImage.c PPMPixel.c -o PPMImage.o

pixel: PPMPixel.c
	$(CC) -O0 -Wall PPMPixel.c -o PPMPixel.o

clean:
	rm PPMImage.o PPMPixel.o
