#include <cjson/cJSON.h>
#include <stdint.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTIMINGS 85
#define DHTPIN 7
int dht11_dat[5] = {0, 0, 0, 0, 0};

void read_dht11_dat(float *humidity, float *dht11_temperature) {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);
    pinMode(DHTPIN, INPUT);

    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while (digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255)
                break;
        }
        laststate = digitalRead(DHTPIN);

        if (counter == 255)
            break;

        if ((i >= 4) && (i % 2 == 0)) {
            dht11_dat[j / 8] <<= 1;
            if (counter > 16)
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))) {
        *humidity = dht11_dat[0] + dht11_dat[1] / 10.0;
        *dht11_temperature = dht11_dat[2] + dht11_dat[3] / 10.0;
    } else {
        *humidity = -1.0;
        *dht11_temperature = -1.0;
    }
}

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

char *serialize_to_JSON(float temperature, const char *memory_info, float humidity, float dht11_temperature) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature of cpu", temperature);
    cJSON_AddStringToObject(root, "memory_info", memory_info);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "temperature", dht11_temperature);

    char *serialized = cJSON_Print(root);
    cJSON_Delete(root);

    return serialized;
}


int main(int argc, char **argv) {
    char memory_info[128];
    float temperature = get_cpu_temperature();
    float humidity = 0, dht11_temperature = 0;

    int success_memory = get_memory_info(memory_info, sizeof(memory_info));
    if (temperature == -1.0 || !success_memory) {
        printf("Failed to get CPU temperature or kernel version or memory info");
        return 1;
    }

    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi.\n");
        return 1;
    }

    char *json_str;

    while (1) {
        read_dht11_dat(&humidity, &dht11_temperature);
        if (humidity != -1.0 && dht11_temperature != -1.0) {
            json_str = serialize_to_JSON(temperature, memory_info, humidity, dht11_temperature);
            printf("Serialized JSON: %s\n", json_str);
            free(json_str);
        }
        delay(10000);
    }

    return 0;
}
