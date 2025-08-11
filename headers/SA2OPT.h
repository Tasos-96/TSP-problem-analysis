//// SA2OPT.h - The header file for Simulated Annealing (SA) on 2-Opt algorithm

#ifndef SA2OPT_H
#define SA2OPT_H

#define INITIAL_TEMPERATURE 1000.0
#define COOLING_RATE 0.999
#define MIN_TEMPERATURE 0.01
#define MAX_ITERATIONS 1000



void swapCities(int tour[], int i, int k) {
    while (i < k) {
        int temp = tour[i];
        tour[i] = tour[k];
        tour[k] = temp;
        i++;
        k--;
    }
}

double twoOptDeltaEnergy(struct Graph *graph, int *tour, int i, int k) {
    int numNodes = graph->numNodes;
    int a = tour[i];
    int b = tour[(i + 1) % numNodes];
    int c = tour[k];
    int d = tour[(k + 1) % numNodes];

    double originalEdge1 = calculateDistance(graph->nodes[a], graph->nodes[b]);
    double originalEdge2 = calculateDistance(graph->nodes[c], graph->nodes[d]);
    double newEdge1 = calculateDistance(graph->nodes[a], graph->nodes[c]);
    double newEdge2 = calculateDistance(graph->nodes[b], graph->nodes[d]);

    return (newEdge1 + newEdge2) - (originalEdge1 + originalEdge2);
}

void twoOpt(struct Graph *graph, int *tour) {
    int numNodes = graph->numNodes;
    double initialTourLength = calculateTourLength(graph, tour);
    double temperature = INITIAL_TEMPERATURE;

    while (temperature > MIN_TEMPERATURE) {
        for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
            int i = rand() % numNodes;
            int k = rand() % numNodes;
            while (k == i)
                k = rand() % numNodes;

            double deltaEnergy = twoOptDeltaEnergy(graph, tour, i, k);

            if (deltaEnergy < 0 || (rand() / (double)RAND_MAX) < exp(-deltaEnergy / temperature)) {
                swapCities(tour, i + 1, k);
            }
        }

        temperature *= COOLING_RATE;
    }
}


#endif

