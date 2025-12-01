#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"

/* Helper function to get color value */
int get_color_value(const char* color) {
    char lower[20];
    int i;
    
    /* Convert to lowercase */
    for (i = 0; color[i] && i < 19; i++) {
        lower[i] = tolower(color[i]);
    }
    lower[i] = '\0';
    
    if (strcmp(lower, "black") == 0) return 0;
    if (strcmp(lower, "brown") == 0) return 1;
    if (strcmp(lower, "red") == 0) return 2;
    if (strcmp(lower, "orange") == 0) return 3;
    if (strcmp(lower, "yellow") == 0) return 4;
    if (strcmp(lower, "green") == 0) return 5;
    if (strcmp(lower, "blue") == 0) return 6;
    if (strcmp(lower, "violet") == 0 || strcmp(lower, "purple") == 0) return 7;
    if (strcmp(lower, "grey") == 0 || strcmp(lower, "gray") == 0) return 8;
    if (strcmp(lower, "white") == 0) return 9;
    
    return -1; /* Invalid color */
}

/* Helper function to get tolerance value */
double get_tolerance(const char* color) {
    char lower[20];
    int i;
    
    for (i = 0; color[i] && i < 19; i++) {
        lower[i] = tolower(color[i]);
    }
    lower[i] = '\0';
    
    if (strcmp(lower, "brown") == 0) return 1.0;
    if (strcmp(lower, "red") == 0) return 2.0;
    if (strcmp(lower, "gold") == 0) return 5.0;
    if (strcmp(lower, "silver") == 0) return 10.0;
    
    return 20.0; /* No band = 20% */
}

/* Helper function to format resistance with unit prefix */
void format_resistance(double resistance, char* output) {
    if (resistance >= 1e6) {
        sprintf(output, "%.2f MOhm", resistance / 1e6);
    } else if (resistance >= 1e3) {
        sprintf(output, "%.2f kOhm", resistance / 1e3);
    } else {
        sprintf(output, "%.2f Ohm", resistance);
    }
}
