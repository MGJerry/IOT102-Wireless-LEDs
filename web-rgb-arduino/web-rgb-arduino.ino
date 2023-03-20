/* RGB web server with ESP8266 & Arduino
* analogWrite with values received from web page
* Arduino UNO part
*/

#include <SoftwareSerial.h>

SoftwareSerial ArduinoUNO(10, 11); //Rx, TX

const int redLED = 4;
const int greenLED = 5;
const int blueLED = 6;
char c;
String dataIn;
int8_t indexOfR, indexOfG, indexOfB;
String red, green, blue;
int redVal, greenVal, blueVal;

void setup() {
  // Pin setup
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  analogWrite(redLED, 0);
  analogWrite(greenLED, 0);
  analogWrite(blueLED, 0);

  // Serial setup
  delay(1000);
  Serial.begin(9600);
  Serial.println();

  // Communication to Arduino setup
  ArduinoUNO.begin(4800);

  // Test Color
  //testRGB();
}

void loop() {
  // Read data in (when available)
  while (ArduinoUNO.available() > 0) {
    c = ArduinoUNO.read();

    // If not EndOfLine char, add to dataIn string
    if (c == "\n") {break;}
    else           {dataIn += c;}
  }

  if (dataIn != "" && dataIn != "\n") {
    // Parse data string
    parseDataIn();

    // Set LED values
    analogWrite(redLED, red.toInt());
    analogWrite(greenLED, green.toInt());
    analogWrite(blueLED, blue.toInt());

    // Display data
    Serial.println("Red: ");
    Serial.println(red.toInt());
    Serial.println("Green: ");
    Serial.println(green.toInt());
    Serial.println("Blue: ");
    Serial.println(blue.toInt());

    // Reset for later input
    c = 0;
    dataIn = "";    
  }
  delay(90);
}

void parseDataIn() {
  // Find data value location
  indexOfR = dataIn.indexOf("R");
  indexOfG = dataIn.indexOf("G");
  indexOfB = dataIn.indexOf("B");

  // Get data from location
  red = dataIn.substring(0, indexOfR);
  green = dataIn.substring(indexOfR + 1, indexOfG);
  blue = dataIn.substring(indexOfG + 1, indexOfB);

  // Parse data from string to int
  redVal = limitParse(red.toInt());
  greenVal = limitParse(green.toInt());
  blueVal = limitParse(blue.toInt());
}

int limitParse(int i) {
  if (i < 0) {
    i = 0;
  }
  if (i > 255) {
    i = 255;
  }
  return i;
}

void testRGB() { // fade in and out of Red, Green, Blue
  analogWrite(redLED, 255); // R off
  analogWrite(greenLED, 255); // G off
  analogWrite(blueLED, 255); // B off

  fade(redLED); // R
  fade(greenLED); // G
  fade(blueLED); // B
}

void fade(int pin) {
  for (int u = 0; u < 1024; u++) {
    analogWrite(pin, u);
    delay(1);
  }
  for (int u = 0; u < 1024; u++) {
    analogWrite(pin, 1023 - u);
    delay(1);
  }
}