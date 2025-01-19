CC = gcc
CFLAGS = -O0 -Wall

# Pixel Sources/Headers
PPM_PIXEL_SOURCES = PPMPixel.c
PPM_PIXEL_HEADERS = PPMPixel.h

# Image Sources/Headers
PPM_IMAGE_SOURCES = PPMImage.c $(PPM_PIXEL_SOURCES)
PPM_IMAGE_HEADERS = PPMImage.h $(PPM_PIXEL_HEADERS)

# Image Processor Sources/Headers
PPM_IMAGE_PROCESSOR_SOURCES = PPMImageProcessor.c $(PPM_IMAGE_SOURCES)
PPM_IMAGE_PROCESSOR_HEADERS = PPMImageProcessor.h $(PPM_IMAGE_HEADERS)

# Evolution Sources/Headers
PPM_EVOLUTION_SOURCES = PPMEvolution.c $(PPM_IMAGE_PROCESSOR_SOURCES)
PPM_EVOLUTION_HEADERS = PPMEvolution.h $(PPM_IMAGE_PROCESSOR_HEADERS)

#Default Target
all: run-PPMEvolution

# PPMEvolution.o
PPMEvolution.o: $(PPM_EVOLUTION_SOURCES) $(PPM_EVOLUTION_HEADERS)
	$(CC) $(CFLAGS) $(PPM_EVOLUTION_SOURCES) -o PPMEvolution.o

# PPMImageProcessor.o
PPMImageProcessor.o: $(PPM_IMAGE_PROCESSOR_SOURCES) $(PPM_IMAGE_PROCESSOR_HEADERS) PPMImageProcessorMain.c
	$(CC) $(CFLAGS)  $(PPM_IMAGE_PROCESSOR_SOURCES) PPMImageProcessorMain.c -o PPMImageProcessor.o

#PPMImage.o
PPMImage.o: $(PPM_IMAGE_SOURCES) $(PPM_IMAGE_HEADERS)
	$(CC) $(CFLAGS) $(PPM_IMAGE_SOURCES) -o PPMImage.o

# PPMPixel.o
PPMPixel.o: $(PPM_PIXEL_SOURCES) $(PPM_PIXEL_HEADERS)
	$(CC) $(CFLAGS) $(PPM_PIXEL_SOURCES) -o PPMPixel.o

#Run PPMEvolution
run-PPMEvolution: PPMEvolution.o
	./PPMEvolution.o

# Run PPMImageProcessor
run-PPMImageProcessor: PPMImageProcessor.o
	./PPMImageProcessor.o

# Run PPMImage
run-PPMImage: PPMImage.o
	./PPMImage.o

# Run PPMPixel
run-PPMPixel: PPMPixel.o
	./PPMPixel.o

clean:
	rm -f *.o
	@echo "Deleted object files"
	rm -f *.ppm
	@echo "Deleted image files"
