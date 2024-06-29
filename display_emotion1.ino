// Define pins for LEDs, vibration motor, and seven-segment display
const int redLedPin = 9;
const int blueLedPin = 11;
const int greenLedPin = 10;
const int vibrationMotorPin = 12;
// Define pins for seven-segment display segments
const int segmentPins[] = {4, 5, 8, 7, 6, 3, 2};
void setup() {
  // Initialize pins
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(vibrationMotorPin, OUTPUT);
  // Initialize pins for seven-segment display
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], HIGH); // Turn off all segments
  }
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Enter temperature (°C), skin conductance (µS), and heart rate (BPM):");
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read the input values from the serial monitor
    float temperature = Serial.parseFloat();
    float skinConductance = Serial.parseFloat();
    float heartRate = Serial.parseFloat();

    // Print the received values for debugging
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Skin Conductance: ");
    Serial.println(skinConductance);
    Serial.print("Heart Rate: ");
    Serial.println(heartRate);

    // Determine emotion based on input values
    String detectedEmotion = "";
    String emotionLevel = "";
    determineEmotion(temperature, skinConductance, heartRate, detectedEmotion, emotionLevel);

    // Print detected emotion for debugging
    Serial.print("Detected Emotion: ");
    Serial.println(detectedEmotion);
    Serial.print("Emotion Level: ");
    Serial.println(emotionLevel);

    // Set LED colors and brightness based on detected emotion and level
    setLEDs(detectedEmotion, emotionLevel);

    // Check for extreme level and blink LED if necessary
    if (emotionLevel == "Extreme") {
      blinkLed(redLedPin); // Blink the red LED
    }

    // Trigger vibration if emotion level is not normal
    if (emotionLevel != "Normal") {
      activateVibration();
    }

    // Start countdown on seven-segment display
    startCountdown();
  }
}

void determineEmotion(float temp, float conductance, float hr, String &emotion, String &level) {
  // Determine the emotion and level based on input values
  if (temp >= 27 && temp <= 32 && hr >= 60 && hr <= 80 && conductance >= 10 && conductance <= 20) {
    emotion = "sadness";
    level = "Normal";
  } else if (temp >= 25 && temp <= 30 && hr >= 80 && hr <= 100 && conductance >= 15 && conductance <= 25) {
    emotion = "sadness";
    level = "High";
  } else if (temp >= 20 && temp <= 25 && hr >= 100 && hr <= 140 && conductance >= 20 && conductance <= 30) {
    emotion = "sadness";
    level = "Extreme";
  } else if (temp >= 35 && temp <= 37 && hr >= 80 && hr <= 100 && conductance >= 20 && conductance <= 35) {
    emotion = "anger";
    level = "Normal";
  } else if (temp >= 36 && temp <= 38 && hr >= 100 && hr <= 140 && conductance >= 25 && conductance <= 40) {
    emotion = "anger";
    level = "High";
  } else if (temp >= 37 && temp <= 39 && hr >= 120 && hr <= 160 && conductance >= 30 && conductance <= 40) {
    emotion = "anger";
    level = "Extreme";
  } else if (temp >= 33 && temp <= 35 && hr >= 60 && hr <= 80 && conductance >= 15 && conductance <= 20) {
    emotion = "fear";
    level = "Normal";
  } else if (temp >= 32 && temp <= 34 && hr >= 100 && hr <= 140 && conductance >= 25 && conductance <= 35) {
    emotion = "fear";
    level = "High";
  } else if (temp >= 31 && temp <= 32 && hr >= 120 && hr <= 160 && conductance >= 30 && conductance <= 40) {
    emotion = "fear";
    level = "Extreme";
  } else {
    emotion = "unknown";
    level = "unknown";
  }
}

void setLEDs(String emotion, String level) {
  int redIntensity = 0;
  int blueIntensity = 0;
  int greenIntensity = 0;
  
  if (emotion == "sadness") {
    if (level == "Normal") {
      blueIntensity = 85; // Low brightness
    } else if (level == "High") {
      blueIntensity = 170; // Medium brightness
    } else if (level == "Extreme") {
      blueIntensity = 255; // High brightness
    }
  } else if (emotion == "anger") {
    if (level == "Normal") {
      redIntensity = 85; // Low brightness
    } else if (level == "High") {
      redIntensity = 170; // Medium brightness
    } else if (level == "Extreme") {
      redIntensity = 255; // High brightness
    }
  } else if (emotion == "fear") {
    if (level == "Normal") {
      greenIntensity = 85; // Low brightness
    } else if (level == "High") {
      greenIntensity = 170; // Medium brightness
    } else if (level == "Extreme") {
      greenIntensity = 255; // High brightness
    }
  }

  analogWrite(redLedPin, redIntensity);
  analogWrite(blueLedPin, blueIntensity);
  analogWrite(greenLedPin, greenIntensity);
}

void blinkLed(int pin) {
  // Blink the LED connected to the given pin
  digitalWrite(pin, HIGH);
  delay(2000); // Increased delay to 2 seconds
  digitalWrite(pin, LOW);
  delay(2000); // Increased delay to 2 seconds
}

void activateVibration() {
  // Trigger vibration motor
  digitalWrite(vibrationMotorPin, HIGH);
  delay(2000); // Increased delay to 2 seconds
  digitalWrite(vibrationMotorPin, LOW);
}

void startCountdown() {
  // Countdown from 5 to 1
  for (int count = 5; count >= 1; count--) {
    displayDigit(count);
    delay(1000); // Wait for a second
  }

  // Turn off display after countdown
  for (int i = 0; i < 6; i++) {
    digitalWrite(segmentPins[i], LOW);
  }
  digitalWrite(segmentPins[6], HIGH);
}

void displayDigit(int digit) {
  // Define patterns for digits 0 to 9 (segments A to G)
  const byte digitPatterns[10] = {
    B11000000, // 0
    B11111001, // 1
    B10100100, // 2
    B10110000, // 3
    B10011001, // 4
    B10010010, // 5
    B10000010, // 6
    B11111000, // 7
    B10000000, // 8
    B10010000  // 9
  };

  // Display the specified digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], bitRead(digitPatterns[digit], i));
  }
}