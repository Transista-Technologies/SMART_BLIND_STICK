#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Arduino.h>

// Define Pins
#define I2C_0 Wire
#define I2C_1 Wire1
#define buzzer 3
#define vib_mot 19
#define MODE 13  // Tactile switch pin
#define SLEEP 9  // GPIO pin for wake-up button
#define SOS 22 // GPIO pin for SOS button
#define TX_PIN 0       // UART TX for GSM module
#define RX_PIN 1       // UART RX for GSM module
#define BAUD_RATE 115200
#define SerialAT Serial1
#define LED 2
#define LEFT 6
#define RIGHT 7   
bool left_obstacle = false;
bool right_obstacle = false;
volatile long time_count;

#define PHONE_NUMBER "+919384628608"  // Replace with actual SOS recipient
#define SOS_MESSAGE "EMERGENCY! Need Help ASAP!"  // SOS message

#define GSM_UART uart0  // Using UART0 for GSM communication

// Sensor Objects
SparkFun_VL53L5CX myImager;
SparkFun_VL53L5CX myImager2;
VL53L5CX_ResultsData measurementData;
VL53L5CX_ResultsData measurementData2;

// Mode Variable
int mode = 0;  // 0 = Normal, 1 = Sensitive
int mode_val = 0;

void vibrateMotor();
void beepBuzzer();
void handleTOFSensor();
void send_at_command(const char *command, int delay_ms);
void send_sos_message();
bool detectLongPress(int pin, int duration_ms);

