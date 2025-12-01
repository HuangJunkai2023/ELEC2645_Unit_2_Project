#ifndef HELPERS_H
#define HELPERS_H

/* Helper function to get color value (0-9) from color name */
int get_color_value(const char* color);

/* Helper function to get tolerance percentage from color name */
double get_tolerance(const char* color);

/* Helper function to format resistance with appropriate unit prefix */
void format_resistance(double resistance, char* output);

#endif
