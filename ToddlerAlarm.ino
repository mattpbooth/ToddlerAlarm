//-------------------------------------------------------
// Uses a sonar to detect movement (e.g. if your small human has worked out doors and stealth moving).
// Uses NewPing:
// http://playground.arduino.cc/Code/NewPing
// And a modified version of PlayMelody to play the Peppa Pig theme:
// http://www.arduino.cc/en/Tutorial/PlayMelody
//-------------------------------------------------------
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define LED_PIN 2
#define PIEZO_PIN 9 

byte names[] = {'B', 'G', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};  
int tones[] = {2025, 2551, 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
//byte melody[] = "2d2a1f2c2d2a2d2c2f2d2a2c2d2a1f2c2d2a2a2g2p8p8p8p";
byte melody[] = "4g2e2c4d2G2p2G2B2d2f4e4c4p4p4p";
// count length: 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
//                                10                  20             30
int count = 0;
int count2 = 0;
int count3 = 0;
int MAX_COUNT = 15;
bool shouldSoundTheAlarm = false;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void SoundTheAlarm() {
  analogWrite(PIEZO_PIN, 0);     
  for (count = 0; count < MAX_COUNT; count++) {
    for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++) {
      for (count2=0;count2<10;count2++) {
        if (names[count2] == melody[count*2 + 1]) {       
          analogWrite(PIEZO_PIN,500);
          delayMicroseconds(tones[count2]);
          analogWrite(PIEZO_PIN, 0);
          delayMicroseconds(tones[count2]);
        } 
        if (melody[count*2 + 1] == 'p') {
          // make a pause of a certain size
          analogWrite(PIEZO_PIN, 0);
          delayMicroseconds(500);
        }
      }
    }
  }
  shouldSoundTheAlarm = false;
}

void CheckSonar()
{
  delay(200);                     // Wait 200ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  float distance = uS / US_ROUNDTRIP_CM;
  
  if(distance > 0.0f)
  {
    digitalWrite(LED_PIN, HIGH);
    shouldSoundTheAlarm = true;
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
   
  //Serial.print("Ping: ");
  //Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  //Serial.println("cm");
}

void loop() {
  if (shouldSoundTheAlarm)
  {
    SoundTheAlarm();
  }
  else
  {
    CheckSonar();
  }
}
