#include <stdio.h>
#include <math.h>
#include "funcs.h"

void menu_item_2(void) {
    double resistance, capacitance, voltage, time_constant;
    int mode, num_points, i;
    double time, max_time;
    FILE *fp;
    
    printf("\n>> RC Circuit Simulator\n");
    printf("\nSimulate and analyse simple RC (resistor-capacitor) circuits.\n\n");
    
    printf("Enter resistance value (Ohm): ");
    if (scanf("%lf", &resistance) != 1 || resistance <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter capacitance value (F, e.g., 0.001 for 1mF): ");
    if (scanf("%lf", &capacitance) != 1 || capacitance <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter supply voltage (V): ");
    if (scanf("%lf", &voltage) != 1 || voltage <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("\nSelect mode:\n");
    printf("1. Charging (0 to V)\n");
    printf("2. Discharging (V to 0)\n");
    printf("Enter choice: ");
    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    /* Calculate time constant */
    time_constant = resistance * capacitance;
    max_time = 5 * time_constant; /* 5tau for ~99% completion */
    
    printf("\n========================================\n");
    printf("Time Constant (tau): %.6f seconds\n", time_constant);
    printf("Time to 99%% charge: %.6f seconds (5*tau)\n", max_time);
    printf("========================================\n\n");
    
    /* Generate data points */
    num_points = 20;
    printf("Time(s)\t\tVoltage(V)\tCurrent(mA)\n");
    printf("------------------------------------------------\n");
    
    /* Save to file option */
    printf("\nSave data to file? (y/n): ");
    char save_choice;
    if (scanf(" %c", &save_choice) != 1) {
        save_choice = 'n';
    }
    while (getchar() != '\n');
    
    fp = NULL;
    if (save_choice == 'y' || save_choice == 'Y') {
        fp = fopen("rc_simulation.csv", "w");
        if (fp) {
            fprintf(fp, "Time(s),Voltage(V),Current(mA)\n");
            printf("Saving data to rc_simulation.csv...\n\n");
        }
    }
    
    for (i = 0; i <= num_points; i++) {
        time = (max_time / num_points) * i;
        double vc, ic;
        
        if (mode == 1) {
            /* Charging: Vc(t) = V0(1 - e^(-t/tau)) */
            vc = voltage * (1 - exp(-time / time_constant));
            ic = (voltage / resistance) * exp(-time / time_constant) * 1000; /* mA */
        } else {
            /* Discharging: Vc(t) = V0 * e^(-t/tau) */
            vc = voltage * exp(-time / time_constant);
            ic = -(voltage / resistance) * exp(-time / time_constant) * 1000; /* mA */
        }
        
        printf("%.6f\t%.4f\t\t%.4f\n", time, vc, ic);
        
        if (fp) {
            fprintf(fp, "%.6f,%.4f,%.4f\n", time, vc, ic);
        }
    }
    
    if (fp) {
        fclose(fp);
        printf("\nData saved successfully!\n");
    }
    
    printf("\n========================================\n");
    printf("Simulation complete!\n");
    printf("========================================\n");
}
