/*
 * v2 is working for concert
 * 
 * mode 1
 * channel 1 kick
 * channel 2 snare / others
 * channel 3 keyboard 1
 * channel 4 keyboard 2
 * 
 */

#include <DmxSimple.h>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiA);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiB);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, midiC);

// variables used for potentiometer
int potValue1 = 0;
int potValue2 = 0;
int potValue3 = 0;
int potValue4 = 0;
int potValue5 = 0;

float potFloat1 = 0;
float potFloat2 = 0;
float potFloat3 = 0;
float potFloat4 = 0;
float potFloat5 = 0;

int potPin1 = 0;
int potPin2 = 1;
int potPin3 = 2;
int potPin4 = 3;
int potPin5 = 8;

//variables for modular inputs
int modularPin1 = 4;
int modularPin2 = 5;
int modularPin3 = 6;
int modularPin4 = 7;

float modularFloat1 = 0.0;
float modularFloat2 = 0.0;
float modularFloat3 = 0.0;
float modularFloat4 = 0.0;


// variables used for DMX stuff
int firstEg = 0;
int secondEg = 0;
int thirdAEg = 0;
int thirdBEg = 0;
int fourthEg = 0;
int dim1 = 150; // drum eg speed
int dim2 = 0;
int dimKey3 = 50;  // korg mopho
int dimKey4 = 50; // gaia

int dmxVal1 = 0;
int dmxVal2 = 0;
int dmxVal3 = 0;
int dmxVal4 = 0;

int MIDIdmxVal1 = 0;
int MIDIdmxVal2 = 0;
int MIDIdmxVal3 = 0;
int MIDIdmxVal4 = 0;

boolean keyboardThirdDirectionUp = true;
int keyboardThirdValue = 0; // between 0 to 20000
int keyboardThirdChange = 10; // between 5 to 200
int keyboardThirdMin = 0;
int keyboardThirdMax = 20000;

boolean keyboardFourthDirectionUp = true;
int keyboardFourthValue = 0; // between 0 to 20000
int keyboardFourthChange = 10; // between 5 to 200
int keyboardFourthMin = 0;
int keyboardFourthMax = 20000;

boolean lfoDirection1 = true;
int lfoChange1 = 10;
int lfoValue1 = 0;
int lfoMin1 = 0;
int lfoMax1 = 0;

boolean lfoDirection2 = true;
int lfoChange2 = 10;
int lfoValue2 = 0;
int lfoMin2 = 0;
int lfoMax2 = 0;


// variables used for midi stuff
byte incomingByte;
byte note = 0;
byte velocity;
int channel = 0;
int action=2; //0 =note off ; 1=note on ; 2= nada
byte ccValue = 0;
byte ccNumber = 0;

float kickLevel = 1;
float snareLevel = 1;

byte commandByte;
byte noteByte;
byte velocityByte;

boolean noteReady = false;
int numberOfNotesA = 0;
int numberOfNotesB = 0;
int numberOfNotesC = 0;

int modeNumber = 1;

float gaiaLevel = 1;
float korgLevel = 1;
float mophoLevel = 1;

float midiLevel = 1;
float modularLevel = 1;

//
//interface variables

// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 52;    // the number of the pushbutton pin
const int buttonPin2 = 53;    // the number of the pushbutton pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState1 = LOW;
int buttonState2 = LOW;
int lastButtonState1 = LOW;   // the previous reading from the input pin
int lastButtonState2 = LOW;   // the previous reading from the input pin

int ledNumberPins[7] = {27,28,25,24,30,26,29};
int numbers[10][7] = {{1, 1, 1, 0, 1, 1, 1},
                      {0, 0, 1, 0, 0, 1, 0},
                      {1, 0, 1, 1, 1, 0, 1},
                      {1, 0, 1, 1, 0, 1, 1},
                      {0, 1, 1, 1, 0, 1, 0},
                      {1, 1, 0, 1, 0, 1, 1},
                      {1, 1, 0, 1, 1, 1, 1},
                      {1, 0, 1, 0, 0, 1, 0},
                      {1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 0, 1, 0}
                      };

