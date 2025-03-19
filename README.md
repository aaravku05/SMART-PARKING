# Smart Parking System - SlotSync

## Overview
The **SlotSync Smart Parking System** is an Arduino-based project that manages parking slots using RFID authentication, ultrasonic sensors, and a servo-controlled gate. It allows users to add and remove authorized RFID cards, track available slots, and control access automatically.

## Features
- **RFID-based Access Control**: Only registered RFID cards can enter.
- **Ultrasonic Sensor for Exit**: Detects vehicle exit and updates available slots.
- **LCD Display**: Shows available slots and system messages.
- **Servo-controlled Gate**: Automatically opens/closes based on access.
- **Buttons for Adding/Removing Cards**: Allows easy management of authorized users.

## Hardware Components
- **Arduino Board** (Uno/Nano)
- **MFRC522 RFID Module**
- **LiquidCrystal_I2C LCD (16x2, I2C Address: 0x27)**
- **SG90 Servo Motor**
- **Ultrasonic Sensors (HC-SR04)** for entry and exit
- **Push Buttons** for adding/removing cards
- **Jumper Wires**
- **Power Supply (5V)**

## Pin Configuration
| Component       | Arduino Pin |
|----------------|-------------|
| RFID SS        | 10          |
| RFID RST       | 9           |
| LCD SDA        | A4          |
| LCD SCL        | A5          |
| Servo Motor    | 6           |
| Entry Trigger  | 4           |
| Entry Echo     | 3           |
| Exit Trigger   | A3          |
| Exit Echo      | 5           |
| Add Card Btn   | A1   (connect  second pin to gnd)         |
| Remove Card Btn| A2   (connect  second pin to gnd)       |

## Installation & Setup
1. **Install Required Libraries**:
   - Install `MFRC522`, `Wire`, `SPI`, and `LiquidCrystal_I2C` libraries in Arduino IDE.
2. **Wiring**: Connect components as per the pin configuration table.
3. **Upload Code**: Flash the provided Arduino sketch to the board.
4. **Power Up**: Connect a 5V power supply or USB cable.
5. **Test System**: Use RFID cards to check the entry and exit process.

## How It Works
1. **Adding a New Card**:
   - Press the `Add Card` button.
   - Scan the RFID card.
   - If not already registered, it will be stored.
2. **Removing a Card**:
   - Press the `Remove Card` button.
   - Scan the card to remove it from memory.
3. **Entry Process**:
   - Scan an authorized RFID card.
   - If a slot is available, the gate opens, and a slot is occupied.
4. **Exit Process**:
   - The exit sensor detects a car leaving.
   - If an occupied slot is detected, it increases available slots and opens the gate.

## Future Enhancements
- **Cloud-based Monitoring** for real-time slot tracking.
- **Mobile App Integration** to check availability.
- **Persistent Storage** for card data to survive power loss.

## Author
Developed by **Aarav Kumar** as part of a smart parking system project.

## License
This project is open-source. Feel free to modify and improve it!

