#include "collect_data.h"

float get_cpu_temperature() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp == NULL) {
        perror("error opening temperature");
        return -1.0;
    }
    char temperature_str[10];
    fgets(temperature_str, sizeof(temperature_str), fp);
    fclose(fp);

    float temperature = atof(temperature_str) / 1000.0;
    return temperature;
}

int get_memory_info(char *memory_info, int size) {
    FILE *fp = popen("free -h | grep Mem: | awk '{print $4}'", "r");
    if (fp == NULL) {
        perror("error opening memory info");
        return 0;
    }
    fgets(memory_info, size, fp);
    pclose(fp);
    return 1;
}