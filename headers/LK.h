// LK.h - The header file for Lin-Kernighan algorithm
// contains functions to implement Lin-Kernighan algorithm

#ifndef LKH_H
#define LKH_H

void reverse(int *tour, int start, int end) {
    while (start < end) {
        int temp = tour[start];
        tour[start] = tour[end];
        tour[end] = temp;
        start++;
        end--;
    }
}

void lkhAlgorithm(struct Graph *graph, int *tour) {
    int maxIterations = 1000;  // Set a maximum number of iterations


    int n = graph->numNodes;
    int t1, t2, t3;

    // Generate a random initial tour
    for (int i = 0; i < n; i++) {
        tour[i] = i;
    }
    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
    }

    double pathLength = calculateTourLength(graph, tour);
    double bestLength = pathLength;


    int i = 1;
    int iterations = 0;  // Counter for iterations
    while (iterations < maxIterations) {
        t1 = i;
        // Step 3: Choose x1 = (t1,t2) ∈ T
        t2 = (t1 + 1) % n;

        // Step 4: Choose y1 = (t2,t3) ∉ T such that G1 > 0
        double maxG1 = -1.0;
        int maxG1Index = -1;
        for (t3 = 0; t3 < n; t3++) {
            if (t3 != t2 && t3 != ((t2 + 1) % n)) {
                double G1 = calculateDistance(graph->nodes[tour[t1]], graph->nodes[tour[t2]]) +
                            calculateDistance(graph->nodes[tour[t2]], graph->nodes[tour[t3]]) -
                            calculateDistance(graph->nodes[tour[t1]], graph->nodes[tour[t3]]) -
                            calculateDistance(graph->nodes[tour[(t2 + 1) % n]], graph->nodes[tour[(t2 + 2) % n]]);
                if (G1 > maxG1) {
                    maxG1 = G1;
                    maxG1Index = t3;
                }
            }
        }

        if (maxG1 <= 0) {
            // If G1 is not positive, we can't proceed, so we stop
            break;
        }

        // Step 5: Let i = i+1
        i++;

        // Step 6: Choose xi = (t2i-1,t2i) ∈ T such that (a) and (b) conditions hold
        int t2i_1 = t2;
        int t2i = t2i_1 + 1;
        if (t2i == n) {
            t2i = 0;
        }
        int xi_a = tour[t2i_1];
        int xi_b = tour[t2i];

        // Step 7: Choose yi = (t2i,t2i+1) ∉ T such that (a), (b), and (c) conditions hold
        int t2iPlus1 = t2i + 1;
        if (t2iPlus1 == n) {
            t2iPlus1 = 0;
        }
        int yi_a = tour[t2i];
        int yi_b = tour[t2iPlus1];

        // Do 2-opt move
        double originalEdge1 = calculateDistance(graph->nodes[xi_a], graph->nodes[xi_b]);
        double originalEdge2 = calculateDistance(graph->nodes[yi_a], graph->nodes[yi_b]);
        double newEdge1 = calculateDistance(graph->nodes[xi_a], graph->nodes[yi_a]);
        double newEdge2 = calculateDistance(graph->nodes[xi_b], graph->nodes[yi_b]);
        double deltaEnergy = (newEdge1 + newEdge2) - (originalEdge1 + originalEdge2);

        if (deltaEnergy < 0) {
            reverse(tour, t2, t2i);
            pathLength += deltaEnergy;
            if (pathLength < bestLength) {
                bestLength = pathLength;
                for (int j = 0; j < n; j++) {
                    tour[j] = tour[(t1 + j) % n];
                }
            }

            // Go back to Step 2
            i = 1;
        } else {
            // Go back to Step 5 and try again
            i--;
        }

        if (i == 0) {
            break; // No more improvements can be made, exit
        }

        // Increment the iteration counter
        iterations++;

        // If no improvement is found for a certain number of iterations,
        // break the loop to prevent indefinite execution
        if (iterations > maxIterations && pathLength >= bestLength) {
            break;
        }
    }
}


#endif

