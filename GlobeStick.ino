#include <BleKeyboard.h>    // BLE Keyboard library
#include <Arduino.h>

// Define pins for rotary encoder
#define ENCODER_PIN_A 19    // Rotary Encoder Pin A (CLK)
#define ENCODER_PIN_B 21    // Rotary Encoder Pin B (DT)

// Define pins for joystick
#define VRX_PIN  34
#define VRY_PIN  35
#define BUTTON_PIN 32       // Joystick button pin

// Joystick threshold values
#define LEFT_THRESHOLD   1000
#define RIGHT_THRESHOLD  4000
#define UP_THRESHOLD     4000
#define DOWN_THRESHOLD   900

// Command definitions for joystick movement
#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

// Variables for rotary encoder
int lastState = HIGH;
int currentState;

// Variables for joystick
int valueX = 0;
int valueY = 0;
int command = COMMAND_NO;
bool buttonPressed = false;

// Initialize BLE Keyboard object (used for both keyboard and mouse-like functionalities)
BleKeyboard bleKeyboard("GlobeStick");

void setup() {
  Serial.begin(115200);

  // Initialize rotary encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // Initialize joystick pins
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Joystick button pin

  // Start BLE Keyboard
  bleKeyboard.begin();
}

void loop() {
  // Only proceed if BLE is connected
  if (bleKeyboard.isConnected()) {

    // --- Rotary Encoder for Zoom In/Out ---
    currentState = digitalRead(ENCODER_PIN_A);
    if (currentState != lastState) {
      // Clockwise rotation (zoom in)
      if (digitalRead(ENCODER_PIN_B) != currentState) {
        Serial.println("Zoom in");
        // bleKeyboard.press(KEY_LEFT_SHIFT);  // Press Shift
        bleKeyboard.press('=');             // Simulate the '=' key to zoom in
        delay(500);                         // Small delay to avoid excessive zoom
        bleKeyboard.releaseAll();           // Release all keys
      }
      // Counter-clockwise rotation (zoom out)
      else {
        Serial.println("Zoom out");
        bleKeyboard.press('-');             // Simulate the '-' key to zoom out
        delay(500);                         // Small delay to avoid excessive zoom
        bleKeyboard.releaseAll();           // Release all keys
      }
      lastState = currentState;  // Update state
    }

    // --- Joystick for Navigation ---
    valueX = analogRead(VRX_PIN);
    valueY = analogRead(VRY_PIN);
    command = COMMAND_NO;

    // Check for left and right movement
    if (valueX < LEFT_THRESHOLD)
      command |= COMMAND_LEFT;
    else if (valueX > RIGHT_THRESHOLD)
      command |= COMMAND_RIGHT;

    // Check for up and down movement
    if (valueY > UP_THRESHOLD)
      command |= COMMAND_DOWN;
    else if (valueY < DOWN_THRESHOLD)
      command |= COMMAND_UP;

    // Handle BLE keyboard arrow key commands
    if (command & COMMAND_LEFT) {
      Serial.println("COMMAND LEFT");
      bleKeyboard.press(KEY_LEFT_ARROW);  // Simulate pressing LEFT arrow key
      delay(500);                         // Short delay to allow the key press
      bleKeyboard.release(KEY_LEFT_ARROW); // Release the key
    }
    if (command & COMMAND_RIGHT) {
      Serial.println("COMMAND RIGHT");
      bleKeyboard.press(KEY_RIGHT_ARROW);  // Simulate pressing RIGHT arrow key
      delay(500);                          // Short delay
      bleKeyboard.release(KEY_RIGHT_ARROW); // Release the key
    }
    if (command & COMMAND_UP) {
      Serial.println("COMMAND UP");
      bleKeyboard.press(KEY_UP_ARROW);  // Simulate pressing UP arrow key
      delay(500);                       // Short delay
      bleKeyboard.release(KEY_UP_ARROW); // Release the key
    }
    if (command & COMMAND_DOWN) {
      Serial.println("COMMAND DOWN");
      bleKeyboard.press(KEY_DOWN_ARROW);  // Simulate pressing DOWN arrow key
      delay(500);                         // Short delay
      bleKeyboard.release(KEY_DOWN_ARROW); // Release the key
    }

    // --- Joystick Button Press to Reset ('R') ---
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      delay(50);
      Serial.println("Joystick button pressed, resetting with 'R'");
      bleKeyboard.press('r');  // Simulate pressing the 'R' key
      delay(100);              // Short delay to ensure key press
      bleKeyboard.release('r'); // Release the key
      buttonPressed = true;     // Mark button as pressed
    }

    // Reset the button state once released
    if (digitalRead(BUTTON_PIN) == HIGH && buttonPressed) {
      buttonPressed = false;  // Reset button press status
    }

    delay(100);  // Adjust delay as needed for responsiveness
  } else {
    Serial.println("Waiting for BLE connection...");
    delay(1000);  // Wait for BLE to connect
  }
}
