#pragma once
#include <stdlib.h>


/* ================= Structure ================= */
// Keep track of statistics in sorting algorithms
typedef struct Sstat {
    int comparisons;
    int swaps;
} Sstat;

typedef Sstat* Stat;

// Structure pour une arête (lien entre deux stations)
typedef struct Edge {
    int destination;
    int time;
    struct Edge *next;
} Edge;

// Structure pour une station
typedef struct {
    int id_station;
    int degree;
    Edge *edges;  // Liste chaînée des voisins
} Station;



/* ================= Struct constructor ================= */
Stat stat_constructor();
Station* station_constructor(int id, int degree);