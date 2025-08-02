#include <ESP32Servo.h>
#include <Keypad.h>

// Pin assignments
const int servoPin = 13;
const int irSensorPin = 14;
const int redLedPin = 4;
const int greenLedPin = 19;
const int buzzerPin = 18;

// Keypad configuration - now all numeric keys will work
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {21, 22, 23, 25};
byte colPins[COLS] = {26, 27, 32, 33};

Servo doorServo;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String password = "1144";  // Correct password
String inputPassword;

void setup() {
  Serial.begin(115200);
  Serial.println("System initialized");
  Serial.println("Correct password is: 1144");

  // Initialize hardware
  doorServo.attach(servoPin);
  doorServo.write(0);

  pinMode(irSensorPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  noTone(buzzerPin);
}

void loop() {
  char key = keypad.getKey();
  
  // Process all numeric keys (0-9)
  if (key && (key >= '0' && key <= '9')) {
    Serial.print(key);
    inputPassword += key;

    if (inputPassword.length() == password.length()) {
      if (inputPassword == password) {
        grantAccess();
      } else {
        denyAccess();
      }
      inputPassword = "";  // Reset for next attempt
    }
  }

  // IR sensor auto-close
  if (digitalRead(irSensorPin) == LOW) {
    if (doorServo.read() != 0) {
      delay(1000);
      doorServo.write(0);
      digitalWrite(greenLedPin, LOW);
    }
  }
}

void grantAccess() {
  Serial.println("\nAccess Granted");
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  noTone(buzzerPin);
  doorServo.write(90);
  delay(2000);
}

void denyAccess() {
  Serial.println("\nAccess Denied");
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
  tone(buzzerPin, 1000);
  delay(2000);
  digitalWrite(redLedPin, LOW);
  noTone(buzzerPin);
}