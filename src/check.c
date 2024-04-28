#include "check.h"

void check_cpu_temperature_and_memory_info(float temperature, int success_memory) {
    if (temperature == -1.0 || !success_memory) {
        printf("Failed to get CPU temperature or memory info\n");
        exit(1);
    }
}

void check_available_memory(long available_memory) {
    if (available_memory == -1) {
        printf("Failed to get available memory\n");
        exit(1);
    }
}

void check_network_connections(int connection_count) {
    if (connection_count == -1) {
        printf("Failed to get network connections\n");
        exit(1);
    }
}

void check_wiringPi_setup(int wiringPiSetup_result) {
    if (wiringPiSetup_result == -1) {
        printf("Failed to initialize wiringPi.\n");
        exit(1);
    }
}