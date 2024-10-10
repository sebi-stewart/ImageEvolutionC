CC = gcc

all: PPMImageProcessor.o

PPMImageProcessor.o: PPMImageProcessor.c PPMImageProcessor.h PPMImage.c PPMImage.h PPMPixel.c PPMPixel.h
	$(CC) -O0 -Wall PPMImageProcessor.c PPMImage.c PPMPixel.c -o PPMImageProcessor.o

PPMImage.o: PPMImage.c PPMImage.h PPMPixel.c PPMPixel.h
	$(CC) -O0 -Wall PPMImage.c PPMPixel.c -o PPMImage.o

PPMPixel.o: PPMPixel.c PPMPixel.h
	$(CC) -O0 -Wall PPMPixel.c -o PPMPixel.o

clean:
	@rm -rf *.o
	@echo "Deleted Output files"
	@rm -rf *.ppm
	@echo "Deleted Image files"
