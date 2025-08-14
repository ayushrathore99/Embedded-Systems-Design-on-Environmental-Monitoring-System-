#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHT_PIN 2
#define MQ2_PIN A0
#define BUTTON_PIN 3
#define DHT_TYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

int displayMode = 0;
const int numModes = 3;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MQ2_PIN, INPUT);
}

void loop() {
  static unsigned long lastButtonPress = 0;
  static unsigned long lastSensorRead = 0;
  
  // Button debounce and mode switching
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (millis() - lastButtonPress > 200) {
      displayMode = (displayMode + 1) % numModes;
      lastButtonPress = millis();
    }
  }
  
  // Sensor reading (every 2 seconds)
  if (millis() - lastSensorRead > 2000) {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int gasValue = analogRead(MQ2_PIN);
    
    // Update LCD based on current mode
    lcd.clear();
    switch(displayMode) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");
        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(humidity);
        lcd.print("%");
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Gas Level: ");
        lcd.setCursor(0, 1);
        lcd.print(gasValue);
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature);
        lcd.print("C H:");
        lcd.print(humidity);
        lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.print("Gas: ");
        lcd.print(gasValue);
        break;
    }
    
    // Send data to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Gas Level: ");
    Serial.println(gasValue);
    
    lastSensorRead = millis();
  }
}


