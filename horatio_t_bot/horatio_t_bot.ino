#include <NewPing.h>
#include "Servo.h"

// Ultrasonic detection stuff
const int TRIGGER_PIN = 3;
const int ECHO_PIN = 4;
const int MAX_DISTANCE = 150;

// Instance of ultrasonic detector
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long firstSeen = 0;
const unsigned long DWELL_TIME = 3000;
boolean isRunning = false;

// Waving
const int NUMBER_OF_WAVES = 3;
// Microseconds of deflection from WAVE_HOME
// for left & right side of wave
int waveDeflect = 150;
// Time to allow for each wave (ms)
int waveTime = 850;
// home point for wave servo (us)
const int WAVE_HOME = 1500;
// centre point for wave action (us)
const int WAVE_CENTRE = 1500;

const int NUMBER_OF_SHAKES = 3;
// microseconds of deflection from SHAKE_HOME
// for up & down portion of shake
int shakeDeflect = 225;
// Time to allow for each wave (ms)
int shakeTime = 700;
// home point for wave servo (us)
const int SHAKE_HOME = 650;
// centre point for wave action (us)
const int SHAKE_CENTRE = 925;
const int SHAKE_CENTRE_FOR_WAVE = 1600;
// home point for wrist servo (us)
const int WRIST_HOME = 1465;   //600=palm down 1000=thumb right, 1465=thumb and little finger level, 1500=thumb left 2000=palm up  2200=palm right 2600=thumb down
// centre point for wave action (us)
const int WRIST_CENTRE = 1600;
const int WRIST_CENTRE_FOR_WAVE = 1420;
const int WRIST_CENTRE_FOR_SHAKE = 1600;
//servo definitions
const int wristServoPin = 1;
const int shakeServoPin = 2;
const int waveServoPin = 5;




// Servos
Servo waveServo;
Servo shakeServo;
Servo wristServo;

void setup() {
  //wait on power up, so that we can unplug the programming cable and plug in the power supply. we don't want to try to run 3 servos with a 1A stall each from a usb socket do we...
    delay(3000);
    waveServo.attach(waveServoPin);
    shakeServo.attach(shakeServoPin);
    wristServo.attach(wristServoPin);
    goHome();
}

void loop() {
    delay(50);
    int distance = getDistance();
    bool doThings = shouldDoThings(distance);
    // If we have something in range,
    // and we're not already doing so, wave;
    if(doThings){
        int waveOrShake = random(100);
        if (waveOrShake > 50) {
            shake(distance);
        } else {
            wave(distance);
        }
        delay(1000);
        goHome();
    } else {
        goHome();
    }
}

bool shouldDoThings(int distance){
    unsigned long now = millis();

    if(distance > 0){
        // If this is the first time we're seeing a ping
        // Record the time and continue
        if (firstSeen == 0){
            firstSeen = now;
            return false;
        }
        // If more than DWELL_TIME has elapsed with continous
        // pings, then do things

        if ((now - firstSeen) > DWELL_TIME){
            firstSeen = 0;
            return true;
        }

        // Keep on looping
        return false;
    } else {
        // No ping detected. reset firstSeen and return False
        firstSeen = 0;
        return false;
    }
}

int getDistance(){
    int uS = sonar.ping();
    int distance = uS / US_ROUNDTRIP_CM;
    return distance;
}

void goHome(){
  //home is lying flat on desk, palm down
    waveServo.writeMicroseconds(WAVE_HOME);
    shakeServo.writeMicroseconds(SHAKE_HOME);
    wristServo.writeMicroseconds(WRIST_HOME);
    delay(1000);
}

void wave(int uS){

    isRunning = true;
    //raise the hand for the start of the wave
    shakeServo.writeMicroseconds(SHAKE_CENTRE_FOR_WAVE);
    //rotate the hand for the start of the wave
    wristServo.writeMicroseconds(WRIST_CENTRE_FOR_WAVE);
    
    delay(2000);
    for (int n=0; n < NUMBER_OF_WAVES; n++){
        waveServo.writeMicroseconds(WAVE_CENTRE + waveDeflect);
        delay(waveTime);
        waveServo.writeMicroseconds(WAVE_CENTRE - waveDeflect);
        delay(waveTime);
    }
    waveServo.writeMicroseconds(WAVE_CENTRE);
    delay(waveTime);
    isRunning = false;
}

void shake(int uS){
    isRunning = true;
    //move hand and rotate the wrist ready for the shake
    shakeServo.writeMicroseconds(SHAKE_CENTRE);
    wristServo.writeMicroseconds(WRIST_CENTRE_FOR_SHAKE);
    delay(400);
    
    for (int n=0; n < NUMBER_OF_SHAKES; n++){
        shakeServo.writeMicroseconds(SHAKE_CENTRE - shakeDeflect);
        delay(shakeTime);
        shakeServo.writeMicroseconds(SHAKE_CENTRE + shakeDeflect);
        delay(shakeTime);
    }
    shakeServo.writeMicroseconds(SHAKE_CENTRE);
    delay(shakeTime);
    isRunning = false;
}
