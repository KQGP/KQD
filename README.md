# KQD — Tank Level Monitor

An **Arduino/ESP8266 tank level monitor**: measures water level with an HC-SR04 ultrasonic sensor, shows it on a 16x2 I2C LCD, supports potentiometer-based calibration and a limit switch. Includes a Proteus simulation project.

## ✨ Features

- 📏 Ultrasonic distance measurement (5-sample median filter)
- 🖥️ 16x2 LCD (I2C, address 0x27)
- 🎚️ Potentiometer sets tank height / calibration mode
- 🔘 Limit switch for calibration trigger
- 📊 Displays distance, level (cm) and percentage full with LOW/FULL alerts
- 🧪 Proteus simulation (`KQD.pdsprj`)

## 🧱 Hardware

- ESP8266 (NodeMCU) — uses `D5`, `D6`, `A0`, `D3`, `D2`/`D1`
- HC-SR04 ultrasonic sensor
- 16x2 I2C LCD
- Potentiometer, limit switch

## 📁 Project Structure

```
KQD.ino          # Firmware sketch
KQD.pdsprj       # Proteus simulation project
```

## 🚀 Getting Started

### Firmware
1. Open `KQD.ino` in Arduino IDE with ESP8266 board support installed.
2. Upload to the board.
3. Wire per the sketch pin definitions.

### Simulation
Open `KQD.pdsprj` in Proteus to run the virtual circuit.
