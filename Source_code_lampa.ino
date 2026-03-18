5.Cod Sursă
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define PIR_PIN 15        // Pin pentru senzor PIR
#define LED_PIN 17        // Pin pentru LED/Bec
#define BUZZER_PIN 13     // Pin pentru Buzzer

#define OLED_MOSI 23      // Pin MOSI
#define OLED_CLK 18       // Pin CLK
#define OLED_DC 17        // Pin DC
#define OLED_CS 16        // Pin CS
#define OLED_RESET 5      // Pin RESET

BH1750 lightMeter;  
Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

float lightLevel = 0.0;  
int pirState = LOW;      

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin();
  lightMeter.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("Eroare la inițializarea OLED!"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Lampa Inteligenta"));
  display.display();
  delay(2000);

  Serial.println("Sistem pornit! Toate componentele sunt inițializate.");
}


void updateDisplay(float lux, int pirState) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Lampa Inteligenta"));
  display.print(F("Lumina: "));
  display.print(lux);
  display.println(F(" lx"));
  display.print(F("PIR: "));
  display.println(pirState == HIGH ? "Mişcare detectată" : "Liniște");
  display.display();
}

void loop() {
  lightLevel = lightMeter.readLightLevel();
  Serial.print("Nivel de lumina: ");
  Serial.print(lightLevel);
  Serial.println(" lx");

  
  pirState = digitalRead(PIR_PIN);
  Serial.print("Stare PIR: ");
  Serial.println(pirState == HIGH ? "Mişcare detectată!" : "Nicio mişcare.");

  if (lightLevel < 50 && pirState == HIGH) {
    digitalWrite(LED_PIN, HIGH);   
    digitalWrite(BUZZER_PIN, HIGH); 
    delay(500);                     
    digitalWrite(BUZZER_PIN, LOW);  
  } else {
    digitalWrite(LED_PIN, LOW);    
    digitalWrite(BUZZER_PIN, LOW); 
  }

 
  updateDisplay(lightLevel, pirState);

  delay(1000);
}


