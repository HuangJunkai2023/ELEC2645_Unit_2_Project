#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "funcs.h"

/* Helper function to get color value */
static int get_color_value(const char* color) {
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
static double get_tolerance(const char* color) {
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
static void format_resistance(double resistance, char* output) {
    if (resistance >= 1e6) {
        sprintf(output, "%.2f MOhm", resistance / 1e6);
    } else if (resistance >= 1e3) {
        sprintf(output, "%.2f kOhm", resistance / 1e3);
    } else {
        sprintf(output, "%.2f Ohm", resistance);
    }
}

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

void menu_item_4(void) {
    int waveform_type, num_samples, i;
    double frequency, amplitude, phase;
    double sample_rate, period, time;
    const double PI = 3.14159265358979323846;
    FILE *fp;
    
    printf("\n>> Signal Generator\n");
    printf("\nGenerate different waveforms with adjustable parameters.\n\n");
    
    printf("Select waveform type:\n");
    printf("1. Sine wave\n");
    printf("2. Square wave\n");
    printf("3. Triangle wave\n");
    printf("4. Sawtooth wave\n");
    printf("Enter choice: ");
    if (scanf("%d", &waveform_type) != 1 || waveform_type < 1 || waveform_type > 4) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("\nEnter frequency (Hz, e.g., 1.0): ");
    if (scanf("%lf", &frequency) != 1 || frequency <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter amplitude (V, e.g., 5.0): ");
    if (scanf("%lf", &amplitude) != 1 || amplitude <= 0) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter phase shift (degrees, 0-360): ");
    if (scanf("%lf", &phase) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    /* Convert phase to radians */
    phase = phase * PI / 180.0;
    
    /* Calculate parameters */
    period = 1.0 / frequency;
    num_samples = 50; /* samples per period */
    sample_rate = frequency * num_samples;
    
    printf("\n========================================\n");
    printf("Signal Parameters\n");
    printf("========================================\n");
    printf("Waveform: ");
    switch(waveform_type) {
        case 1: printf("Sine\n"); break;
        case 2: printf("Square\n"); break;
        case 3: printf("Triangle\n"); break;
        case 4: printf("Sawtooth\n"); break;
    }
    printf("Frequency: %.2f Hz\n", frequency);
    printf("Amplitude: %.2f V\n", amplitude);
    printf("Phase: %.2f degrees\n", phase * 180.0 / PI);
    printf("Period: %.6f seconds\n", period);
    printf("Sample Rate: %.2f samples/sec\n", sample_rate);
    printf("========================================\n\n");
    
    /* Ask to save */
    printf("Save waveform data to file? (y/n): ");
    char save_choice;
    if (scanf(" %c", &save_choice) != 1) {
        save_choice = 'n';
    }
    while (getchar() != '\n');
    
    fp = NULL;
    if (save_choice == 'y' || save_choice == 'Y') {
        fp = fopen("waveform_data.csv", "w");
        if (fp) {
            fprintf(fp, "Time(s),Amplitude(V)\n");
            printf("Saving data to waveform_data.csv...\n\n");
        }
    }
    
    /* Generate waveform */
    printf("Time(s)\t\tAmplitude(V)\tASCII Plot\n");
    printf("--------------------------------------------------------\n");
    
    for (i = 0; i < num_samples; i++) {
        time = (period / num_samples) * i;
        double value = 0.0;
        double t_norm = 2 * PI * frequency * time + phase;
        
        switch(waveform_type) {
            case 1: /* Sine wave */
                value = amplitude * sin(t_norm);
                break;
                
            case 2: /* Square wave */
                value = (sin(t_norm) >= 0) ? amplitude : -amplitude;
                break;
                
            case 3: /* Triangle wave */
                {
                    double phase_norm = fmod(t_norm, 2 * PI);
                    if (phase_norm < PI) {
                        value = amplitude * (2 * phase_norm / PI - 1);
                    } else {
                        value = amplitude * (3 - 2 * phase_norm / PI);
                    }
                }
                break;
                
            case 4: /* Sawtooth wave */
                {
                    double phase_norm = fmod(t_norm, 2 * PI);
                    value = amplitude * (2 * phase_norm / (2 * PI) - 1);
                }
                break;
        }
        
        /* ASCII plot (simple bar representation) */
        int bar_length = (int)((value / amplitude + 1.0) * 20); /* 0-40 range */
        char plot[50] = {0};
        int j;
        for (j = 0; j < 20; j++) {
            plot[j] = ' ';
        }
        plot[20] = '|';
        for (j = 21; j < 41; j++) {
            plot[j] = ' ';
        }
        if (bar_length >= 0 && bar_length < 41) {
            plot[bar_length] = '*';
        }
        plot[41] = '\0';
        
        printf("%.6f\t%+.4f\t\t%s\n", time, value, plot);
        
        if (fp) {
            fprintf(fp, "%.6f,%.4f\n", time, value);
        }
    }
    
    if (fp) {
        fclose(fp);
        printf("\nData saved successfully!\n");
    }
    
    printf("\n========================================\n");
    printf("Waveform generation complete!\n");
    printf("Note: ASCII plot shows one period\n");
    printf("      '|' = zero, '*' = signal value\n");
    printf("========================================\n");
}

void menu_item_5(void) {
    printf("\n>> Chat with DeepSeek\n");
    printf("\nAI-powered assistant for engineering questions and support.\n");
    /* TODO: Implement DeepSeek chat integration */
}
