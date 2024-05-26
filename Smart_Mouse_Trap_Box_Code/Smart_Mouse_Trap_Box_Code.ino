/*********
  Designed by: Syed Tehseen ul Hasan Shah
  Date: 26-May-2024
  Arduino IDE version: v2.2.1
  ESP8266 board version in Arduino IDE Board Manager: v3.1.2 

  Project GitHub link for more details, parts list etc: https://github.com/TehseenHasan/Smart_Mouse_Trap_using_Arduino
  Project Working Video: [YouTube]
  
  This Code can be used on Arduino, ESP8266 or ESP8266.
  Just change the pin names according to your board.
  I am using this code on NodeMCU 0.9 (ESP8266).
*********/
// Ultrasonic Sensor (HC-SR04) Pins
const int trigPin = 12;   // D6 pin on NodeMCU
const int echoPin = 14;   // D5 pin on NodeMCU
// Shutter Relay Pin
const int shutterPin = 5;    // D1 pin on NodeMCU
// Buzzer Pin
const int buzzerPin = 4;    // D2 pin on NodeMCU

// Parameters for Ultrasonic Range Detection Sensor
// Define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;

void setup()
{
  Serial.begin(115200);

  // Setting Pins for Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setting Pins for Shutter Relay and Buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin,LOW);  // Turn OFF the buzzer on boot
  pinMode(shutterPin, OUTPUT);
  digitalWrite(shutterPin,HIGH);  // Turn OFF the Shutter (1-Channel Relay Module). Note: The relay module turn ON when gets LOW pulse. 
}

void loop()
{
  readDistance();   // Read the Distance from the Ultrasonic Sensor
  Serial.println(distanceCm);

  // Normally the sensor is giving 10 or 11 cm when the box is empty.
  // When there is a mouse in the box then the distance will be decreased. Then release the shutter to trap the mouse :-D
  if (distanceCm < 9.00)
  {
    digitalWrite(shutterPin,LOW); // Release the Shutter by turning on the relay (give LOW pulse)
    digitalWrite(buzzerPin,HIGH); // Alarm ON
    delay(200);   // Energize the shutter solenoid for very short time to avoid burning the solenoid.
    digitalWrite(shutterPin,HIGH);  // Shutter Off
    delay(5000);  // Keep the alarm ON for 5 seconds.
    digitalWrite(buzzerPin,LOW);  // Alarm OFF
    // After catching the mouse, turn off the system by entering into an infinite loop
    while(1)
    {
      delay (10);
    }
  }
  
  delay(100);
}


void readDistance()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
}