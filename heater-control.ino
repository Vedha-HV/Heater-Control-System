#include <DHT.h>

// Pin configuration
#define DHTPIN 2         // DHT22 data pin connected to digital pin 2
#define DHTTYPE DHT22    // We're using the DHT22 sensor
#define HEATER_PIN 3     // LED simulating the heater
#define BUZZER_PIN 4     // Buzzer for overheat alert

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Define the different system states
enum State {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

State currentState = IDLE;      // Start in the idle state

// Temperature settings
float targetTemp = 23.0;        // Target room temperature in Celsius
float hysteresis = 0.5;         // Small margin to avoid rapid toggling
float overheatTemp = 40.0;      // Safety cutoff temperature

void setup() {
  Serial.begin(9600);           // Start serial monitor
  dht.begin();                  // Initialize the DHT sensor
  pinMode(HEATER_PIN, OUTPUT);  // Heater (LED) as output
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer as output
}

void loop() {
  float temp = dht.readTemperature();  // Get the current temperature

  // If the reading fails, skip the rest
  if (isnan(temp)) {
    Serial.println("Temperature read failed.");
    return;
  }

  updateState(temp);     // Check which state we should be in
  logStatus(temp);       // Print the current status to serial monitor

  delay(3000);           // Wait 3 seconds before next reading
}

void updateState(float temp) {
  switch (currentState) {
    case IDLE:
      if (temp < targetTemp - hysteresis) {
        currentState = HEATING;
      }
      break;

    case HEATING:
      if (temp >= targetTemp - hysteresis) {
        currentState = STABILIZING;
      }
      break;

    case STABILIZING:
      if (temp >= targetTemp) {
        currentState = TARGET_REACHED;
      }
      break;

    case TARGET_REACHED:
      if (temp > overheatTemp) {
        currentState = OVERHEAT;
      } else if (temp < targetTemp - hysteresis) {
        currentState = HEATING;
      }
      break;

    case OVERHEAT:
      if (temp < targetTemp) {
        currentState = HEATING;
      }
      break;
  }

  // Turn heater and buzzer on/off based on the current state
  if (currentState == HEATING || currentState == STABILIZING) {
    digitalWrite(HEATER_PIN, HIGH);   // Turn on heater (LED)
    digitalWrite(BUZZER_PIN, LOW);    // Buzzer stays off
  } else {
    digitalWrite(HEATER_PIN, LOW);    // Turn off heater

    if (currentState == OVERHEAT) {
      digitalWrite(BUZZER_PIN, HIGH); // Sound buzzer if overheated
    } else {
      digitalWrite(BUZZER_PIN, LOW);  // Otherwise, keep it off
    }
  }
}

void logStatus(float temp) {
  Serial.print("[Temp: ");
  Serial.print(temp);
  Serial.print(" °C]  State: ");

  switch (currentState) {
    case IDLE: Serial.print("IDLE"); break;
    case HEATING: Serial.print("HEATING"); break;
    case STABILIZING: Serial.print("STABILIZING"); break;
    case TARGET_REACHED: Serial.print("TARGET REACHED"); break;
    case OVERHEAT: Serial.print("OVERHEAT"); break;
  }

  Serial.print(" | Heater: ");
  Serial.println(digitalRead(HEATER_PIN) ? "ON" : "OFF");
}
