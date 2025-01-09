//
// Created by Sebastian Stewart on 20/12/2024.
//

#ifndef IMAGE_EVOLUTION_PPMEVOLUTION_H
#define IMAGE_EVOLUTION_PPMEVOLUTION_H

#include "PPMImageProcessor.h"

typedef struct Individual {
    PPMImageProcessor* processor;
    int eval;
} Individual;

typedef struct Population {
    Individual* pop;
    Individual* best;
    int count;
} Population;

typedef struct RunnerParams {
    void (*evaluation_func)(PPMImage*, Population*);
    void (*survive_func)(Population*, int);
    int survival_rate;
    void (*mutation_func)(Population*, bool);
    bool elitist;
    void (*breed_func)(Population*);
} RunnerParams;

void ppm_evolution_population_evaluate(PPMImage* org, Population* comp);

#endif
