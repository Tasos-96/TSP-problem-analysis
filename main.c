#include  <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "headers/TSPUTILS.h"
#include "headers/LK.h"
#include "headers/VNS.h"
#include "headers/GPX.h"
#include "headers/SA2OPT.h"


int main() {
    struct Graph graph;

    char inputFilename[MAX_FILENAME_LENGTH];
    selectInputFile(inputFilename);

    readInput(&graph, inputFilename);

    printf("\nInstance loaded: %s\n", inputFilename);
    printf("Number of nodes: %d\n", graph.numNodes);

    if (graph.numNodes < 100) {
        printf("⚠️ Warning: Small instance detected (<100 nodes). VNS may be unstable without proper parameter tuning.\n");
    }

    int tour[MAX_NODES];
    for (int i = 0; i < graph.numNodes; i++) {
        tour[i] = i;
    }

    struct timeval start, end;

    int choice;
    char algorithmName[MAX_ALGORITHM_NAME];

    printf("\nSelect the desired algorithm:\n");
    printf("  1. Lin-Kernighan (LK)\n");
    printf("  2. Variable Neighborhood Search (VNS)\n");
    printf("  3. Generalized Partition Crossover (GPX)\n");
    printf("  4. Simulated Annealing on 2-Opt Algorithm (SA2OPT)\n");
    printf("Insert your choice (1, 2, 3 or 4) and press Enter: ");
    scanf("%d", &choice);
    printf("\n---- Presentation of Distances Total Tour Length and execution times  -----\n\n");

    switch (choice) {
        case 1:
            strncpy(algorithmName, "LK", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL);
            lkhAlgorithm(&graph, tour);
            gettimeofday(&end, NULL);
            break;
        case 2:
            srand(time(NULL));
            strncpy(algorithmName, "VNS", MAX_ALGORITHM_NAME);

            // ✅ Προληπτικός έλεγχος για out-of-bounds
            if (graph.numNodes > MAX_NODES) {
                printf("❌ Error: Number of nodes exceeds MAX_NODES (%d). Aborting VNS.\n", MAX_NODES);
                return 1;
            }

            // ✅ Εκτύπωση παραμέτρων πριν την εκτέλεση
            // printf("🔍 Executing VNS with parameters:\n");
            // printf("   - kmax: %d\n", kmax);
            // printf("   - maxIterations: %d\n", maxIterations);
            // printf("   - numNodes: %d\n", graph.numNodes);

            gettimeofday(&start, NULL);
            vnsAlgorithm(&graph, tour, kmax, maxIterations);
            gettimeofday(&end, NULL);

            /* ✅ Εκτύπωση πρώτων 10 κόμβων της λύσης για έλεγχο
            printf("🧪 Sample tour output (first 10 nodes): ");
            for (int i = 0; i < 10 && i < graph.numNodes; i++) {
                printf("%d ", tour[i]);
            }
            printf("\n");*/

            break;
        case 3:
            strncpy(algorithmName, "GPX", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL);
            gpcxAlgorithm(&graph, tour);
            gettimeofday(&end, NULL);
            break;
        case 4:
            strncpy(algorithmName, "SA2OPT", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL);
            twoOpt(&graph, tour);
            gettimeofday(&end, NULL);
            break;
        default:
            printf("Invalid choice. Exiting...\n");
            return 1;
    }

    double executionTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    double finalTourLength = calculateTourLength(&graph, tour);

    writeOutput(&graph, tour, finalTourLength, executionTime, algorithmName, inputFilename, "results");

    printf("\n✅ %s executed successfully, with execution time: %lf seconds\n", algorithmName, executionTime);

    return 0;
}


