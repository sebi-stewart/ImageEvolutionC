//
// Created by Sebastian Stewart on 20/12/2024.
//

#include "PPMEvolution.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        current_individual->eval = ppm_image_compare(org, cur_image); // The lower the value the better
        if (current_individual->eval == -1){
#ifdef DEBUG
            printf("ppm_evolution_population_evaluate: Evaluation was -1\n");
#endif
            exit(1);
        }
        if (current_individual->eval < comparison->best->eval){
            comparison->best = current_individual;
        }
        ppm_image_del(cur_image);
    }
}

float rand_float(){
    return (float)rand() / (float)RAND_MAX;
}

int rand_int(int max){
    return (int)(rand_float() * (float)max);
}

Polygon* ppm_evolution_random_polygon(Polygon* polygons){
    if (polygons == NULL) {
        fprintf(stderr, "ppm_evolution_random_polygon: no polygon to mutate");
        exit(1); // No polygons in the list
    }

    int polygon_count = 0;
    for (Polygon* cur = polygons; cur != NULL; cur=cur->next){
        polygon_count++;
    }

    // Generate a random index between 0 and polygon_count - 1
    int random_index = (int)(rand_float() * polygon_count);

    // Traverse the list to the random index
    Polygon* selected_polygon = polygons;
    for (int i = 0; i < random_index; i++) {
        selected_polygon = selected_polygon->next;
    }
    return selected_polygon;
}

int narrow_coords(int coord, int max_val){
    if (coord < 0){
        return 0;
    }
    if (coord >= max_val){
        return max_val - 1;
    }
    return coord;
}

void ppm_evolution_mutate_polygon_points(PPMImageProcessor* processor){
    if (processor->polygons) {
#ifdef DEBUG
        fprintf(stderr, "ppm_evolution_mutate_polygon_points: no polygon to mutate");
#endif
        return; // No polygons in the list
    }

    Polygon* random_polygon = ppm_evolution_random_polygon(processor->polygons);
    double coord;
    for (Corner* cur = random_polygon->corners; cur != NULL; cur = cur->next){
        if (rand_float() > 0.1){
            coord = (double)cur->x - (rand_float() * 5 - 2.5);
            cur->x = narrow_coords((int)coord, (int)processor->width);
        }
        if (rand_float() > 0.1){
            coord = (double)cur->y - (rand_float() * 5 - 2.5);
            cur->y = narrow_coords((int)coord, (int)processor->height);
        }
    }
}

int narrow_colors(double color){
    if (color <= 0){
        return 0;
    } if (color >= 255){
        return 255;
    } return (int) color;
}

void ppm_evolution_mutate_polygon_colors(PPMImageProcessor* processor){
    if (processor->polygons) {
#ifdef DEBUG
        fprintf(stderr, "ppm_evolution_mutate_polygon_colors: no polygon to mutate");
#endif
        return; // No polygons in the list
    }

    Polygon* random_polygon = ppm_evolution_random_polygon(processor->polygons);
    PPMPixel* polygon_color = random_polygon->color;
    double color;
    if (rand_float() > 0.1){
        color = (double)polygon_color->R - (rand_float() * 10 - 5);
        polygon_color->R = narrow_colors(color);
    }
    if (rand_float() > 0.1){
        color = (double)polygon_color->G - (rand_float() * 10 - 5);
        polygon_color->G = narrow_colors(color);
    }
    if (rand_float() > 0.1){
        color = (double)polygon_color->B - (rand_float() * 10 - 5);
        polygon_color->B = narrow_colors(color);
    }
}

void ppm_evolution_mutate_add_polygon(PPMImageProcessor* processor){
    int polygon_count = 0;
    for(Polygon* cur = processor->polygons; cur != NULL; cur = cur->next){
        polygon_count++;
    }
    if (polygon_count >= 50){
#ifdef DEBUG_VERBOSE
        fprintf(stderr, "ppm_evolution_mutate_add_polygon: no individual to mutate");
#endif
        return;
    }

    int width = (int)processor->width;
    int height = (int)processor->height;

    Polygon* polygon = polygon_init(rand_int(255), rand_int(255), rand_int(255));

    push_one_corner(polygon, corner_init(rand_int(width), rand_int(height)));
    push_one_corner(polygon, corner_init(rand_int(width), rand_int(height)));
    push_one_corner(polygon, corner_init(rand_int(width), rand_int(height)));

    push_one_polygon(processor, polygon);
}

void ppm_evolution_mutate_remove_polygon(PPMImageProcessor* processor){
    Polygon* random_polygon = ppm_evolution_random_polygon(processor->polygons);
    Polygon* prev_polygon = processor->polygons;
    if (prev_polygon == random_polygon){
        processor->polygons = random_polygon->next;
        pop_all_corners(random_polygon);
        free
    }
}

