#include <WiFi.h>
#include <esp_now.h>

#define VIBRATION_PIN 6

void vibrate() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(VIBRATION_PIN, HIGH);
        delay(100);
        digitalWrite(VIBRATION_PIN, LOW);
        delay(100);
    }
}

// Updated callback function with the correct parameters
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    Serial.print("Received: ");
    Serial.println((char *)incomingData);
    vibrate();
}

void setup() {
    Serial.begin(115200);
    pinMode(VIBRATION_PIN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    // Register the corrected callback function
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Everything is handled in OnDataRecv
}