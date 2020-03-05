#include "movement.h"


int move_forward_cm(oi_t *sensor_data, int distance_cm) {

    double travel_dist = 0;

    double distance_mm = distance_cm * 10.0;

    oi_setWheels(100, 100);
    while(travel_dist < distance_mm) {
        oi_update(sensor_data);
        travel_dist += sensor_data -> distance;

        lcd_printf("%f", travel_dist);
    }

    oi_setWheels(0,0);

    return travel_dist / 10;
}

double move_forward_mm(oi_t *sensor_data, double distance_mm) {

    double travel_dist = 0;

    oi_setWheels(100, 100);
    while(travel_dist < distance_mm) {
        oi_update(sensor_data);
        travel_dist += sensor_data -> distance;

        lcd_printf("%f", travel_dist);
    }

    oi_setWheels(0,0);

    return travel_dist;
}

int move_backward_cm(oi_t *sensor_data, int distance_cm) {

    double travel_dist = 0;

    double distance_mm = distance_cm * 10.0;

    oi_setWheels(-100, -100);
    while(travel_dist > distance_mm * -1) {
        oi_update(sensor_data);
        travel_dist += sensor_data -> distance;

        lcd_printf("%f", travel_dist);
    }

    oi_setWheels(0,0);

    return travel_dist / 10;
}

double move_backward_mm(oi_t *sensor_data, double distance_mm) {

    double travel_dist = 0;

    oi_setWheels(-100, -100);
    while(travel_dist > distance_mm * -1) {
        oi_update(sensor_data);
        travel_dist += sensor_data -> distance;

        lcd_printf("%f", travel_dist);
    }

    oi_setWheels(0,0);

    return travel_dist;
}

void turn_left_degrees(oi_t *sensor_data, double degrees){
    double travel_degrees = 0;

    oi_setWheels(100, -100);

    while(travel_degrees < degrees) {
        oi_update(sensor_data);
        travel_degrees += sensor_data -> angle;
    }

    oi_setWheels(0,0);
}
void turn_right_degrees(oi_t *sensor_data, double degrees){
    double travel_degrees = degrees;

    oi_setWheels(-100, 100);

    while(travel_degrees > 0) {
        oi_update(sensor_data);
        travel_degrees += sensor_data -> angle;
    }

    oi_setWheels(0,0);
}

int check_bump(oi_t *sensor_data) {
    if (sensor_data -> bumpLeft) {
        if(sensor_data -> bumpRight) {
            return 3;
        }
        return 1;
    }
    else if(sensor_data -> bumpRight) {
        return 2;
    }
    return 0;
}

void move_two_meters(oi_t *sensor_data) {
    double travel_dist = 0;

    while(travel_dist < 2000) {

        oi_update(sensor_data);
        travel_dist += sensor_data -> distance;
        lcd_printf("%f", travel_dist);
        int bmp = check_bump(sensor_data);

        if(bmp == 0) {
            move_forward_cm(sensor_data, 5);
        }
        else {

            if(bmp == 1) {
                move_backward_cm(sensor_data, 15);
                travel_dist -= 15;
                turn_right_degrees(sensor_data, 90);
                move_forward_cm(sensor_data, 25);
                turn_left_degrees(sensor_data, 90);
            }
            else if(bmp == 2 || bmp == 3) {
                move_backward_cm(sensor_data, 15);
                travel_dist -= 15;
                turn_left_degrees(sensor_data, 90);
                move_forward_cm(sensor_data, 25);
                turn_right_degrees(sensor_data, 90);
            }
        }
    }
}
