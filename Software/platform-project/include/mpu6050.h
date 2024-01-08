#ifndef _MPU6050_H
#define _MPU6050_H

typedef enum {
    POSITION_LEFT_TOP,
    POSITION_TOP,
    POSITION_LEFT_DOWN,
    POSITION_RIGH_TOP,
    POSITION_RIGH_DOWN,
    POSITION_LEFT_UNKNOW
}position_state_t;

void mpu6050_init();
void mpu6050_task();

position_state_t get_position_state();
#endif