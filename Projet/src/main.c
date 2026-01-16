#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structs.h"
#include "../include/menu.h"

#define LINE_SIZE 256

Station *read_station_file(const char *filename, int *count)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    int capacity = 16;
    int n = 0;
    Station *stations = malloc(sizeof(Station) * capacity);
    if (!stations) {
        fclose(fp);
        return NULL;
    }

    char line[LINE_SIZE];

    while (fgets(line, LINE_SIZE, fp)) {
        if (line[0] == '#' || line[0] == '\n')
            continue;

        if (strncmp(line, "STATION;", 8) == 0 || strncmp(line, "GARE;", 5) == 0) {
            int id;
            char name[LINE_SIZE];

            if (sscanf(line, "STATION;%d;%[^\n]", &id, name) == 2 ||
                sscanf(line, "GARE;%d;%[^\n]", &id, name) == 2) {

                if (n >= capacity) {
                    capacity *= 2;
                    Station *tmp = realloc(stations, sizeof(Station) * capacity);
                    if (!tmp) {
                        free(stations);
                        fclose(fp);
                        return NULL;
                    }
                    stations = tmp;
                }

                stations[n].id_station = id;
                stations[n].degree = 0;
                stations[n].edges = NULL;
                n++;
            }
        }
    }

    rewind(fp);
    while (fgets(line, LINE_SIZE, fp)) {
        if (line[0] == '#' || line[0] == '\n')
            continue;

        if (strncmp(line, "EDGE;", 5) == 0) {
            int id_depart, id_arrivee, temps;
            if (sscanf(line, "EDGE;%d;%d;%d", &id_depart, &id_arrivee, &temps) == 3) {
                for (int i = 0; i < n; i++) {
                    if (stations[i].id_station == id_depart) {
                        Edge *new_edge = malloc(sizeof(Edge));
                        if (new_edge) {
                            new_edge->destination = id_arrivee;
                            new_edge->time = temps;
                            new_edge->next = stations[i].edges;
                            stations[i].edges = new_edge;
                            stations[i].degree++;
                        }
                        break;
                    }
                }
            }
        }
    }

    fclose(fp);
    *count = n;
    return stations;
}

int main(int argc, char **argv) {
    Station *stations;
    int n = 0;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier.txt\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    stations = read_station_file(argv[1], &n);
    if (!stations)
        return EXIT_FAILURE;
    
    printf("Réseau chargé: %d stations\n\n", n);
    
    menu_loop(stations, n);
    
    return EXIT_SUCCESS;
}