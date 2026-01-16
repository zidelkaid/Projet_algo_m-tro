#pragma once
#include "structs.h"

Station* read_station_file(const char *filename, int *count);
void menu_loop(Station *stations, int n);
