#define BLYNK_TEMPLATE_ID "TMPL69GmPBaJ-"
#define BLYNK_TEMPLATE_NAME "Plant Watering system"
#define BLYNK_AUTH_TOKEN "hoC9Itz-szcLWcY7aHK14DdR9_xwAztW"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "RSB";           // type your WiFi name
char pass[] = "11223344";      // type your WiFi password

BlynkTimer timer;

#define RELAY_PIN D5                 // Relay connected to D5
#define MOISTURE_THRESHOLD +'"'      // Threshold for soil moisture (adjust as needed)

// Initialize LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

void sendSensor()
{
    int value = analogRead(A0);
    value = map(value, 400, 1023, 100, 0);  // Convert reading to percentage

    // Relay control based on moisture level
    if (value < MOISTURE_THRESHOLD) 
    {
        digitalWrite(RELAY_PIN, LOW);  // Turn ON relay
    } 
    else 
    {
        digitalWrite(RELAY_PIN, HIGH); // Turn OFF relay
    }

    // Send data to Blynk
    Blynk.virtualWrite(V0, value);

    // Print to Serial Monitor
    Serial.print("Soil Moisture: ");
    Serial.print(value);
    Serial.print("% ");
    Serial.println(digitalRead(RELAY_PIN) == LOW ? "Pump ON" : "Pump OFF");

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mois: "); 
    lcd.print(value); 
    lcd.print("%");

    lcd.setCursor(9, 0);
    lcd.print(digitalRead(RELAY_PIN) == LOW ? "Pump ON " : "Pump OF");
}

void setup()
{   
    Serial.begin(115200);
    
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Default OFF state for relay

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Please Wait for");
    lcd.setCursor(0, 1);
    lcd.print("the Soil Data...");
    
    Blynk.begin(auth, ssid, pass);
    
    timer.setInterval(1000L, sendSensor);  // Send sensor data every second
}

void loop()
{
    Blynk.run();
    timer.run();
}
