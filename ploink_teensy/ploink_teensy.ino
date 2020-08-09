// ploink
// v0.0.1
// august 2020

// this is a project started by aaron montoya-moraga
// research assistant at mit media lab
// opera of the future + future sketches

// this version runs on teensy 4.0
// with the teensy audio shield

// current functionalities include:
// 0. read knob position
// 1. move micro servo according to knob for manipulating string tension
// 2. receive audio mic input
// 3. estimate fundamental frequency of audio and its probability

//import libraries for teensy audio shield
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//#include <SerialFlash.h>

// teensy audio library variables
AudioInputI2S audioInput;
AudioAnalyzeNoteFrequency audioAnalyzer;
AudioConnection          patchCord1(audioInput, 0, audioAnalyzer, 0);
AudioControlSGTL5000     sgtl5000_1;

//import servo library
#include <Servo.h>

// create servo instance of Servo
Servo servo;

// variable to store current servo position
int servoPosCurrent = 0;

// variable to store target servo position
int servoPosTarget = 90;
int servoPosDelta = 1;
int servoPin = 9;
// measured in milliseconds
int servoDelay = 25;

int knobValueRaw = 0;
int knobValueMapped = 0;
int knobPin = 0;

boolean knobCallibrate = false;

// setup() happens once, at the beginning
void setup() {


  // audio shield setup

  AudioMemory(30);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);

  // initialize serial communcation at 9600 bits per second
  Serial.begin(9600);

  // connect servo to pin
  servo.attach(servoPin);

  // initialize audio frequency analyzer
  audioAnalyzer.begin(.15);

}

void loop() {

  knobValueRaw = analogRead(knobPin);
  knobValueMapped = map(knobValueRaw, 0, 1023, 0, 180);
  servoPosTarget = knobValueMapped;

  servo.write(servoPosTarget);
  
  Serial.print("servoPosTarget: ");
  Serial.print(servoPosTarget);

  //  if (servoPosCurrent - servoPosTarget < servoPosDelta) {
  //    servo.write(servoPosCurrent + servoPosDelta);
  //  } else if (servoPosCurrent - servoPosTarget > servoPosDelta) {
  //    servo.write(servoPosCurrent - servoPosDelta);
  //  }
  delay(servoDelay);

  if (audioAnalyzer.available()) {
    float freq = audioAnalyzer.read();
    float prob = audioAnalyzer.probability();
    Serial.printf(", mic freq: %3.2f , mic prob: %.2f", freq, prob);
  }

  Serial.println("");

}