void setup() {
  // put your setup code here, to run once:

  DmxSimple.usePin(3);
  DmxSimple.maxChannel(4);

  pinMode(13,OUTPUT);   // declare the LED's pin as output
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);

  pinMode(ledNumberPins[0], OUTPUT);
  pinMode(ledNumberPins[1], OUTPUT);
  pinMode(ledNumberPins[2], OUTPUT);
  pinMode(ledNumberPins[3], OUTPUT);
  pinMode(ledNumberPins[4], OUTPUT);
  pinMode(ledNumberPins[5], OUTPUT);
  pinMode(ledNumberPins[6], OUTPUT);
  
  Serial.begin(31250);        

  midiA.begin(MIDI_CHANNEL_OMNI);
  midiB.begin(MIDI_CHANNEL_OMNI);
  midiC.begin(MIDI_CHANNEL_OMNI);

  //interface code
  pinMode(52, INPUT);
  pinMode(53, INPUT);

  for (int i=0; i < 7; i++) {
    pinMode(ledNumberPins[i], OUTPUT);
  }
 
}


void loop() {
  // check for midi notes
 // checkMIDI();
  newMIDI();
  if(note!=0) {
    processMIDI();
  }

  readAnalogValues();

  updateScaledNotes(); // update values for the first and second channels
  writeToDMX(); // send dmx commands

  //interface
 
  updateModeCounter();
 
  modeNumber = max(0, modeNumber);
  modeNumber = min(9, modeNumber);
 // modeNumber = 4;
  displayNumber(modeNumber);

}


void displayNumber(int input){
  input = max(0, input);
  input = min(9, input);

  for (int j=0; j < 7; j++) {
    if(numbers[input][j] == 1) {
      digitalWrite(ledNumberPins[j], HIGH);       
    } else {
      digitalWrite(ledNumberPins[j], LOW); 
    }
  }
}

