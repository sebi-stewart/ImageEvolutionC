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

## Testing
With this program I did several sets of testing, including manual run big bang test of the program and unit testing for the basic functionality such as the polygon creation and the image creation. This can both be tested by using the 
```shell
    make test-PPMPixel
    ./test/test_PPMPixel.o
    make test-PPMImage
    ./test/test_PPMImage.o
``` 
commands in the terminal.

## Results:
After running the program for several hours I was able to achieve a similarity of 97% to the original image:
<table>
    <tr>
        <th> Original</th>
        <th> Created Image</th>
    </tr>
    <tr>
        <td><img width="128" alt="Original Image" src="https://github.com/user-attachments/assets/70f59ae2-8220-4d85-b3c2-fbd62847c5c1" /></td>
        <td><img width="128" alt="Best Evolution" src="https://github.com/user-attachments/assets/a074fa1d-436d-44be-8170-ae66936a3de7" /></td>
    </tr>
</table>


> [!NOTE]
> This was run using multithreading and only having the application running and used a 200x200 pixel image that was first converted to the PPM format and then the result converted back to png to display here
