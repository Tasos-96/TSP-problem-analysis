#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>

#include "headers/TSPUTILS.h"
#include "headers/LK.h"
#include "headers/VNS.h"
#include "headers/GPX.h"
#include "headers/SA2OPT.h"

int main() {
    struct Graph graph;
    char inputFilename[MAX_FILENAME_LENGTH];
    int tour[MAX_NODES];
    int choice;
    char algorithmName[MAX_ALGORITHM_NAME];

    printf("\nSelect execution mode:\n");
    printf("  1. Manual mode (select algorithm and instance)\n");
    printf("  2. Batch mode (run all algorithms on all instances)\n");
    printf("Insert your choice (1 or 2) and press Enter: ");
    scanf("%d", &choice);

    if (choice == 2) {
        // Batch mode
        const char *instanceFolder = "input_problems/";
        const char *algorithms[] = {"LK", "VNS", "GPX", "SA2OPT"};
        const int numAlgorithms = 4;

        struct dirent *entry;
        DIR *dp = opendir(instanceFolder);
        if (dp == NULL) {
            perror("Failed to open input_problems folder");
            return 1;
        }

        while ((entry = readdir(dp))) {
            if (strstr(entry->d_name, ".tsp")) {
                char fullPath[MAX_FILENAME_LENGTH];
                snprintf(fullPath, sizeof(fullPath), "%s%s", instanceFolder, entry->d_name);

                readInput(&graph, fullPath);
                printf("\nLoaded instance: %s\n", fullPath);
                printf("Number of nodes: %d\n", graph.numNodes);

                struct timeval mstStart, mstEnd;
                gettimeofday(&mstStart, NULL);
                double mstLength = calculateMST(&graph);
                gettimeofday(&mstEnd, NULL);
                double mstTime = (mstEnd.tv_sec - mstStart.tv_sec) + (mstEnd.tv_usec - mstStart.tv_usec) / 1000000.0;

                for (int a = 0; a < numAlgorithms; a++) {
                    for (int i = 0; i < graph.numNodes; i++) {
                        tour[i] = i;
                    }

                    struct timeval start, end;
                    gettimeofday(&start, NULL);

                    if (strcmp(algorithms[a], "LK") == 0) {
                        lkhAlgorithm(&graph, tour);
                    } else if (strcmp(algorithms[a], "VNS") == 0) {
                        vnsAlgorithm(&graph, tour, kmax, maxIterations);
                    } else if (strcmp(algorithms[a], "GPX") == 0) {
                        gpcxAlgorithm(&graph, tour);
                    } else if (strcmp(algorithms[a], "SA2OPT") == 0) {
                        twoOpt(&graph, tour);
                    }

                    gettimeofday(&end, NULL);
                    double executionTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                    double finalTourLength = calculateTourLength(&graph, tour);

                    writeOutput(&graph, tour, finalTourLength, mstLength, mstTime, executionTime, algorithms[a], fullPath, "results");

                    printf("%s on %s completed in %.6f seconds\n", algorithms[a], entry->d_name, executionTime);
                }
            }
        }

        closedir(dp);
        return 0;
    }

    // Manual mode
    selectInputFile(inputFilename);

    if (strstr(inputFilename, ".tsp") == NULL) {
        printf("Invalid file selected.\n");
        return 1;
    }

    readInput(&graph, inputFilename);
    printf("\nInstance loaded: %s\n", inputFilename);
    printf("Number of nodes: %d\n", graph.numNodes);

    if (graph.numNodes < 100) {
        printf("Warning: Small instance detected (<100 nodes). VNS may be unstable without proper parameter tuning.\n");
    }

    struct timeval mstStart, mstEnd;
    gettimeofday(&mstStart, NULL);
    double mstLength = calculateMST(&graph);
    gettimeofday(&mstEnd, NULL);
    double mstTime = (mstEnd.tv_sec - mstStart.tv_sec) + (mstEnd.tv_usec - mstStart.tv_usec) / 1000000.0;

    printf("MST Length: %.2f\n", mstLength);
    printf("MST Execution Time: %.6f seconds\n\n", mstTime);

    printf("\nSelect the desired algorithm:\n");
    printf("  1. Lin-Kernighan (LK)\n");
    printf("  2. Variable Neighborhood Search (VNS)\n");
    printf("  3. Generalized Partition Crossover (GPX)\n");
    printf("  4. Simulated Annealing on 2-Opt Algorithm (SA2OPT)\n");
    printf("Insert your choice (1-4) and press Enter: ");

    scanf("%d", &choice);

    // Εκτέλεση ενός μόνο αλγορίθμου
    struct timeval start, end;
    switch (choice) {
        case 1:
            strncpy(algorithmName, "LK", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL);
            lkhAlgorithm(&graph, tour);
            gettimeofday(&end, NULL);
            break;
        case 2:
            strncpy(algorithmName, "VNS", MAX_ALGORITHM_NAME);
            srand(time(NULL));
            gettimeofday(&start, NULL);
            vnsAlgorithm(&graph, tour, kmax, maxIterations);
            gettimeofday(&end, NULL);
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

    writeOutput(&graph, tour, finalTourLength, mstLength, mstTime, executionTime, algorithmName, inputFilename, "results");

    printf("\n%s executed successfully, with execution time: %.6f seconds\n", algorithmName, executionTime);

    return 0;
}




