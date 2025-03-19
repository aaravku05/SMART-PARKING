#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

#define TRIG_ENTRY 4
#define ECHO_ENTRY 3
#define TRIG_EXIT A3
#define ECHO_EXIT 5

#define SERVO_PIN 6

#define ADD_BUTTON A1
#define REMOVE_BUTTON A2

#define MAX_SLOTS 3
#define MAX_UIDS 10  // Max number of registered UIDs

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

String registeredUIDs[MAX_UIDS];
int registeredCount = 0;
int availableSlots = MAX_SLOTS;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.init();
    lcd.backlight();
    pinMode(ADD_BUTTON, INPUT_PULLUP);
    pinMode(REMOVE_BUTTON, INPUT_PULLUP);
    pinMode(TRIG_ENTRY, OUTPUT);
    pinMode(ECHO_ENTRY, INPUT);
    pinMode(TRIG_EXIT, OUTPUT);
    pinMode(ECHO_EXIT, INPUT);
    gateServo.attach(SERVO_PIN);
    closeGate();  // Set initial gate position
    lcd.print("SlotSync");
    lcd.setCursor(0, 1);
    lcd.print("Smart-Parking");
    delay(1700);
    lcd.clear();
    lcd.print("Reserve");
    delay(1500);
    lcd.clear();
    lcd.print("Park");
    delay(1500);
    lcd.clear();
    lcd.print("Enjoy");
    delay(1500);
    lcd.clear();
    lcd.print("Made by Aarav");
    lcd.setCursor(0, 1);
    lcd.print("and Jainam");
    delay(2000);
    lcd.clear();
    updateLCD();
}

void loop() {
    if (digitalRead(ADD_BUTTON) == LOW) {
        addCard();
    } else if (digitalRead(REMOVE_BUTTON) == LOW) {
        removeCard();
    } else {
        checkEntry();
        checkExit();
    }
    
    static unsigned long lastSent = 0;
    if (millis() - lastSent >= 1000) {  // Send data every 1 second
        Serial.print("Available Slots: ");
        Serial.println(availableSlots);
        lastSent = millis();
    }
}

void addCard() {
    lcd.clear();
    lcd.print("Scan to Add...");
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial());

    String newUID = getUID();
    if (isRegistered(newUID)) {
        lcd.clear();
        lcd.print("Already Added!");
        delay(2000);
        return;
    }

    if (registeredCount >= MAX_UIDS) {
        lcd.clear();
        lcd.print("Memory Full!");
        delay(2000);
        return;
    }

    registeredUIDs[registeredCount++] = newUID;
    lcd.clear();
    lcd.print("Added UID:");
    lcd.setCursor(0, 1);
    lcd.print(newUID);
    delay(5000);
    updateLCD();
}

void removeCard() {
    lcd.clear();
    lcd.print("Scan to Remove...");
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial());

    String uid = getUID();
    int index = getUIDIndex(uid);
    if (index == -1) {
        lcd.clear();
        lcd.print("Not Found!");
        delay(2000);
        return;
    }

    for (int i = index; i < registeredCount - 1; i++) {
        registeredUIDs[i] = registeredUIDs[i + 1];
    }
    registeredCount--;

    lcd.clear();
    lcd.print("Removed UID!");
    delay(2000);
    updateLCD();
}

void checkEntry() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

    String uid = getUID();
    if (!isRegistered(uid)) {
        lcd.clear();
        lcd.print("Access Denied!");
        delay(2000);
        updateLCD();
        return;
    }

    if (availableSlots <= 0) {
        lcd.clear();
        lcd.print("Parking Full!");
        delay(2000);
        updateLCD();
        return;
    }

    lcd.clear();
    lcd.print("Entry Granted!");
    openGate();
    availableSlots--;
    delay(5000);
    closeGate();
    updateLCD();
}

void checkExit() {
    if (measureDistance(TRIG_EXIT, ECHO_EXIT) < 10) {
        if (availableSlots < MAX_SLOTS) {
            lcd.clear();
            lcd.print("Exit Granted!");
            openGate();
            availableSlots++;
            delay(5000);
            closeGate();
            updateLCD();
        }
    }
}

bool isRegistered(String uid) {
    for (int i = 0; i < registeredCount; i++) {
        if (registeredUIDs[i] == uid) return true;
    }
    return false;
}

int getUIDIndex(String uid) {
    for (int i = 0; i < registeredCount; i++) {
        if (registeredUIDs[i] == uid) return i;
    }
    return -1;
}

String getUID() {
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    mfrc522.PICC_HaltA();
    return uid;
}

void openGate() {
    gateServo.write(90);  // Open at 90°
}

void closeGate() {
    gateServo.write(165); // Close at 165°
}

float measureDistance(int trig, int echo) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH) * 0.034 / 2;
}

void updateLCD() {
    lcd.clear();
    lcd.print("Slots: ");
    lcd.print(availableSlots);
    lcd.print("/3");
    Serial.println(availableSlots);
}
