#ifndef HEADER_FILE

#define HEADER_FILE

#include "open_interface.h"

int move_forward_cm(oi_t *sensor_data, int distance_cm);
double move_forward_mm(oi_t *sensor_data, double distance_mm);
int move_backward_cm(oi_t *sensor_data, int distance_cm);
double move_backward_mm(oi_t *sensor_data, double distance_mm);

void turn_left_degrees(oi_t *sensor_data, double degrees);
void turn_right_degrees(oi_t *sensor_data, double degrees);

int check_bump(oi_t *sensor_data);

void move_two_meters(oi_t *sensor_data);

#endif
