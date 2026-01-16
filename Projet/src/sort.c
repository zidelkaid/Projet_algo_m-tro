#include <stdlib.h>
#include "../include/sort.h"
#include "../include/utils.h"
#include "../include/structs.h"

/* ================= Swap station ================= */
// Swap two Station elements
static void swap_station(Station *a, Station *b)
{
    Station temp_array = *a;
    *a = *b;
    *b = temp_array;
}


/* ================= SELECTION SORT ================= */
// Selection sort for Station array
static Stat selection_sort_station(Station *station_list, int n)
{
    if (!station_list || n <= 1)
        return NULL;

    Stat stat = stat_constructor();

    for (int i = 0; i < n - 1; i++) {
        int min = i;

        for (int j = i + 1; j < n; j++) {
            stat->comparisons++;
            if (station_list[j].degree < station_list[min].degree)
                min = j;
        }

        if (min != i) {
            swap_station(&station_list[i], &station_list[min]);
            stat->swaps++;
        }
    }
    return stat;
}


/* ================= INSERTION SORT ================= */
// Insertion sort function
static Stat insertion_sort_station(Station *station_list, int n)
{
    if (!station_list || n <= 1)
        return NULL;

    Stat stat = stat_constructor();

    for (int i = 1; i < n; i++) {
        Station key = station_list[i];
        int j = i - 1;

        while (j >= 0) {
            stat->comparisons++;
            if (station_list[j].degree > key.degree) {
                station_list[j + 1] = station_list[j];
                stat->swaps++;
                j--;
            } else {
                break;
            }
        }
        station_list[j + 1] = key;
    }
    return stat;
}



/* ================= BUBBLE SORT ================= */
// Bubble sort function
static Stat bubble_sort_station(Station *station_list, int n)
{
    if (!station_list || n <= 1)
        return NULL;

    Stat stat = stat_constructor();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            stat->comparisons++;
            if (station_list[j].degree > station_list[j + 1].degree) {
                swap_station(&station_list[j], &station_list[j + 1]);
                stat->swaps++;
            }
        }
    }
    return stat;
}


/* ================= MERGE SORT ================= */
// Merge two halves
static void merge_station(Station *station_list, Station *temp_array, int left, int mid, int right, Stat stat)
{
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        stat->comparisons++;
        if (station_list[i].degree <= station_list[j].degree)
            temp_array[k++] = station_list[i++];
        else
            temp_array[k++] = station_list[j++];
    }

    while (i <= mid)
        temp_array[k++] = station_list[i++];

    while (j <= right)
        temp_array[k++] = station_list[j++];
}


// Recursive merge sort function
static void merge_sort_station_rec(Station *station_list, Station *temp_array, int left, int right, Stat stat)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    merge_sort_station_rec(station_list, temp_array, left, mid, stat);
    merge_sort_station_rec(station_list, temp_array, mid + 1, right, stat);
    merge_station(station_list, temp_array, left, mid, right, stat);

    for (int i = left; i <= right; i++)
        station_list[i] = temp_array[i];
}


// Merge sort main function
static Stat merge_sort_station(Station *station_list, int n)
{
    if (!station_list || n <= 1)
        return NULL;

    Stat stat = stat_constructor();
    Station *temp_array = malloc(sizeof(Station) * n);
    if (!temp_array) {
        free(stat);
        return NULL;
    }

    merge_sort_station_rec(station_list, temp_array, 0, n - 1, stat);
    free(temp_array);
    return stat;
}


/* ================= QUICK SORT ================= */
// Partition function for quick sort
static int partition_station(Station *station_list, int left, int right, Stat stat)
{
    Station pivot = station_list[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        stat->comparisons++;
        if (station_list[j].degree <= pivot.degree) {
            i++;
            swap_station(&station_list[i], &station_list[j]);
            stat->swaps++;
        }
    }
    swap_station(&station_list[i + 1], &station_list[right]);
    stat->swaps++;

    return i + 1;
}


// Recursive quick sort function
static void quick_sort_station_rec(Station *station_list, int left, int right, Stat stat)
{
    if (left < right) {
        int p = partition_station(station_list, left, right, stat);
        quick_sort_station_rec(station_list, left, p - 1, stat);
        quick_sort_station_rec(station_list, p + 1, right, stat);
    }
}

// Quick sort main function
static Stat quick_sort_station(Station *station_list, int n)
{
    if (!station_list || n <= 1)
        return NULL;

    Stat stat = stat_constructor();
    quick_sort_station_rec(station_list, 0, n - 1, stat);
    return stat;
}



/* ================= SORT STATIONS BY DEGREE ================= */
Stat sort_stations_by_degree(Station *stations, int n, int method)
{
    switch (method) {
        case 1:
            return selection_sort_station(stations, n);
        case 2:
            return insertion_sort_station(stations, n);
        case 3:
            return bubble_sort_station(stations, n);
        case 4:
            return merge_sort_station(stations, n);
        case 5:
            return quick_sort_station(stations, n);
        default:
            return NULL;
    }
}
