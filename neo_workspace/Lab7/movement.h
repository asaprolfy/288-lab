/*
*
*   movement.h
*
*   Uses open_interface to move, turn, avoid obstacles
*
*
*
*   @author rolf
*
*/

#ifndef HEADER_FILE
#define HEADER_FILE

#include "open_interface.h"

// move forward in cm
//
int move_forward_cm(oi_t *sensor_data, int distance_cm);

// move forward in mm
//
double move_forward_mm(oi_t *sensor_data, double distance_mm);

// move backward in cm
//
int move_backward_cm(oi_t *sensor_data, int distance_cm);

// move backward in mm
//
double move_backward_mm(oi_t *sensor_data, double distance_mm);

// turn left
//
void turn_left_degrees(oi_t *sensor_data, double degrees);

// turn right
//
void turn_right_degrees(oi_t *sensor_data, double degrees);

// check if there is a bump
//
int check_bump(oi_t *sensor_data);

// needed for checkpoint in lab
//
void move_two_meters(oi_t *sensor_data);

#endif
