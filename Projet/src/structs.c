#include <stdlib.h>
#include "../include/structs.h"

/* ================= Stat constructor ================= */
// Create and initialize a Stat structure
Stat stat_constructor(void)
{
    Stat s = malloc(sizeof(Sstat));
    s->comparisons = 0;
    s->swaps = 0;
    return s;
}

Station* station_constructor(int id, int degree)
// Create and initialize a Station structure
{
    Station *s = malloc(sizeof(Station));
    if (!s)
        return NULL;

    s->id_station = id;
    s->degree = degree;
    return s;
}

// Constructeur pour une arête
Edge* edge_constructor(int destination, int time) {
    Edge *e = malloc(sizeof(Edge));
    if (!e)
        return NULL;
    
    e->destination = destination;
    e->time = time;
    e->next = NULL;
    return e;
}

