#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sort.h"
#include "../include/structs.h"
#include "../include/menu.h"

#define LINE_SIZE 256

/* Read STATION lines and build a Station station_arrayay */
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

        /* Ignore comments and empty lines */
        if (line[0] == '#' || line[0] == '\n')
            continue;

        if (strncmp(line, "STATION;", 8) == 0) {
            int id;
            char name[LINE_SIZE];

            /* Parse: STATION;id;name */
            if (sscanf(line, "STATION;%d;%[^\n]", &id, name) == 2) {

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

                /* Fake degree for testing (deterministic) */
                // Need to replace with real degree from text file later
                stations[n].degree = (id * 3) % 7 + 1;

                n++;
            }
        }
    }

    fclose(fp);
    *count = n;
    return stations;
}

static void print_stations(const Station *station_array, int n)
{
    for (int i = 0; i < n; i++) {
        printf("Station %d -> degree %d\n", station_array[i].id_station, station_array[i].degree);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = 0;
    Station *stations = read_station_file(argv[1], &n);
    if (!stations)
        return EXIT_FAILURE;

    printf("Original stations:\n");
    print_stations(stations, n);

    menu_loop(stations, n);

    
    return EXIT_SUCCESS;
}
