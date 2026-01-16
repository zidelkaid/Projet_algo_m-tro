#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/menu.h"
#include "../include/sort.h"
#include "../include/structs.h"

// Display the menu options
static void show_menu() {
    printf("==== MENU RESEAU DE TRANSPORT ====\n");
    printf("1 - Afficher les informations d'une station\n");
    printf("2 - Lister les voisins d'une station\n");
    printf("3 - Calculer un chemin minimal\n");
    printf("4 - Afficher les stations triées par degré\n");
    printf("0 - Quitter\n");
    printf("Votre choix : ");
}

// Print the list of stations
static void print_stations(const Station *station_array, int n)
{
    for (int i = 0; i < n; i++) {
        printf("Station %d -> degree %d\n",
               station_array[i].id_station,
               station_array[i].degree);
    }
}

// Fonction pour afficher les informations d'une station
static void show_station_info(Station *stations, int n) {
    int id;
    
    printf("Entrez l'identifiant de la station : ");
    scanf("%d", &id);
    
    // Chercher la station
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (stations[i].id_station == id) {
            printf("Identifiant : %d\n", stations[i].id_station);
            printf("Degré sortant : %d\n", stations[i].degree);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Station non trouvée.\n");
    }
}

// Affiche tous les voisins d'une station avec le temps de trajet
static void display_neighbors(Station *stations, int n) {
    int id;
    Station *station = NULL;
    
    printf("Entrez l'identifiant de la station : ");
    scanf("%d", &id);
    
    // Chercher la station
    for (int i = 0; i < n; i++) {
        if (stations[i].id_station == id) {
            station = &stations[i];
            break;
        }
    }
    
    if (station == NULL) {
        printf("Station non trouvée.\n");
        return;
    }
    
    if (station->degree == 0) {
        printf("Aucun voisin.\n");
        return;
    }
    
    printf("Voisins de la station %d :\n", station->id_station);
    
    Edge *current = station->edges;
    while (current != NULL) {
        printf("  -> Station %d (temps: %d min)\n", 
            current->destination, current->time);
        current = current->next;
    }
}

// trouver l'index d'une station
static int trouver_index(Station *stations, int n, int id) {
    int i;
    for (i = 0; i < n; i++) {
        if (stations[i].id_station == id) {
            return i;
        }
    }
    return -1;
}

// calculer le plus court chemin avec dijkstra
static void calculer_chemin(Station *stations, int n) {
    int i, j;
    int depart, arrivee;
    int src_idx, dest_idx;
    int *distances;
    int *precedents;
    int *visite;
    int min_dist, min_idx;
    Edge *voisin;
    int nouvelle_dist;
    
    printf("Station de départ : ");
    scanf("%d", &depart);
    printf("Station d'arrivée : ");
    scanf("%d", &arrivee);
    
    // Allocation dynamique
    distances = malloc(n * sizeof(int));
    precedents = malloc(n * sizeof(int));
    visite = malloc(n * sizeof(int));
    
    // trouver les indices
    src_idx = trouver_index(stations, n, depart);
    dest_idx = trouver_index(stations, n, arrivee);
    
    if (src_idx == -1 || dest_idx == -1) {
        printf("Une des stations n'existe pas\n");
        free(distances);
        free(precedents);
        free(visite);
        return;
    }
    
    // initialisation
    for (i = 0; i < n; i++) {
        distances[i] = INT_MAX;
        precedents[i] = -1;
        visite[i] = 0;
    }
    distances[src_idx] = 0;
    
    // algorithme principal
    for (i = 0; i < n - 1; i++) {
        // chercher le min
        min_dist = INT_MAX;
        min_idx = -1;
        for (j = 0; j < n; j++) {
            if (visite[j] == 0 && distances[j] < min_dist) {
                min_dist = distances[j];
                min_idx = j;
            }
        }
        
        if (min_idx == -1) {
            break;
        }
        
        visite[min_idx] = 1;
        
        // regarder les voisins
        voisin = stations[min_idx].edges;
        while (voisin != NULL) {
            int v_idx = trouver_index(stations, n, voisin->destination);
            if (v_idx != -1 && visite[v_idx] == 0) {
                nouvelle_dist = distances[min_idx] + voisin->time;
                if (nouvelle_dist < distances[v_idx]) {
                    distances[v_idx] = nouvelle_dist;
                    precedents[v_idx] = min_idx;
                }
            }
            voisin = voisin->next;
        }
    }
    
    // affichage du résultat
    if (distances[dest_idx] == INT_MAX) {
        printf("Pas de chemin trouvé\n");
        free(distances);
        free(precedents);
        free(visite);
        return;
    }
    
    // reconstruire le chemin
    int *chemin = malloc(n * sizeof(int));
    int taille = 0;
    int actuel = dest_idx;
    while (actuel != -1) {
        chemin[taille] = actuel;
        taille++;
        actuel = precedents[actuel];
    }
    
    // afficher
    printf("Temps total: %d minutes\n", distances[dest_idx]);
    printf("Chemin: ");
    for (i = taille - 1; i >= 0; i--) {
        printf("%d", stations[chemin[i]].id_station);
        if (i != 0) {
            printf(" -> ");
        }
    }
    printf("\n");
    
    free(distances);
    free(precedents);
    free(visite);
    free(chemin);
}

// Handle the user's menu choice
static void handle_choice(int choice, Station *stations, int n) {
    switch (choice) {
        case 1:
            show_station_info(stations, n);
            break;

        case 2:
            display_neighbors(stations, n);
            break;

        case 3:
            calculer_chemin(stations, n);
            break;

        case 4: {
            // Change the function here to test other sorts
            Stat stat = sort_stations_by_degree(stations, n, 5); // 1: Selection, 2: Insertion, 3: Bubble, 4: Merge, 5: Quick

            printf("\nSorted stations (by degree):\n");
            print_stations(stations, n);

            if (stat) {
                printf("\nComparisons: %d\n", stat->comparisons);
                printf("Swaps: %d\n", stat->swaps);
                free(stat);
            }
            break;
        }

        case 0:
            printf("Au revoir!\n");
            break;

        default:
            printf("Choix invalide. Veuillez réessayer.\n");
    }
}

// Call after data from text file is loaded and processed
void menu_loop(Station *stations, int n) {
    int choice = -1;

    // Main loop
    while (choice != 0) {
        show_menu();
        scanf("%d", &choice);
        handle_choice(choice, stations, n);
    }

    free(stations);
}