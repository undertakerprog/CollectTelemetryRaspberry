#include "collect_data.h"


int dht11_dat[5] = {0, 0, 0, 0, 0};


int read_sound_sensor() {
    int sound_value = digitalRead(SOUND_PIN);
    return sound_value;
}

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

int read_gpio_state() {
    FILE *fp;
    char file_path[128];
    char state[2];

    sprintf(file_path, "/sys/class/gpio/gpio%s/value", GPIO_PIN);

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("Error to open file GPIO");
        return -1;
    }

    fgets(state, sizeof(state), fp);

    fclose(fp);

    return atoi(state);
}