#include "DHTesp.h"

// Definisi Pin
const int PIR_PIN = 12;
const int DHT_PIN = 14;
const int LDR_PIN = 34; // Pin analog ESP32
const int LAMP_PIN = 26; // LED Kuning
const int AC_PIN = 27;   // LED Biru

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  
  // Konfigurasi Mode Pin
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(AC_PIN, OUTPUT);
  
  // Inisialisasi Sensor Suhu
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  
  Serial.println("Sistem Otomatisasi Kelas Dimulai...");
}

void loop() {
  // 1. Membaca Data dari Sensor
  bool isMotionDetected = digitalRead(PIR_PIN); // true jika ada gerakan
  int lightLevel = analogRead(LDR_PIN);         // Semakin gelap, nilai semakin tinggi di Wokwi
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temperature = data.temperature;

  // Mencetak log ke Serial Monitor untuk keperluan debugging
  Serial.print("Gerakan: "); Serial.print(isMotionDetected ? "Ada Orang | " : "Kosong | ");
  Serial.print("Cahaya: "); Serial.print(lightLevel);
  Serial.print(" | Suhu: "); Serial.print(temperature); Serial.println(" C");

  // 2. Evaluasi Aturan Logika Lampu
  // Wokwi LDR: nilai tinggi (>2000) berarti gelap
  if (isMotionDetected && lightLevel < 1500) {
    digitalWrite(LAMP_PIN, HIGH); // Nyalakan Lampu
  } else {
    digitalWrite(LAMP_PIN, LOW);  // Matikan Lampu
  }

  // 3. Evaluasi Aturan Logika AC
  // AC menyala jika ada orang dan suhu di atas 24 derajat Celcius
  if (isMotionDetected && temperature > 24.0) {
    digitalWrite(AC_PIN, HIGH);   // Nyalakan AC
  } else {
    digitalWrite(AC_PIN, LOW);    // Matikan AC
  }

  // Jeda 1 detik sebelum membaca sensor lagi
  delay(1000); 
}