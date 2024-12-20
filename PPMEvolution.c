//
// Created by Sebastian Stewart on 20/12/2024.
//

#include "PPMEvolution.h"
#include <stdio.h>
#include <string.h>

void ppm_evolution_population_evaluate(PPMImage* org, Population* comparison){
    for (int i = 0; i < comparison->count; ++i) {
        Individual* current_individual = &comparison->pop[i];
        if (current_individual == NULL){
            #ifdef DEBUG
            printf("ppm_evolution_population_evaluate: Current individual was null\n");
            #endif
            continue;
        }
        PPMImageProcessor* current_processor = current_individual->processor;
        if (current_processor == NULL){
            #ifdef DEBUG
            printf("ppm_evolution_population_evaluate: Current processor was null\n");
            #endif
            continue;
        }
        PPMImage* cur_image = ppm_image_processor_draw_polygons(current_processor);
        current_individual->eval = ppm_image_compare(org, cur_image);
    }
}

void ppm_evolution_population_mutate(Population* population){}
void ppm_evolution_population_breed(Population* population){}
void ppm_evolution_population_kill_off(Population* population, int survival_rate){
    int amount_to_delete = (int)((float)population->count  / 100.0 * (float) survival_rate);
    int to_delete[amount_to_delete][2];
    for (int i = 0; i < population->count; i++){
        Individual* current_individual = &population->pop[i];
        if (current_individual == NULL){
#ifdef DEBUG
            printf("ppm_evolution_population_kill_off: Current individual was null\n");
#endif
            continue;
        }

    }
}

void ppm_evolution_runner(Population* base_population,
                          PPMImage* base_image,
                          RunnerParams* params
                          ){

    // Unpack functions into local variables
    void (*evaluation_func)(PPMImage*, Population*) = params->evaluation_func;
    int survival_rate = params->survival_rate;
    void (*mutation_func)(Population*) = params->mutation_func;
    void (*breed_func)(Population*) = params->breed_func;


}

Population* ppm_evolution_create_population(int population_size){

}

void run(char* image_filepath, int population_size, int generation_count){

    PPMImage* base_image = ppm_image_load(image_filepath);
    // Initialize the function bundle
    RunnerParams functions = {
            .evaluation_func = ppm_evolution_population_evaluate,
            .survival_rate = 50,
            .mutation_func = ppm_evolution_population_mutate,
            .breed_func = ppm_evolution_population_breed
    };

    // Call the runner
    ppm_evolution_runner(&population, base_image, &functions, 50);

}