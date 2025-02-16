# Smart Blind Stick Project

## Overview
This repository contains the firmware, schematics, and 3D design files for the **Smart Blind Stick and Watch Module**. The system provides real-time obstacle detection, haptic feedback, and SOS communication using ESP-NOW and GSM.

## Folder Structure
```
📂 Smart_Blind_Stick_Project
├── 📂 Schematic Diagrams
│   ├── HANDLE_SCHEMATIC_2-1.pdf
│   ├── Schematic_BLIND_STICK_WATCH_2025-02-08-1.pdf
│   ├── handle_schematic_1-1.pdf
│
├── 📂 Firmwares
│   ├── main_stick_module.ino     # Raspberry Pi Pico W firmware
│   ├── stick_master_esp.ino      # ESP32 Master firmware
│   ├── watch_slave_esp.ino       # ESP32 Slave firmware
│
├── 📂 Design Files
│   ├── TSI BS Final Product (STEP).step
│   ├── TSI Wrist Band (STEP).step
│
└── README.md
```

## Hardware Components
- **Raspberry Pi Pico W** (Main Stick Controller)
- **ESP32/ESP32-S3** (Master and Slave Communication)
- **VL53L5CX ToF Sensors** (Obstacle Detection)
- **GSM Module (EC200U)** (SOS Communication)
- **TP4056 Type-C Charging Module**
- **Vibration Motor & Buzzer**

## Required Libraries
Ensure you have the following libraries installed in the **Arduino IDE**:

### For **Main Stick (Pico W)**:
- `Wire.h` (Built-in)
- `Arduino.h` (Built-in)
- `SparkFun_VL53L5CX_Library.h` ([Install](https://github.com/sparkfun/SparkFun_VL53L5CX_Arduino_Library))

### For **ESP Master & Slaves**:
- `WiFi.h` (Built-in for ESP32)
- `esp_now.h` (Built-in for ESP32)

## Installation & Setup
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-repo-link.git
   ```
2. **Upload Firmware**:
   - Use **Arduino IDE** to compile and upload the respective `.ino` files to your devices.
   - Select the correct board:
     - **Raspberry Pi Pico W** for `main_stick_module.ino`
     - **ESP32/ESP32-S3** for `stick_master_esp.ino` and `watch_slave_esp.ino`
3. **Wiring & Assembly**:
   - Refer to the schematics in the `Schematic Diagrams` folder.
   - Connect sensors, buttons, and actuators as per the design.
4. **Testing**:
   - Power on the system and ensure obstacle detection and haptic feedback work correctly.
   - Verify ESP-NOW communication between the master and slaves.
   - Test the SOS feature with a valid phone number.

## Features
✅ **Obstacle Detection** using VL53L5CX ToF sensors  
✅ **ESP-NOW Communication** for wireless trigger signals  
✅ **Haptic Feedback** via vibration motors and buzzers  
✅ **SOS Alert System** with GSM module  
✅ **Rechargeable Battery Support** via TP4056  

---
License

**Software: MIT License** - Open for modification and distribution.

**Hardware: CERN Open Hardware License v2** - Ensures open-source hardware design.

