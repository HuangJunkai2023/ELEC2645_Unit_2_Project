#include <stdio.h>
#include <math.h>
#include "funcs.h"
#include "helpers.h"

void menu_item_1(void) {
    char band1[20], band2[20], band3[20], band4[20];
    int val1, val2, multiplier;
    double resistance, tolerance;
    char formatted[50];
    int num_bands;
    
    printf("\n>> Resistor Colour Code Decoder\n");
    printf("\nThis utility deciphers the colour code of resistors.\n");
    printf("Supported colors: Black, Brown, Red, Orange, Yellow, Green,\n");
    printf("                  Blue, Violet/Purple, Grey/Gray, White\n");
    printf("Tolerance: Gold, Silver, Brown, Red\n\n");
    
    printf("Enter number of bands (4 or 5): ");
    if (scanf("%d", &num_bands) != 1 || (num_bands != 4 && num_bands != 5)) {
        printf("Invalid input! Please enter 4 or 5.\n");
        while (getchar() != '\n'); /* Clear input buffer */
        return;
    }
    while (getchar() != '\n'); /* Clear newline */
    
    printf("\nEnter band 1 color (first digit): ");
    if (scanf("%19s", band1) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter band 2 color (second digit): ");
    if (scanf("%19s", band2) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter band 3 color (multiplier): ");
    if (scanf("%19s", band3) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter band 4 color (tolerance): ");
    if (scanf("%19s", band4) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); /* Clear input buffer */
    
    /* Validate colors */
    val1 = get_color_value(band1);
    val2 = get_color_value(band2);
    multiplier = get_color_value(band3);
    
    if (val1 == -1 || val2 == -1 || multiplier == -1) {
        printf("\nError: Invalid color entered!\n");
        return;
    }
    
    /* Calculate resistance */
    resistance = (val1 * 10 + val2) * pow(10, multiplier);
    tolerance = get_tolerance(band4);
    
    /* Format and display result */
    format_resistance(resistance, formatted);
    
    printf("\n========================================\n");
    printf("Resistance Value: %s +/-%.1f%%\n", formatted, tolerance);
    printf("Numeric Value: %.2f Ohm\n", resistance);
    printf("========================================\n");
}
