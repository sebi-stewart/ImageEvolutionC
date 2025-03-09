CC = gcc
CFLAGS = -O3 -Wall
TESTING_FLAGS = -O3 -w -DPPM_TESTING
CC_ALT = clang
PARALLEL_FLAGS = -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp

# Global Sources/Headers
PPM_GLOBAL_SOURCES = PPMMacros.c
PPM_GLOBAL_HEADERS = PPMMacros.h

# Pixel Sources/Headers
PPM_PIXEL_SOURCES = PPMPixel.c $(PPM_GLOBAL_SOURCES)
PPM_PIXEL_HEADERS = PPMPixel.h $(PPM_GLOBAL_HEADERS)

# Image Sources/Headers
PPM_IMAGE_SOURCES = PPMImage.c $(PPM_PIXEL_SOURCES)
PPM_IMAGE_HEADERS = PPMImage.h $(PPM_PIXEL_HEADERS)

# Image Processor Sources/Headers
PPM_IMAGE_PROCESSOR_SOURCES = PPMImageProcessor.c $(PPM_IMAGE_SOURCES)
PPM_IMAGE_PROCESSOR_HEADERS = PPMImageProcessor.h $(PPM_IMAGE_HEADERS)

# Evolution Sources/Headers
PPM_EVOLUTION_SOURCES = PPMEvolution.c $(PPM_IMAGE_PROCESSOR_SOURCES)
PPM_EVOLUTION_HEADERS = PPMEvolution.h $(PPM_IMAGE_PROCESSOR_HEADERS)

# Testing files
MUNIT_SOURCE = test/munit/munit.c

#Default Target
all: run-PPMEvolution

test: test-PPMPixel

# PPMEvolution.o
PPMEvolution.o: $(PPM_EVOLUTION_SOURCES) $(PPM_EVOLUTION_HEADERS)
	$(CC_ALT) $(CFLAGS) $(PARALLEL_FLAGS) $(PPM_EVOLUTION_SOURCES) -o PPMEvolution.o

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
	find . -maxdepth 2 -type f -iname "*.o" -exec rm {} \;
	@echo "Deleted object files"
	find . -maxdepth 2 -type f -iname "*.ppm" -exec rm {} \;
	@echo "Deleted image files"


test-PPMPixel:
	$(CC) $(TESTING_FLAGS) test/test_PPMPixel.c $(PPM_PIXEL_SOURCES) $(MUNIT_SOURCE) -o test/test_PPMPixel.o

test-PPMImage:
	$(CC) $(TESTING_FLAGS) test/test_PPMImage.c $(PPM_IMAGE_SOURCES) $(MUNIT_SOURCE) -o test/test_PPMImage.o