void ppm_evolution_population_mutate(Population* population, bool elitist){
    for (int i = 0; i < population->count; i++){
        Individual* current_individual = &population->pop[i];
        if (elitist && current_individual == population->best){
            continue;
        }
        if (current_individual == NULL) {
#ifdef DEBUG
            fprintf(stderr, "ppm_evolution_population_mutate: no individual to mutate");
#endif
            continue;
        }
        PPMImageProcessor* current_processor = current_individual->processor;
        if (current_processor == NULL) {
#ifdef DEBUG
            fprintf(stderr, "ppm_evolution_population_mutate: no processor to mutate");
#endif
            continue;
        }


        if (rand_float() < 0.5){
            ppm_evolution_mutate_polygon_points(current_processor);
        }
        if (rand_float() < 0.5){
            ppm_evolution_mutate_polygon_colors(current_processor);
        }
        if (rand_float() < 0.02){
            ppm_evolution_mutate_add_polygon(current_processor);
        }
        if (rand_float() < 0.005){
            ppm_evolution_mutate_remove_polygon(current_processor);
        }
    }
}
void ppm_evolution_population_breed_copy_best(Population* population){
    Individual* best = population->best;
    if (best == NULL){
        fprintf(stderr, "ppm_evolution_population_breed_copy_best: best was zero");
        exit(1);
    }

    for (int i = 0; i < population->count; i++){
        Individual* current_individual = &population->pop[i];
        if (current_individual->processor == NULL){
            current_individual->processor = ppm_image_processor_copy(best->processor);
        }
    }
}
void ppm_evolution_population_survive_simple_kill_off(Population* population, int survival_rate){
    int amount_to_delete = (int)((float)population->count  / 100.0 * (float) survival_rate);
    int to_delete[amount_to_delete][2];
    for(int i = 0; i < amount_to_delete; i++){
        to_delete[i][0] = 0;
        to_delete[i][1] = 0;
    }

    for (int i = 0; i < population->count; i++){
        Individual* current_individual = &population->pop[i];
        if (current_individual == NULL){
#ifdef DEBUG
            printf("ppm_evolution_population_kill_off: Current individual was null\n");
#endif
            continue;
        }
        if (current_individual->eval == -1){
#ifdef DEBUG
            printf("ppm_evolution_population_kill_off: Current individual's eval was -1\n");
#endif
            continue;
        }

        // Loop through our list of items to be deleted, if we find one that is to be deleted we add it to the list
        for (int j = 0; j < amount_to_delete; ++j) {
            if (current_individual->eval > to_delete[j][0]){
                to_delete[j][0] = current_individual->eval;
                to_delete[j][1] = i;
                break;
            }
        }
    }

    int cur;
    // Delete items referenced in the list and set them to NULL
    for (int i = 0; i < amount_to_delete; i++){
        cur = to_delete[i][1];
        Individual* current_individual = &population->pop[cur];

        ppm_image_processor_free(current_individual->processor);
        current_individual->processor = NULL;
        current_individual->eval = -1;
    }
}

void ppm_evolution_runner(PPMImage* base_image,
                          Population* base_population,
                          RunnerParams* params
                          ){

    params->evaluation_func(base_image, base_population);
    params->survive_func(base_population, params->survival_rate);
    params->breed_func(base_population);
    params->mutation_func(base_population, params->elitist);

}

Population* ppm_evolution_population_create(int population_size, int width, int height){
    Population *population = (Population *) malloc(sizeof(Population));
    if (!population) {
        fprintf(stderr, "ppm_evolution_population_create: Failed to allocate memory for Population\n");
        exit(1);
    }
    population->count = population_size;
    population->pop = malloc(population_size * sizeof(Individual));
    if (!population->pop) {
        fprintf(stderr, "ppm_evolution_population_create: Failed to allocate memory for Individuals\n");
        free(population); // Free the Population struct before returning
        exit(1);
    }
    population->best = &population->pop[0];
    
    // Initialize the processors;
    for (int i = 0; i < population_size; ++i) {
        Individual* current_individual = &population->pop[i];
        current_individual->processor = ppm_image_processor_init(0, 0, 0, width, height);
        current_individual->eval = -1;
    }

    return population;
}

void ppm_evolution_population_del(Population* population){
    if (!population){
        fprintf(stderr, "ppm_evolution_population_del: Population pointer was NULL\n");
        return;
    }
    for (int i = 0; i < population->count; ++i) {
        ppm_image_processor_free(population->pop[i].processor);
    }
    free(population->pop);
    free(population);
};

void run(char* image_filepath, int population_size, int generation_count, int rand_seed){
    srand(rand_seed);
    PPMImage* base_image = ppm_image_load(image_filepath);
    Population* population = ppm_evolution_population_create(population_size, base_image->x, base_image->y);
    // Initialize the function bundle
    RunnerParams functions = {
            .evaluation_func = ppm_evolution_population_evaluate,
            .survive_func = ppm_evolution_population_survive_simple_kill_off,
            .survival_rate = 50,
            .mutation_func = ppm_evolution_population_mutate,
            .elitist = false,
            .breed_func = ppm_evolution_population_breed_copy_best
    };

    // Call the runner
    for (int i = 0; i < generation_count; ++i) {
        ppm_evolution_runner(base_image, population, &functions);
    }
    
    ppm_image_del(base_image);
    ppm_evolution_population_del(population);

}