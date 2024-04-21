import json
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Функция для чтения данных из файла
def read_data(filename):
    data = []
    with open(filename, 'r') as file:
        for line in file:
            data.append(json.loads(line))  # Предполагается, что каждая строка содержит JSON
    return data

# Функция для обновления данных графиков
def update_plot(frame):
    data = read_data(filename)
    humidities = [entry['hum'] for entry in data]
    temperatures = [entry['temp'] for entry in data]
    sound_values = [entry['sound'] for entry in data]

    ax1.clear()
    ax1.plot(humidities, marker='o', color='b')
    ax1.set_title('Humidity')
    ax1.set_ylabel('Humidity')

    ax2.clear()
    ax2.plot(temperatures, marker='o', color='g')
    ax2.set_title('Temperature')
    ax2.set_ylabel('Temperature (°C)')

    ax3.clear()
    ax3.plot(sound_values, marker='o', color='r')
    ax3.set_title('Sound')
    ax3.set_ylabel('Sound')

# Создание начальных графиков
filename = '/home/undertaken/C++/project/sensor_data.json'  # Замените на путь к вашему файлу JSON
data = read_data(filename)

fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 6))

humidities = [entry['hum'] for entry in data]
temperatures = [entry['temp'] for entry in data]
sound_values = [entry['sound'] for entry in data]

ax1.plot(humidities, marker='o', color='b')
ax1.set_title('Humidity')
ax1.set_ylabel('Humidity')

ax2.plot(temperatures, marker='o', color='g')
ax2.set_title('Temperature')
ax2.set_ylabel('Temperature (°C)')

ax3.plot(sound_values, marker='o', color='r')
ax3.set_title('Sound')
ax3.set_ylabel('Sound')

# Анимация обновления графиков
ani = FuncAnimation(fig, update_plot, interval=1000)  # Обновление каждую секунду

plt.tight_layout()
plt.show()