void setup() {
    Serial.begin(115200);
    //delay(2000);
    Serial.println("Initializing VL53L5CX & GSM...");
    

    pinMode(buzzer, OUTPUT);
    pinMode(vib_mot, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(MODE, INPUT);
    pinMode(SOS, INPUT);
    pinMode(SLEEP, INPUT_PULLUP);
    pinMode(LEFT, OUTPUT);
    pinMode(RIGHT, OUTPUT);

    digitalWrite(buzzer, HIGH);
    digitalWrite(vib_mot, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);
    digitalWrite(vib_mot, LOW);

    // Configure I2C buses
    I2C_0.setSDA(4);
    I2C_0.setSCL(5);
    I2C_0.begin();
    I2C_0.setClock(400000);

    I2C_1.setSDA(14);
    I2C_1.setSCL(15);
    I2C_1.begin();
    I2C_0.setClock(400000);

    // Initialize TOF Sensors
    while (!myImager.begin(VL53L5CX_DEFAULT_I2C_ADDRESS >> 1, I2C_0)) {
        Serial.println(F("Sensor 1 not found - check wiring."));
        delay(1000);
    }
    myImager.setResolution(4 * 4);
    myImager.setRangingFrequency(60);
    myImager.startRanging();

    while (!myImager2.begin(VL53L5CX_DEFAULT_I2C_ADDRESS >> 1, I2C_1)) {
        Serial.println(F("Sensor 2 not found - check wiring."));
        delay(1000);
    }
    myImager2.setResolution(4 * 4);
    myImager2.setRangingFrequency(60);
    myImager2.startRanging();

    // Initialize GSM UART
    uart_init(GSM_UART, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    digitalWrite(buzzer, HIGH);
    digitalWrite(vib_mot, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);
    digitalWrite(vib_mot, LOW);
    delay(150);
    digitalWrite(buzzer, HIGH);
    digitalWrite(vib_mot, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);
    digitalWrite(vib_mot, LOW);
    delay(150);
    digitalWrite(buzzer, HIGH);
    digitalWrite(vib_mot, HIGH);
    delay(150);
    digitalWrite(buzzer, LOW);
    digitalWrite(vib_mot, LOW);

}

void loop() {
    // Check if MODE button is pressed
    //if (detectLongPress(MODE, 3000)) {
      if (digitalRead(MODE) == HIGH) {
        if(mode_val == 0){
          digitalWrite(buzzer, HIGH);
          delay(500);
          digitalWrite(buzzer, LOW);
        }
        mode_val = 1;
        mode = 1;  // Enter Sensitive Mode (600mm)
        
      } else {
          digitalWrite(buzzer, LOW);
          mode_val = 0;
          mode = 0;  // Return to Normal Mode (300mm)
      }
    
    //Serial.print("Switched to Mode: ");
    //Serial.println(mode ? "Sensitive (600mm)" : "Normal (300mm)");

    if (detectLongPress(SOS, 3000)) {
      //  Serial.println("SOS Triggered!");
        vibrateMotor();  // Indicate activation
        send_sos_message();
        beepBuzzer();
    }
    
    handleTOFSensor();
}

void handleTOFSensor() {
    int threshold = (mode == 1) ? 600 : 850;
    

  if (myImager.isDataReady() && myImager.getRangingData(&measurementData)) {
     Serial.print("Distance 1 (mm): ");
     Serial.println(measurementData.distance_mm[0]);
      left_obstacle = measurementData.distance_mm[0] <= threshold;
     //digitalWrite(buzzer, left_obstacle ? HIGH : LOW);
     digitalWrite(LEFT,  left_obstacle ? HIGH : LOW);
}

  if (myImager2.isDataReady() && myImager2.getRangingData(&measurementData2)) {
     Serial.print("Distance 2 (mm): ");
     Serial.println(measurementData2.distance_mm[0]);
     right_obstacle = measurementData2.distance_mm[0] <= threshold;
     //digitalWrite(buzzer, right_obstacle ? HIGH : LOW);
     digitalWrite(RIGHT, right_obstacle ? HIGH : LOW);
  }

    // Check if both sides are below the threshold
  if (left_obstacle && right_obstacle) {
    digitalWrite(vib_mot, HIGH); 
    delay(500);
    digitalWrite(vib_mot, LOW); 
    time_count++;
  } else {
    time_count = 0;
    digitalWrite(vib_mot, LOW);
  }
  if(time_count == 20){
    digitalWrite(buzzer,HIGH);
    delay(3000);
    digitalWrite(buzzer,LOW);
    while(true){
      if(digitalRead(SLEEP) == HIGH){
        time_count = 0;
        digitalWrite(buzzer, HIGH);
        digitalWrite(vib_mot, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);
        digitalWrite(vib_mot, LOW);
        delay(150);
        digitalWrite(buzzer, HIGH);
        digitalWrite(vib_mot, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);
        digitalWrite(vib_mot, LOW);
        delay(150);
        digitalWrite(buzzer, HIGH);
        digitalWrite(vib_mot, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);
        digitalWrite(vib_mot, LOW);
        break;
      }
      else {
        digitalWrite(LEFT,LOW);
        digitalWrite(RIGHT,LOW);
        continue;
      }
    }
  }
/*if (left_obstacle || right_obstacle) {
    digitalWrite(buzzer, HIGH); // Turn buzzer ON if any obstacle is detected
    delay(100);
    digitalWrite(buzzer, LOW);
}*/
}

bool detectLongPress(int pin, int duration_ms) {
    if (digitalRead(pin) == HIGH) {
        delay(duration_ms);
        return (digitalRead(pin) == HIGH);
    }
    return false;
}



void vibrateMotor() {
    digitalWrite(vib_mot, HIGH);
    delay(100);
    digitalWrite(vib_mot, LOW);
}

void beepBuzzer() {
    digitalWrite(buzzer, HIGH);
    delay(10);
    digitalWrite(buzzer, LOW);
}

void send_at_command(const char *command, int delay_ms) {
    //Serial.print("Sending: "); 
    //Serial.println(command);
    uart_puts(GSM_UART, command);
    uart_puts(GSM_UART, "\r\n");
    delay(delay_ms);

    while (uart_is_readable(GSM_UART)) {
        char c = uart_getc(GSM_UART);
        Serial.write(c);
    }
   // Serial.println();
}

void send_sos_message() {
    //Serial.println("Checking if GSM module is responsive...");
    send_at_command("AT", 1000);
    send_at_command("AT+CMGF=1", 1000);
    
    String cmgsCommand = "AT+CMGS=\"" + String(PHONE_NUMBER) + "\"";
    send_at_command(cmgsCommand.c_str(), 2000);
    
    //Serial.println("Sending SOS message...");
    uart_puts(GSM_UART, SOS_MESSAGE);
    uart_putc(GSM_UART, 0x1A);
    delay(5000);
    
    while (uart_is_readable(GSM_UART)) {
        char c = uart_getc(GSM_UART);
        Serial.write(c);
    }
    
    send_at_command("AT+RESET", 1000);
    delay(500);
}