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
    strtok(memory_info, "\n");
    return 1;
}

float get_cpu_load() {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp == NULL) {
        perror("error opening CPU load");
        return -1.0;
    }
    float load;
    fscanf(fp, "%f", &load);
    fclose(fp);
    return load;
}  //средняя_загрузка_за_последнюю_минуту

float get_cpu_frequency() {
    FILE *fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (fp == NULL) {
        perror("error opening CPU frequency");
        return -1.0;
    }
    float frequency;
    fscanf(fp, "%f", &frequency);
    fclose(fp);
    return frequency / 1000.0; // Переводим значение в ГГц
}

long get_available_memory() {
    FILE *meminfo_file;
    long available_memory = -1; // Инициализируем значение -1, чтобы обозначить ошибку

    meminfo_file = fopen("/proc/meminfo", "r");
    if (meminfo_file == NULL) {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), meminfo_file)) {
        if (strstr(line, "MemAvailable:") == line) {
            sscanf(line, "MemAvailable: %ld kB", &available_memory);
            break;
        }
    }

    fclose(meminfo_file);
    return available_memory;
}

// Функция для получения информации о состоянии сетевых соединений
int get_network_connections() {
    FILE *netstat_output;
    int connection_count = 0;

    netstat_output = popen("netstat -tuln | wc -l", "r");
    if (netstat_output == NULL) {
        perror("Error running netstat command");
        return -1;
    }

    fscanf(netstat_output, "%d", &connection_count);
    pclose(netstat_output);

    return connection_count;
}

// Функция для получения информации об использовании сетевых ресурсов
float get_power_usage() {
    FILE *vcgencmd_output;
    float power_usage = -1.0;

    vcgencmd_output = popen("vcgencmd measure_volts core", "r");
    if (vcgencmd_output == NULL) {
        perror("Error running vcgencmd command");
        return -1.0;
    }

    char line[256];
    while (fgets(line, sizeof(line), vcgencmd_output)) {
        if (strstr(line, "volt=") != NULL) {
            char *token = strtok(line, "=");
            token = strtok(NULL, "V");
            power_usage = atof(token);
            break;
        }
    }

    pclose(vcgencmd_output);
    return power_usage;
}
// Функция для получения информации об использовании памяти процессами
long get_process_memory_usage() {
    FILE *proc_meminfo_file;
    long process_memory_usage = -1; // Инициализируем значение -1, чтобы обозначить ошибку

    proc_meminfo_file = popen("ps -eo rss | awk '{sum += $1} END {print sum}'", "r");
    if (proc_meminfo_file == NULL) {
        perror("Error running command for process memory usage");
        return -1;
    }

    fscanf(proc_meminfo_file, "%ld", &process_memory_usage);
    pclose(proc_meminfo_file);
    return process_memory_usage;
}