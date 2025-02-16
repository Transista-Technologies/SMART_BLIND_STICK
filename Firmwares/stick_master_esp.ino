#include <WiFi.h>
#include <esp_now.h>

// Left & Right Slave MAC Addresses
uint8_t leftSlave[] =  {0x98, 0x3D,0xAE,0xED,0xB4,0xB0}; //L-F -- {0xD8, 0x3B, 0xDA, 0xA3, 0xE5, 0x70} L - {0x98, 0x3D, 0xAE, 0xED, 0xB4, 0xB0}
uint8_t rightSlave[] = {0x98,0x3D,0xAE,0xED,0xCB,0xC0}; //R-F -- {0x98, 0x3D, 0xAE, 0xEE, 0x98, 0x50} R - {0x98, 0x3D, 0xAE, 0xED, 0xCB, 0xC0}


/*// GPIO pins for esp32s3
#define LEFT_TRIGGER 3  // GPIO 6 for Left for pico
#define RIGHT_TRIGGER 2 // GPIO 7 for Right for pico
*/
//for esp32c3
#define LEFT_TRIGGER 2  // GPIO 6 for Left for pico
#define RIGHT_TRIGGER 3 // GPIO 7 for Right for pico

void setup() {
    Serial.begin(115200);
    
    // Set GPIO as Input
    pinMode(LEFT_TRIGGER, INPUT);
    pinMode(RIGHT_TRIGGER, INPUT);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    // Register Left Slave
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, leftSlave, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    // Register Right Slave
    memcpy(peerInfo.peer_addr, rightSlave, 6);
    esp_now_add_peer(&peerInfo);
}

void loop() {
    const char *message = "Vibrate!";

    if (digitalRead(LEFT_TRIGGER) == HIGH) {
        esp_err_t result = esp_now_send(leftSlave, (uint8_t *)message, strlen(message));
        Serial.println(result == ESP_OK ? "Sent to Left" : "Failed to send Left");
        delay(200); // Avoid multiple triggers
    }

    if (digitalRead(RIGHT_TRIGGER) == HIGH) {
        esp_err_t result = esp_now_send(rightSlave, (uint8_t *)message, strlen(message));
        Serial.println(result == ESP_OK ? "Sent to Right" : "Failed to send Right");
        delay(200); // Avoid multiple triggers
    }
}