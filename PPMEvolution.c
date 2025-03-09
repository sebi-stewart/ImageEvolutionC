//
// Created by Sebastian Stewart on 20/12/2024.
//

#include "PPMEvolution.h"
#include <stdio.h>
#include <stdlib.h>

void ppm_evolution_population_evaluate(PPMImage* org, Population* comparison){
    #pragma omp parallel for
    for (int i = 0; i < comparison->count; ++i) {
        #ifdef DEBUG_VERBOSE
        printf("%d: ", i);
        #endif
        Individual* current_individual = &comparison->pop[i];
        if (current_individual == NULL){
            #ifdef DEBUG
            printf("ppm_evolution_population_evaluate: Current individual was null\n");
            #endif
            continue;
        }
        if (current_individual->eval != -1){
            #ifdef DEBUG_VERBOSE
            printf("\tppm_evolution_population_evaluate: No changes, not evaluating\n");
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

        PPMImage* cur_image = ppm_image_processor_draw_polygons_alt(current_processor, false);
        current_individual->eval = ppm_image_compare(org, cur_image);
        ppm_image_del(cur_image);
    }

    // Find the best individual (serial step)
    Individual* best_individual = comparison->pop;
    for (int i = 1; i < comparison->count; ++i) {
        Individual* current_individual = &comparison->pop[i];
        if (current_individual == NULL){
            #ifdef DEBUG
            printf("ppm_evolution_population_evaluate: Current individual was null\n");
            #endif
            continue;
        }
        if (current_individual->eval < best_individual->eval) {
            best_individual = current_individual;
        }
    }
    comparison->best = best_individual;
}

float rand_float(){
    return (float)rand() / (float)RAND_MAX;
}

int rand_int(int max){
    if (max <= 0){
        fprintf(stderr, "rand_int: max must be greater than 0\n");
        exit(1);
    }

    int valid_range = RAND_MAX - (RAND_MAX % max);
    int rand_val;
    do {
        rand_val = rand();
    } while (rand_val >= valid_range);
    return rand_val % max;
}

Polygon* ppm_evolution_random_polygon(PPMImageProcessor* processor){
    if (processor == NULL){
        fprintf(stderr, "ppm_evolution_random_polygon: no processor to mutate\n");
        exit(1);
    }
    if (processor->polygons == NULL) {
        fprintf(stderr, "ppm_evolution_random_polygon: no polygon to choose from\n");
        exit(1); // No polygons in the list
    }

    unsigned int polygon_count = processor->polygon_count - 1;

    // Generate a random index between 0 and polygon_count - 1
    int random_index = (int)(rand_float() * (float)polygon_count);

    // Traverse the list to the random index
    Polygon* selected_polygon = processor->polygons;
    for (int i = 0; i < random_index; i++) {
        selected_polygon = selected_polygon->next;
    }
    return selected_polygon;
}

int narrow_coords(int coord, int max_val){
    if (coord < 0){
        return 0;
    }
    if (coord > max_val){
        return max_val;
    }
    return coord;
}

void ppm_evolution_mutate_polygon_points(PPMImageProcessor* processor){
    if (processor->polygons == NULL) {
#ifdef DEBUG_VERBOSE
        fprintf(stdout, "ppm_evolution_mutate_polygon_points: no polygon to mutate\n");
#endif
        return; // No polygons in the list
    }

    Polygon* random_polygon = ppm_evolution_random_polygon(processor);
    double coord;
    for (Corner* cur = random_polygon->corners; cur != NULL; cur = cur->next){
        if (rand_float() > 0.5){
            coord = (double)cur->x - (rand_float() * 5 - 2.5);
            cur->x = narrow_coords((int)coord, (int)processor->width);
        } else {
            coord = (double)cur->y - (rand_float() * 5 - 2.5);
            cur->y = narrow_coords((int)coord, (int)processor->height);
        }
    }
}

int narrow_colors(double color){
    if (color <= 0){
        return 0;
    } if (color >= PIXEL_COLOR_VALUE){
        return PIXEL_COLOR_VALUE;
    } return (int) color;
}

void ppm_evolution_mutate_polygon_colors(PPMImageProcessor* processor){
    if (processor->polygons == NULL) {
#ifdef DEBUG_VERBOSE
        fprintf(stdout, "ppm_evolution_mutate_polygon_colors: no polygon to mutate\n");
#endif
        return; // No polygons in the list
    }

    Polygon* random_polygon = ppm_evolution_random_polygon(processor);
    PPMPixel* polygon_color = random_polygon->color;
    double color;
    float random_float = rand_float();
    if (random_float < (1.0/3.0)){
        color = (double)polygon_color->R - (rand_float() * 10 - 5);
        polygon_color->R = narrow_colors(color);
    }
    else if (random_float > (2.0/3.0)){
        color = (double)polygon_color->G - (rand_float() * 10 - 5);
        polygon_color->G = narrow_colors(color);
    } else {
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
        fprintf(stderr, "ppm_evolution_mutate_add_polygon: no individual to mutate\n");
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
    if (processor->polygons == NULL){
#ifdef DEBUG_VERBOSE
        fprintf(stdout, "ppm_evolution_mutate_remove_polygon: no polygons to mutate\n");
#endif
        return;
    }
    Polygon* random_polygon = ppm_evolution_random_polygon(processor);
    if (random_polygon == NULL){
#ifdef DEBUG
        fprintf(stderr, "ppm_evolution_mutate_remove_polygon: NULL was returned for random_polygon\n");
#endif
        return;
    }
    pop_all_corners(random_polygon);
    pop_polygon(processor, random_polygon);
}

void ppm_evolution_mutate_background_colour(PPMImageProcessor* processor){
    PPMPixel* background = processor->background;
    double color;
    float random_float = rand_float();
    if (random_float < (1.0/3.0)){
        color = (double)background->R - (rand_float() * 10 - 5);
        background->R = narrow_colors(color);
    }
    else if (random_float > (2.0/3.0)){
        color = (double)background->G - (rand_float() * 10 - 5);
        background->G = narrow_colors(color);
    } else {
        color = (double)background->B - (rand_float() * 10 - 5);
        background->B = narrow_colors(color);
    }

}

void ppm_evolution_population_mutate(Population* population, bool elitist, int generation){
    float multiplier = 2.0f/(float)generation + 0.5f;
    for (int i = 0; i < population->count; i++){
        Individual* current_individual = &population->pop[i];
        if (elitist && current_individual == population->best){
            continue;
        }
        if (current_individual == NULL) {
#ifdef DEBUG
            fprintf(stderr, "ppm_evolution_population_mutate: no individual to mutate\n");
#endif
            continue;
        }
        PPMImageProcessor* current_processor = current_individual->processor;
        if (current_processor == NULL) {
#ifdef DEBUG
            fprintf(stderr, "ppm_evolution_population_mutate: no processor to mutate\n");
#endif
            continue;
        }

        if (rand_float() * multiplier > 0.8){
            ppm_evolution_mutate_background_colour(current_processor);
        }

        if (current_processor->polygons != NULL) {
            if (rand_float()*multiplier < 0.3) {
                ppm_evolution_mutate_polygon_points(current_processor);
                current_individual->eval = -1;
            }
            if (rand_float()*multiplier < 0.3) {
                ppm_evolution_mutate_polygon_colors(current_processor);
                current_individual->eval = -1;
            }
            if (rand_float()*multiplier < 0.005) {
                ppm_evolution_mutate_remove_polygon(current_processor);
                current_individual->eval = -1;
            }
        }
        if (rand_float()*multiplier < 0.025){
            ppm_evolution_mutate_add_polygon(current_processor);
            current_individual->eval = -1;
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
            current_individual->eval = best->eval;
        }
    }
}

void ppm_evolution_population_survive_trim_list(ToDelete* head, int amount_to_delete){
    if (head == NULL){
#ifdef DEBUG
        fprintf(stdout, "ppm_evolution_population_survive_trim_list: head was NULL\n");
        exit(1);
#endif
    }

    // Trim the linked list
    int count = 1;
    ToDelete* current = head;

    // Traverse the list until the amount_to_delete node or the end of the list
    while (current != NULL && count < amount_to_delete) {
        count++;
        current = current->next;
    }

    // Delete the rest
    if (current == NULL){
#ifdef DEBUG
        fprintf(stdout, "ppm_evolution_population_survive_trim_list: current was NULL after traversing amount_to_delete\n");
        exit(1);
#endif
    }

    ToDelete* rest = current->next;
    current->next = NULL; // Disconnect the lists

    while(rest != NULL){ // Free ToDelete Memory of the items that are going to stay
        ToDelete* temp = rest;
        rest = rest->next;
        free(temp);
    }
}

void ppm_evolution_population_survive_simple_kill_off(Population* population, int survival_rate){
    int amount_to_delete = (int)((float)population->count  / 100.0 * (float) survival_rate) -1;
    if (amount_to_delete <= 0 || amount_to_delete >= population->count){
#ifdef DEBUG
        fprintf(stdout, "ppm_evolution_population_survive_simple_kill_off: 0 elements to delete\n");
        return;
#endif
    }

    // Create a sorted linked list of all individuals;
    ToDelete* to_delete_head = NULL;
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
        if (current_individual == population->best){
#ifdef DEBUG_VERBOSE
            printf("ppm_evolution_population_kill_off: Individual was the best, skipping\n");
#endif
            continue;
        }

        ToDelete* new_node = (ToDelete*) malloc(sizeof(ToDelete));
        new_node->eval = current_individual->eval;
        new_node->individual_id = i;
        new_node->next = NULL;
        if (to_delete_head == NULL){
            // Insert at the beginning of the list on the first run
            to_delete_head = new_node;
        } else if (new_node->eval > to_delete_head->eval) {
            // Insert at the beginning if the new_node has the highest eval
            new_node->next = to_delete_head;
            to_delete_head = new_node;
        } else { // Traverse the list to find correct location
            ToDelete* to_delete_current = to_delete_head;

            while (to_delete_current->next != NULL && to_delete_current->next->eval > new_node->eval) {
                to_delete_current = to_delete_current->next;
            }

            new_node->next = to_delete_current->next;
            to_delete_current->next = new_node;
        }
    }

    ppm_evolution_population_survive_trim_list(to_delete_head, amount_to_delete);

#ifdef DEBUG_VERBOSE
    printf("Amount to delete: %d\n", amount_to_delete);
    printf("[\n");
    for (ToDelete* current = to_delete_head; current != NULL; current = current->next){
        printf("[%d, %d]\n", current->individual_id, current->eval);
    }
    printf("]\n");
#endif


    int delete_id;
    // Delete items referenced in the list and set them to NULL
    for (ToDelete* current = to_delete_head; current != NULL; current = current->next){
        delete_id = current->individual_id;
        Individual* current_individual = &population->pop[delete_id];

        ppm_image_processor_free(current_individual->processor);
        current_individual->processor = NULL;
        current_individual->eval = -1;
    }

    // Free leftover ToDelete items
    ToDelete* current = to_delete_head;
    while(current != NULL){
        ToDelete* temp = current;
        current = current->next;
        free(temp);
    }
}

void ppm_evolution_runner(PPMImage* base_image,
                          Population* base_population,
                          RunnerParams* params,
                          int generation
                          ){
    params->survive_func(base_population, params->survival_rate);

    params->breed_func(base_population);

    params->mutation_func(base_population, params->elitist, generation);

    params->evaluation_func(base_image, base_population);
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
        current_individual->processor = ppm_image_processor_init(rand_int(256),
                                                                 rand_int(256),
                                                                 rand_int(256),
                                                                 width, height);
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
    ppm_evolution_population_evaluate(base_image, population);
    // Initialize the function bundle
    RunnerParams functions = {
            .evaluation_func = ppm_evolution_population_evaluate,
            .survive_func = ppm_evolution_population_survive_simple_kill_off,
            .survival_rate = 50,
            .mutation_func = ppm_evolution_population_mutate,
            .elitist = true,
            .breed_func = ppm_evolution_population_breed_copy_best
    };

    // Call the runner
    int const MAX_EVAL = base_image->x * base_image->y * 3 * PIXEL_COLOR_VALUE;
    printf("Generation %5c | Best\n", ' ');
    for (int i = 1; i < generation_count; i++) {
        ppm_evolution_runner(base_image, population, &functions, generation_count);
        if (i % (generation_count/100) == 0){
            printf("Generation %5d | %3.4f\n", i, (double)(MAX_EVAL-population->best->eval)/(double)MAX_EVAL * 100);
        }
    }

    printf("Best image fitness of: %3.4f\n", (double)(MAX_EVAL-population->best->eval)/(double)MAX_EVAL * 100);
    PPMImage* best_image = ppm_image_processor_draw_polygons_alt(population->best->processor, false);
    ppm_image_save(best_image, "best.ppm");
    ppm_image_del(best_image);

#ifdef DEBUG_VERBOSE
    PPMPixel* background = population->best->processor->background;
    printf("Background colour %d, %d, %d\n", background->R, background->G, background->B);
    print_image_processor(population->best->processor);
#endif

    
    ppm_image_del(base_image);
    ppm_evolution_population_del(population);
}

int main(){
    run("images/originals/original.ppm", 160, 100, 1);

//    PPMImage* best_image = ppm_image_load("images/7a.ppm");
//    ppm_image_save(best_image, "best.ppm");
//    ppm_image_del(best_image);
    return 0;
}