# ImageEvolutionC
## Description / Background
This image evolution project is the evolution, pun intended, of a university project where I used a 
genetic/evolutionary algorithm to recreate a specific image using a canvas and a set of random polygons.

The way it works is it create a base population of canvases with randomly selected colours. Then it goes through 
a set of 4 steps:
1. Evaluation: Evaluating them against the original image, by checking how similar each RGB value is to the original image. This is called the fitness. The higher fitness an individual has the better.
2. Survival: Killing/Removing individuals that fall under a certain threshold of fitness. In this case the worst 50% are removed from the total
3. Breeding: This step usually involves copying/combining certain individuals to back-fill the now missing spots. In my implementation I simply copy the best individual of each generation
4. Mutation: Now take each individual and apply small changes/mutations to it. This can involve changing the background colour. Adding/Removing Polygons to the image or maybe changing the polygons colour/corner positions.

These four steps are iterated multiple times, where each iteration is called a generation. 

## Run Instructions 
To run this one must have `clang` or `gcc` installed to be able to compile the C code. Following that it can be run by using the makefile with
```shell
    make all
```

This will compile any changes to the source code and run the program, or simply run the program if no changes are present.
Depending on the machine this is being run on threading can be increased by using the command
```shell
    export OMP_NUM_THREADS=4;
```
Where the number after the `=` determines how many threads are created during the evaluation process of the program.
Here I do recommend to use powers of 2 to determine how many threads to use.

## Testing Evidence
With this program I did several sets of testing, including manual run big bang test of the program and unit testing as well for many of the basic functions, where the basic functionality was asserted and made sure to work. 