// interface, read buttons
void updateModeCounter(){
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);
    if (reading1 != lastButtonState1) {
    // reset the debouncing timer
      lastDebounceTime1 = millis();
    }
    if (reading2 != lastButtonState2) {
    // reset the debouncing timer
      lastDebounceTime2 = millis();
    }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {

      buttonState1 = reading1;

      if (buttonState1 == HIGH) {
        if(modeNumber < 9){
          modeNumber++;
        }
      }
    }
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {

      buttonState2 = reading2;

      if (buttonState2 == HIGH) {
        if(modeNumber > 0){
          modeNumber--;
        }
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
  
}


// midi
void newMIDI() {
  if (midiA.read())                // Is there a MIDI message incoming ?
    {
        switch(midiA.getType())      // Get the type of the message we caught
        {
            case midi::NoteOn:       // If it is a note on,
                channel = midiA.getChannel();
                note = midiA.getData1();
                velocity = midiA.getData2();
                numberOfNotesA += 1;
                
                break;
            case midi::NoteOff:
                channel = midiA.getChannel();
                numberOfNotesA -= 1;
                numberOfNotesA= max(numberOfNotesA, 0);
                break;
            case midi::ControlChange:
                channel = midiA.getChannel();
                ccNumber = midiA.getData1();
                ccValue = midiA.getData2();
                break;
            // See the online reference for other message types
            default:
                break;
        }
    }
  if (midiB.read())                // Is there a MIDI message incoming ?
    {
        switch(midiB.getType())      // Get the type of the message we caught
        {
            case midi::NoteOn:       // If it is a note on,
                channel = 16 + midiB.getChannel(); //displace channel over 16 to distinguish ports
                note = midiB.getData1();
                velocity = midiB.getData2();
                numberOfNotesB += 1;
                break;
            case midi::NoteOff:
                numberOfNotesB -= 1;
                numberOfNotesB = max(numberOfNotesB, 0);
                break;
            // See the online reference for other message types
            default:
                break;
        }
    }
  if (midiC.read())                // Is there a MIDI message incoming ?
    {
        switch(midiC.getType())      // Get the type of the message we caught
        {
            case midi::NoteOn:       // If it is a note on,
                channel = 32 + midiC.getChannel(); //displace channel over 32 to distinguish ports
                note = midiC.getData1();
                velocity = midiC.getData2();
                numberOfNotesC += 1;
                break;
            case midi::NoteOff:
                numberOfNotesC -= 1;
                numberOfNotesC = max(numberOfNotesC, 0);
                break;
            // See the online reference for other message types
            default:
                break;
        }
    }
    
}

void processMIDI() {
  if (channel == 5) { // drums port 1 channel 5
   if (note == 36) {
      if (velocity > 0) {
        playFirst();
      }
    }
    else if (note == 38){
      if (velocity > 0) {
        playSecond();
      }
    }
    else if (note == 42){
      if (velocity > 0) {
      //  playThird();
      }
    }
    else if (note == 59){
      if (velocity > 0) {
        playFourth();
      }
    }
    if (ccNumber == 24) {
      kickLevel = float(ccValue)/127;
    } else if (ccNumber == 29){
      snareLevel = float(ccValue)/127;
    }
  } else if (channel == 1){ // roland keyboard on port 1 channel one
    if (velocity > 0) {
      playFourth();  
    }
  } else if (channel == 17){ // korg on port 2 channel 1 (channel 17)
    if (velocity > 0) {
      playThirdA();  
    }
  } else if (channel == 34){ // dave smith keyboard port 3 channel 2 (channel 34)
    if (velocity > 0) {
      playThirdB();      
    }
  }
  channel = 0; // reset for new note
  note = 0;
  velocity = 0;
  ccNumber = 0;
  ccValue = 0;
}

float scale2(float input) { // scale -1 to 0 to 1 holding at 0 in the middle
    if(input < 0.4) {
    input = (input*2.5)-1;
  } else if(input > 0.6) {
    input = ((input-0.6)*2.5);    
  } else {
    input = 0;
  }
  return input;
}

void readAnalogValues() {
  potValue1 = analogRead(potPin1); // 0 - 1024
  potFloat1 = float(potValue1)/1024;
  potValue2 = analogRead(potPin2); // 0 - 1024
  potFloat2 = float(potValue2)/1024;
  potValue3 = analogRead(potPin3); // 0 - 1024
  potFloat3 = float(potValue3)/1024;
  potValue4 = analogRead(potPin4); // 0 - 1024
  potFloat4 = float(potValue4)/1024;
  potValue5 = analogRead(potPin5); // 0 - 1024
  potFloat5 = float(potValue5)/1024;
  
  modularFloat1 = float(analogRead(modularPin1))/1024;
  modularFloat2 = float(analogRead(modularPin2))/1024;
  modularFloat3 = float(analogRead(modularPin3))/1024;
  modularFloat4 = float(analogRead(modularPin4))/1024;

  mophoLevel = min(1, max(0, 1+scale2(potFloat2)));
  korgLevel = min(1, max(0, 1-scale2(potFloat2)));

  midiLevel = min(1, max(0, 1-scale2(potFloat5)));
  modularLevel = min(1, max(0, 1+scale2(potFloat5)));
  
}

void updateScaledNotes() {
  
  dimFirst();
  dimSecond();
  dimThirdA();
  dimThirdB();
  dimFourth();

  if (modeNumber == 1) {
   
    MIDIdmxVal1 = int(kickLevel*(float(firstEg)/78.4314));
    MIDIdmxVal2 = int(snareLevel*(float(secondEg)/78.4314));
    MIDIdmxVal3 = int((korgLevel*float(thirdAEg)/78.4314)+(mophoLevel*float(thirdBEg)/78.4314));
    MIDIdmxVal4 = int((float(fourthEg)/78.4314));
   // pot 1 0-1024 - 0 is normal, 1024 is all MIDIdmxVal1 
    MIDIdmxVal2 = int((float(MIDIdmxVal1)*potFloat1)+(float(MIDIdmxVal2)*(1+(-1*potFloat1))));
    MIDIdmxVal3 = int((float(MIDIdmxVal1)*potFloat1)+(float(MIDIdmxVal3)*(1+(-1*potFloat1))));
    MIDIdmxVal4 = int((float(MIDIdmxVal1)*potFloat1)+(float(MIDIdmxVal4)*(1+(-1*potFloat1))));

   // pot 5 (Jesse) to the left is midi control (normal) to the right is modular direct link
    dmxVal1 = int((255*modularLevel*modularFloat1)+(float(MIDIdmxVal1)*midiLevel));
    dmxVal2 = int((255*modularLevel*modularFloat2)+(float(MIDIdmxVal2)*midiLevel));
    dmxVal3 = int((255*modularLevel*modularFloat3)+(float(MIDIdmxVal3)*midiLevel)); // korg/mopho/modular
    dmxVal4 = int((255*modularLevel*modularFloat4)+(float(MIDIdmxVal4)*midiLevel)); // gaia/modular
    
      // pot 3 to the left dims all lights, in the middle no change, to the right turns all on full
    dmxVal1 = int(float(dmxVal1) + (255*scale2(potFloat3)));
    dmxVal2 = int(float(dmxVal2) + (255*scale2(potFloat3)));
    dmxVal3 = int(float(dmxVal3) + (255*scale2(potFloat3)));
    dmxVal4 = int(float(dmxVal4) + (255*scale2(potFloat3)));
     
  }

  if (modeNumber == 2) { // knobs on remote control each channel directly
    dmxVal1 = int(potFloat1*255);
    dmxVal2 = int(potFloat2*255);
    dmxVal3 = int(potFloat3*255);
    dmxVal4 = int(potFloat4*255);
  }
  
  if (modeNumber == 3) { // direct control from modular
    dmxVal1 = int(modularFloat1*255);
    dmxVal2 = int(modularFloat2*255);
    dmxVal3 = int(modularFloat3*255);
    dmxVal4 = int(modularFloat4*255);
  }

  if (modeNumber == 4) { // other modular testing
    dmxVal1 = int(modularFloat1*modularFloat2*255);
    dmxVal2 = int(modularFloat2*255);
    dmxVal3 = int(modularFloat1*modularFloat2*255);
    dmxVal4 = int(modularFloat3*255);
  }

  if (modeNumber == 5) {
    dmxVal1 = potFloat3*255;
    dmxVal2 = dmxVal1;
    dmxVal3 = ((scale2(potFloat3)+1)/2)*255;
    dmxVal4 = ((scale2(potFloat3)+1)/2)*255;
  }

  if (modeNumber == 6) { // full on
    dmxVal1 = 255;
    dmxVal2 = 255;
    dmxVal3 = 255;
    dmxVal4 = 255;
  }

  if (modeNumber == 7) { // pot 5 test
    dmxVal1 = potFloat5*255;
    dmxVal2 = potFloat5*255;
    dmxVal3 = potFloat5*255;
    dmxVal4 = potFloat5*255;
  }
  if (modeNumber == 8)
  {
    dmxVal1 = int(korgLevel*float(255));
    dmxVal2 = int(mophoLevel*float(255));
    dmxVal3 = int(midiLevel*float(255));
    dmxVal4 = int(modularLevel*float(255));
  }
}

void writeToDMX() {
  DmxSimple.write(1, fitIn255(dmxVal1));
  DmxSimple.write(2, fitIn255(dmxVal2));
  DmxSimple.write(3, fitIn255(dmxVal3));
  DmxSimple.write(4, fitIn255(dmxVal4));
}
/*
void dimKeyboardFourthTriangle() {
  if(keyboardFourthDirectionUp){
    if (keyboardFourthValue < keyboardFourthMax) {
      keyboardFourthValue += keyboardFourthChange;
    } else {
      keyboardFourthDirectionUp = false;
      keyboardFourthValue = keyboardFourthMax;
    }
  } else if (keyboardFourthValue > keyboardFourthMin) {
    keyboardFourthValue -= keyboardFourthChange;
  } else {
    keyboardFourthDirectionUp = true;
    keyboardFourthValue = keyboardFourthMin; 
  }
}

void dimKeyboardThirdTriangle() {
  if(keyboardThirdDirectionUp){
    if (keyboardThirdValue < keyboardThirdMax) {
      keyboardThirdValue += keyboardThirdChange;
    } else {
      keyboardThirdDirectionUp = false;
      keyboardThirdValue = keyboardThirdMax;
    }
  } else if (keyboardThirdValue > keyboardThirdMin) {
    keyboardThirdValue -= keyboardThirdChange;
  } else {
    keyboardThirdDirectionUp = true;
    keyboardThirdValue = keyboardThirdMin; 
  }
}
*/
void dimFirst(){
  if (firstEg > 0) {
    firstEg = firstEg - dim1;
  } else {
    firstEg = 0;
  }
}

void dimSecond(){
  if (secondEg > 0) {
    secondEg = secondEg - dim1;
  } else {
    secondEg = 0;
  }
}

void dimThirdA(){
  if (thirdAEg > 0) {
    thirdAEg = thirdAEg - dimKey3;
  } else {
    thirdAEg = 0;
  }
}

void dimThirdB(){
  if (thirdBEg > 0) {
    thirdBEg = thirdBEg - dimKey3;
  } else {
    thirdBEg = 0;
  }
}

void dimFourth(){
  if (fourthEg > 0) {
    fourthEg = fourthEg - dimKey4;
  } else {
    fourthEg = 0;
  }
}


void playFirst(){
  firstEg = 20000;
}

void playSecond(){
  secondEg = 20000;
}

void playThirdA(){
  thirdAEg = 20000;
}
void playThirdB(){
  thirdBEg = 20000;
}

void playFourth(){
  fourthEg = 20000;
}

int fitIn255(int input) {
  return min(max(input, 0), 255);   
}

int pow255curve(int input, float scale) {
  return int(255*pow((float(input)/255),scale));
}

void flashChannel1(){
  DmxSimple.write(1, 255);
  delay(100);
  DmxSimple.write(1, 0);
}
