CC = gcc

all: PPMImage.o

PPMImage.o: PPMImage.c PPMPixel.c
	$(CC) -O0 -Wall PPMImage.c PPMPixel.c -o PPMImage.o

PPMPixel.o: PPMPixel.c
	$(CC) -O0 -Wall PPMPixel.c -o PPMPixel.o

clean:
	@rm -rf *.o
	@echo "Deleted Output files"
	@rm -rf *.ppm
	@echo "Deleted Image files"
