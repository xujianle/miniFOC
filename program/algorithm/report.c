//
// Created by Lao·Zhu on 2021/9/23.
//

#include "main.h"

void report_local_variable(void) {
    unsigned int upload_var[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    upload_var[0] = (machine_angle_offset << 16) | phase_sequence;
    switch (pid_control_mode_flag) {
        default:
        case TORQUE_LOOP_CONTROL:upload_var[1] = float_to_int32(FOC_Struct.user_expect);
            break;
        case SPEED_LOOP_CONTROL:upload_var[1] = float_to_int32(speed_pid_handler.expect);
            break;
        case ANGLE_LOOP_CONTROL:upload_var[1] = float_to_int32(angle_pid_handler.expect);
            break;
    }
    upload_var[2] = float_to_int32(speed_pid_handler.kp);
    upload_var[3] = float_to_int32(speed_pid_handler.ki);
    upload_var[4] = float_to_int32(speed_pid_handler.kd);
    upload_var[5] = float_to_int32(speed_pid_handler.sum_maximum);
    upload_var[6] = float_to_int32(angle_pid_handler.kp);
    upload_var[7] = float_to_int32(angle_pid_handler.ki);
    upload_var[8] = float_to_int32(angle_pid_handler.kd);
    upload_var[9] = float_to_int32(angle_pid_handler.sum_maximum);
    for (unsigned char counter = 0; counter < 5; ++counter) {
        buffer[0] = (unsigned char) ((upload_var[counter * 2] >> 24UL) & 0x000000ffUL);
        buffer[1] = (unsigned char) ((upload_var[counter * 2] >> 16UL) & 0x000000ffUL);
        buffer[2] = (unsigned char) ((upload_var[counter * 2] >> 8UL) & 0x000000ffUL);
        buffer[3] = (unsigned char) (upload_var[counter * 2] & 0x000000ffUL);
        buffer[4] = (unsigned char) ((upload_var[counter * 2 + 1] >> 24UL) & 0x000000ffUL);
        buffer[5] = (unsigned char) ((upload_var[counter * 2 + 1] >> 16UL) & 0x000000ffUL);
        buffer[6] = (unsigned char) ((upload_var[counter * 2 + 1] >> 8UL) & 0x000000ffUL);
        buffer[7] = (unsigned char) (upload_var[counter * 2 + 1] & 0x000000ffUL);
        mdtp_data_transmit(0x01 + counter, buffer);
        led_toggle();
        delayms(20);
        led_toggle();
    }
}

void report_angle_speed(void) {
    unsigned char buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    /* converts velocity floating point data to an integer */
    unsigned int velocity = float_to_int32(FOC_Struct.rotate_speed);
    /* converts angle floating point data to an integer */
    unsigned int angle = float_to_int32(FOC_Struct.mechanical_angle);
    /* separates 32-bit velocity integer into 8-bit integers */
    buffer[0] = (unsigned char) ((velocity >> 24UL) & 0x000000ffUL);
    buffer[1] = (unsigned char) ((velocity >> 16UL) & 0x000000ffUL);
    buffer[2] = (unsigned char) ((velocity >> 8UL) & 0x000000ffUL);
    buffer[3] = (unsigned char) (velocity & 0x000000ffUL);
    /* separates 32-bit angle integer into 8-bit integers */
    buffer[4] = (unsigned char) ((angle >> 24UL) & 0x000000ffUL);
    buffer[5] = (unsigned char) ((angle >> 16UL) & 0x000000ffUL);
    buffer[6] = (unsigned char) ((angle >> 8UL) & 0x000000ffUL);
    buffer[7] = (unsigned char) (angle & 0x000000ffUL);
    /* packet and send through medium capacity data transmission protocol */
    mdtp_data_transmit(0x00, buffer);
}