#include <stdio.h>
#include <math.h>
#include "funcs.h"

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
