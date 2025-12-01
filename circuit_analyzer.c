#include <stdio.h>
#include "funcs.h"

void menu_item_3(void) {
    int circuit_type, num_resistors, i;
    double resistors[10];
    double total_resistance = 0.0;
    double voltage, total_current, total_power;
    
    printf("\n>> Basic Circuit Analyser\n");
    printf("\nAnalyse simple electrical circuits.\n\n");
    
    printf("Select circuit type:\n");
    printf("1. Series resistors\n");
    printf("2. Parallel resistors\n");
    printf("Enter choice: ");
    if (scanf("%d", &circuit_type) != 1 || (circuit_type != 1 && circuit_type != 2)) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("\nEnter number of resistors (2-10): ");
    if (scanf("%d", &num_resistors) != 1 || num_resistors < 2 || num_resistors > 10) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    /* Input resistor values */
    printf("\nEnter resistor values (in Ohm):\n");
    for (i = 0; i < num_resistors; i++) {
        printf("R%d: ", i + 1);
        if (scanf("%lf", &resistors[i]) != 1 || resistors[i] <= 0) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            return;
        }
    }
    
    printf("\nEnter supply voltage (V): ");
    if (scanf("%lf", &voltage) != 1 || voltage <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    /* Calculate total resistance */
    if (circuit_type == 1) {
        /* Series: R_total = R1 + R2 + ... + Rn */
        total_resistance = 0.0;
        for (i = 0; i < num_resistors; i++) {
            total_resistance += resistors[i];
        }
    } else {
        /* Parallel: 1/R_total = 1/R1 + 1/R2 + ... + 1/Rn */
        double reciprocal_sum = 0.0;
        for (i = 0; i < num_resistors; i++) {
            reciprocal_sum += 1.0 / resistors[i];
        }
        total_resistance = 1.0 / reciprocal_sum;
    }
    
    /* Calculate current and power */
    total_current = voltage / total_resistance;
    total_power = voltage * total_current;
    
    /* Display results */
    printf("\n========================================\n");
    printf("Circuit Analysis Results\n");
    printf("========================================\n");
    printf("Circuit Type: %s\n", circuit_type == 1 ? "Series" : "Parallel");
    printf("Number of Resistors: %d\n", num_resistors);
    printf("\nResistor Values:\n");
    for (i = 0; i < num_resistors; i++) {
        printf("  R%d = %.2f Ohm\n", i + 1, resistors[i]);
    }
    printf("\n----------------------------------------\n");
    printf("Total Resistance: %.2f Ohm\n", total_resistance);
    printf("Supply Voltage: %.2f V\n", voltage);
    printf("Total Current: %.4f A (%.2f mA)\n", total_current, total_current * 1000);
    printf("Total Power: %.4f W\n", total_power);
    printf("========================================\n\n");
    
    /* Additional details for series/parallel */
    if (circuit_type == 1) {
        /* Series: Same current, different voltages */
        printf("Individual Voltage Drops:\n");
        for (i = 0; i < num_resistors; i++) {
            double voltage_drop = total_current * resistors[i];
            printf("  V%d = %.4f V (across R%d)\n", i + 1, voltage_drop, i + 1);
        }
    } else {
        /* Parallel: Same voltage, different currents */
        printf("Individual Currents:\n");
        for (i = 0; i < num_resistors; i++) {
            double current = voltage / resistors[i];
            printf("  I%d = %.4f A (%.2f mA) through R%d\n", 
                   i + 1, current, current * 1000, i + 1);
        }
    }
    printf("========================================\n");
}
