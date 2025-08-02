# Heater-Control-System
This project simulates basic Heater control system using Arduino UNO , DHT22, LED and Buzzer.
-Arduino UNO as a Microcontroller
-DHT22 temperature sensor
-LED to represent the heater
-Buzzer for overheat alerts

#Features
-Temperature based State machine:
  -IDLE
  -HEATING
  -STABILIZING
  -TARGET REACHED 
  -OVERHEAT
-Heater control based on the tempertaure and hysteresis
-Buzzer alert on overheat

#Hardware Components
-Arduino UNO
-DHT22 sensor
-LED x 1
-Buzzer x 1

#Simulation Tool
using Wokwi online simulation

#Pin Configuration
|DHT22   | D2 |
|LED     | D3 |
|Buzzer  | D4 |

#How It Works
-The system monitors the ambient temperature.
-It turns ON the heater when below target.
-Stabilizes as it nears the target.
-If temperature exceeds 40 degree celcius ,the buzzer sounds.

#Setup
Upload the '.ino' file to Arduino UNO or Simulate on Wokwi.

#License
This project is a open source for educational purposes.
