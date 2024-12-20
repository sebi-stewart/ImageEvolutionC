CC = gcc

all: PPMEvolution.o

PPMEvolution.o:
	$(CC) -O0 -Wall PPMEvolution.c PPMImageProcessor.c PPMImage.c PPMPixel.c -o PPMImageProcessor.o

PPMImageProcessor.o:
	$(CC) -O0 -Wall PPMImageProcessorMain.c PPMImageProcessor.c PPMImage.c PPMPixel.c -o PPMImageProcessor.o

PPMImage.o: PPMImage.c PPMImage.h PPMPixel.c PPMPixel.h
	$(CC) -O0 -Wall PPMImage.c PPMPixel.c -o PPMImage.o

PPMPixel.o: PPMPixel.c PPMPixel.h
	$(CC) -O0 -Wall PPMPixel.c -o PPMPixel.o

clean:
	@rm -rf *.o
	@echo "Deleted Output files"
	@rm -rf *.ppm
	@echo "Deleted Image files"
