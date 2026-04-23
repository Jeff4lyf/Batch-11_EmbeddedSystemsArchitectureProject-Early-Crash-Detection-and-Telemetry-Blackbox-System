# Smart Proximity and Environmental Telemetry System

A real-time environmental monitoring and collision-avoidance system built with Arduino. This project combines ultrasonic distance measuring, multi-directional IR proximity sensors, fire detection, and asynchronous temperature/humidity logging into a unified serial telemetry stream.

## Features

* **Real-time Actuation:** Instantly triggers an LED and Buzzer alarm upon detecting physical obstacles or fire, independent of serial logging delays.
* **Multi-Directional Proximity:** Uses 4 separate IR sensors for perimeter monitoring alongside a forward-facing Ultrasonic sensor.
* **Fire/Flame Detection:** Continuously polls a flame sensor for emergency alerts.
* **Asynchronous Telemetry:** Utilizes non-blocking timers (`millis()`) to log data at a clean 1Hz (1 second) interval without slowing down the core safety loops.
* **Climate Monitoring:** Tracks temperature and humidity using a DHT11 sensor updating every 2.5 seconds.

## Hardware Requirements

* 1x Arduino (Uno, Nano, or Mega)
* 1x HC-SR04 Ultrasonic Sensor
* 4x IR Obstacle Avoidance Modules
* 1x Flame Sensor
* 1x DHT11 Temperature & Humidity Sensor
* 1x Active Buzzer
* 1x LED (with appropriate resistor)
* Jumper wires & Breadboard

## Pin Mapping / Wiring

Connect your components to the Arduino as follows:

| Component / Pin | Arduino Pin | Notes |
| :--- | :--- | :--- |
| **Ultrasonic ECHO** | `D2` | Set as INPUT |
| **Ultrasonic TRIG** | `D3` | Set as OUTPUT |
| **DHT11 Data** | `D8` | *Note: Kept away from D13 to avoid built-in LED timing issues* |
| **Buzzer** | `D11` | Set as OUTPUT |
| **Alarm LED** | `D12` | Set as OUTPUT |
| **Flame Sensor** | `A1` | Digital Read (Active LOW implies fire) |
| **IR Sensor 1 (Front Left)** | `A2` | Digital Read (Active LOW implies obstacle) |
| **IR Sensor 2 (Front Right)**| `A3` | Digital Read (Active LOW implies obstacle) |
| **IR Sensor 3 (Rear Left)** | `A4` | Digital Read (Active LOW implies obstacle) |
| **IR Sensor 4 (Rear Right)**| `A5` | Digital Read (Active LOW implies obstacle) |

> **Warning:** If the IR or Flame sensors are unplugged, the analog pins may "float" and trigger false alarms. Ensure all grounds are connected properly.

## Software Dependencies

You will need to install the following library via the Arduino IDE Library Manager:
* **DHT sensor library** by Adafruit (Required for the DHT11)
* *Note: You may also be prompted to install the Adafruit Unified Sensor library. Accept this prompt.*

## Thresholds & Configuration

You can easily tweak the system parameters by changing these constants at the top of the code:
* `CRASH_DISTANCE_CM = 15` : The distance threshold for the ultrasonic sensor to trigger an alarm.
* `telemetryInterval = 1000` : How often data is printed to the Serial Monitor (in milliseconds).
* `dhtInterval = 2500` : How often the DHT11 sensor is polled (in milliseconds).

## Telemetry Output Format

Open your Serial Monitor and set the baud rate to **115200**. 
The system will output a constant stream of formatted data. 

**Normal State Example:**
```text
[DATA] [ALL CLEAR] Dist: 45cm | IR: 0000 | Temp: 24.5C | Hum: 40.0%
