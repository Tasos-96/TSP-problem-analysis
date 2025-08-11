// VNS.h - The header file for Variable Neighborhood Search (VNS) algorithm

#ifndef VNS_H
#define VNS_H

int kmax = 10;  // Maximum shaking intensity
int maxIterations = 100; // Maximum number of iterations

/* void twoOptNeighborhoodChange(struct Graph *graph, int *tour, int i, int j) {
    // Implement the 2-opt neighborhood change operation.
    // Reverse the portion of the tour from index i to j.
    if (i >= j || i < 0 || j >= graph->numNodes) {
        // Invalid indices
        return;
    }

    // Reverse the tour segment from index i to j
    while (i < j) {
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
        i++;
        j--;
    }
}*/

void twoOptNeighborhoodChange(struct Graph *graph, int *tour, int i, int j) {
    if (i >= j || i < 0 || j >= graph->numNodes) {
        // printf("❌ twoOptNeighborhoodChange invalid indices: i=%d, j=%d, numNodes=%d\n", i, j, graph->numNodes);
        return;
    }

    while (i < j) {
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
        i++;
        j--;
    }
}


void subMSTNeighborhoodChange(struct Graph *graph, int *tour, int k) {
    if (k <= 1 || k >= graph->numNodes) {
        // Invalid sub-MST size
        return;
    }

    // Generate a random starting index
    int startIdx = rand() % graph->numNodes;

    // Create a sub-MST of size k starting from the randomly selected index
    int subMST[MAX_NODES];
    int count = 0;
    for (int i = 0; i < k; i++) {
        subMST[i] = tour[(startIdx + i) % graph->numNodes];
        count++;
    }

    // Rearrange the sub-MST nodes randomly
    for (int i = 0; i < count - 1; i++) {
        int j = i + rand() % (count - i);
        int temp = subMST[i];
        subMST[i] = subMST[j];
        subMST[j] = temp;
    }

    // Update the original tour with the new sub-MST
    for (int i = 0; i < k; i++) {
        tour[(startIdx + i) % graph->numNodes] = subMST[i];
    }
}

// Shake the current tour to generate a new one
void shake(struct Graph *graph, int *tour, int k, int numNodes) {
    // Implement the shaking operation (e.g., 2-opt or sub-MST).
    int choice = rand() % 2; // Randomly choose between 2-opt and sub-MST

    if (choice == 0) {
        // 2-opt neighborhood change
        int i = rand() % numNodes;
        int j;
        do {
            j = rand() % numNodes;
        } while (abs(i - j) < 2);
        twoOptNeighborhoodChange(graph, tour, i, j);
    } else {
        // Sub-MST neighborhood change
        subMSTNeighborhoodChange(graph, tour, k);
    }
}


/* bool improveTour(struct Graph *graph, int *tour, int numNodes, int i, int j) {
    // Check if swapping edges (i, i+1) and (j, j+1) will lead to a shorter tour
    double dist_before = calculateDistance(graph->nodes[tour[i]], graph->nodes[tour[i + 1]])
                         + calculateDistance(graph->nodes[tour[j]], graph->nodes[tour[j + 1]]);

    double dist_after = calculateDistance(graph->nodes[tour[i]], graph->nodes[tour[j]])
                        + calculateDistance(graph->nodes[tour[i + 1]], graph->nodes[tour[j + 1]]);

    return dist_after < dist_before;
}*/

bool improveTour(struct Graph *graph, int *tour, int numNodes, int i, int j) {
    if (i + 1 >= numNodes || j + 1 >= numNodes) {
        // printf("❌ improveTour out-of-bounds: i=%d, j=%d, numNodes=%d\n", i, j, numNodes);
        return false;
    }

    double dist_before = calculateDistance(graph->nodes[tour[i]], graph->nodes[tour[i + 1]])
                       + calculateDistance(graph->nodes[tour[j]], graph->nodes[tour[j + 1]]);

    double dist_after = calculateDistance(graph->nodes[tour[i]], graph->nodes[tour[j]])
                      + calculateDistance(graph->nodes[tour[i + 1]], graph->nodes[tour[j + 1]]);

    return dist_after < dist_before;
}


/*void reverseSegment(int *tour, int i, int j, int numNodes) {
    // Reverse the portion of the tour from index i to j
    while (i < j) {
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
        i++;
        j--;
    }
}*/

void reverseSegment(int *tour, int i, int j, int numNodes) {
    if (i < 0 || j >= numNodes || i >= j) {
        // printf("❌ reverseSegment invalid indices: i=%d, j=%d, numNodes=%d\n", i, j, numNodes);
        return;
    }

    while (i < j) {
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
        i++;
        j--;
    }
}


void twoOptLocalSearch(struct Graph *graph, int *tour, int numNodes) {
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i < numNodes - 2; i++) {
            for (int j = i + 2; j < numNodes - 1; j++) {
                // Check if reversing the segment between indices i and j improves the tour
                if (improveTour(graph, tour, numNodes, i, j)) {
                    // Reverse the segment
                    reverseSegment(tour, i + 1, j, numNodes - 1);
                    improved = true;
                }
            }
        }
    }
}


void vnsAlgorithm(struct Graph *graph, int *tour, int kmax, int maxIterations) {
    int k = 1;
    int iteration = 0;
    int numNodes = graph->numNodes;

    while (iteration < maxIterations) {
        while (k <= kmax) {
            int currentTour[MAX_NODES];
            for (int i = 0; i < numNodes; i++) {
                currentTour[i] = tour[i];
            }

            // Shake the tour
            shake(graph, currentTour, k, numNodes);

            // Perform local search on the shaken tour using 2-opt
            // 2-opt is a common local search algorithm for TSP
            twoOptLocalSearch(graph, currentTour, numNodes);

            // Update the tour if a better solution is found
            double currentLength = calculateTourLength(graph, currentTour);
            double bestLength = calculateTourLength(graph, tour);

            if (currentLength < bestLength) {
                for (int i = 0; i < numNodes; i++) {
                    tour[i] = currentTour[i];
                }
                k = 1; // Reset k
            } else {
                k++; // Increment k
            }
        }

        iteration++;
    }
}


#endif

