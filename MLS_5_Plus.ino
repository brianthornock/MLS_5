
// Define relay control pin assignments
const uint8_t relay1 = 15; //A1
const uint8_t relay2 = 14; //A0
const uint8_t relay3 = 16; //A2
const uint8_t relay4 = 17; //A3
const uint8_t relay5 = 18; //A4

// Define footswitch input pin assignments
const uint8_t fsw1 = 8;
const uint8_t fsw2 = 10;
const uint8_t fsw3 = 7;
const uint8_t fsw4 = 2;
const uint8_t fsw5 = 3;

// Define exclusivity switch pin assignments
const uint8_t xsw1 = 9;
const uint8_t xsw2 = 11;
const uint8_t xsw3 = 6;
const uint8_t xsw4 = 5;
const uint8_t xsw5 = 4;

const uint8_t debugLED = 19;

// Define some initial parameters
int pollInt = 200; // Interval in ms to poll the exclusive switches
int lastPollTime; //Time stamp of last time we met the polling requirements

uint8_t relay1State = LOW;
uint8_t button1State;
uint8_t lastButton1State = LOW;

uint8_t relay2State = LOW;
uint8_t button2State;
uint8_t lastButton2State = LOW;

uint8_t relay3State = LOW;
uint8_t button3State;
uint8_t lastButton3State = LOW;

uint8_t relay4State = LOW;
uint8_t button4State;
uint8_t lastButton4State = LOW;

uint8_t relay5State = LOW;
uint8_t button5State;
uint8_t lastButton5State = LOW;

// Create bit fields to store relay and xsw states in for cases where we need to restore them
uint8_t currRelayStates = 0;
uint8_t prevRelayStates = 0;
uint8_t xswState = 0;

unsigned long lastDebounceTime = 0; // Counter to keep track of how long since we last tried debouncing
unsigned long debounceDelay = 10; // Debounce counter in ms
uint8_t sw_flag = 0; // Flag register for which switch caused the interrupt
uint8_t tempDebounceState = 0; // Temp register for keeping track of the state of the pressed button
unsigned long lastButtonDownTime = 0; // Counter to keep track of when a button was last pressed down
const unsigned int buttonPressTime = 500; //How many ms for a switch to be held down to be considered a press/hold and thus turn the effect off when released

// Set up parameters for MIDI
unsigned char patchBits[8];
unsigned char mask = 1;
byte patchChange = 193; //Uses MIDI channel 1 (2 on devices where channels are 1 indexed)
byte commandByte;
byte patchByte;

void setup() {

 // Initialize pin modes and states
  pinMode(relay1, OUTPUT);
  pinMode(fsw1, INPUT);
  pinMode(xsw1, INPUT);
  digitalWrite(relay1, LOW);

  pinMode(relay2, OUTPUT);
  pinMode(fsw2, INPUT);
  pinMode(xsw2, INPUT);
  digitalWrite(relay2, LOW);

  pinMode(relay3, OUTPUT);
  pinMode(fsw3, INPUT);
  pinMode(xsw3, INPUT);
  digitalWrite(relay3, LOW);

  pinMode(relay4, OUTPUT);
  pinMode(fsw4, INPUT);
  pinMode(xsw4, INPUT);
  digitalWrite(relay4, LOW);

  pinMode(relay5, OUTPUT);
  pinMode(fsw5, INPUT);
  pinMode(xsw5, INPUT);
  digitalWrite(relay5, LOW);

  pinMode(debugLED, OUTPUT);
  digitalWrite(debugLED,LOW);

  checkXSw();

  // Enable pin change interrupts on Ports B and D for the footswitch input pins
  cli();
  PCICR  |= 0b00000101; //Enable Ports B and D for PCI
  PCMSK0 |= 0b00000101; //Enable interrupts only on these port B pins
  PCMSK2 |= 0b10001100; //Enable interrupts only on these port D pins
  sei(); //Start the interrupt service routine

  //Start up the serial port to begin receiving MIDI
  Serial.begin(31250);
}

//PCI routine for when a footswitch is pressed or released on Port B
ISR(PCINT0_vect){

  // Only deal with the interrupt if we aren't currently dealing with one
  if (sw_flag==0){

    int read1 = digitalRead(fsw1);
    int read2 = digitalRead(fsw2);
  
    //If the switch reading is not the same as the previous state
    if(read1 != lastButton1State){
      sw_flag = 1;
      tempDebounceState = read1;
      lastDebounceTime = millis();
    }
  
    if(read2 != lastButton2State){
      sw_flag = 2;
      tempDebounceState = read2;
      lastDebounceTime = millis();
    }
  }
}



//PCI routine for when a footswitch is pressed or released on Port D
ISR(PCINT2_vect){

  // Only deal with the interrupt if we aren't currently dealing with one
  if (sw_flag==0){

    int read3 = digitalRead(fsw3);
    int read4 = digitalRead(fsw4);
    int read5 = digitalRead(fsw5);
  
    //If the switch reading is not the same as the previous state
    if(read3 != lastButton3State){
      sw_flag = 3;
      tempDebounceState = read3;
      lastDebounceTime = millis();
    }
  
    if(read4 != lastButton4State){
      sw_flag = 4;
      tempDebounceState = read4;
      lastDebounceTime = millis();
    }

    if(read5 != lastButton5State){
      sw_flag = 5;
      tempDebounceState = read5;
      lastDebounceTime = millis();
    }
  }
}



void loop() {
  //If a button was pressed, do the necessary processing
  if (sw_flag != 0) {
    checkSwitch();
  }

  //Check MIDI to see if we have any messages
  checkMIDI();

  if (millis() - lastPollTime >= pollInt){
    checkXSw();
    lastPollTime = millis();
  }
}


void checkSwitch(){
  // Debounce and determine if we have a button press or release condition
  switch (sw_flag) {
    case 1:
      if ((millis()-lastDebounceTime) > debounceDelay){ 
        if (digitalRead(fsw1)==tempDebounceState){
          //The switch has been confirmed as debounced
          button1State = tempDebounceState;
        
          if (button1State == HIGH && lastButton1State == LOW){
            // We have a high to low transition. Mark the time and toggle the relay state.
            relay1State = !relay1State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button1State == LOW && lastButton1State == HIGH && (millis()-lastButtonDownTime<buttonPressTime)){
            // We have a low to high transition, but it hasn't been long enough to be a press/hold, so push currRelayStates to prevRelayStates
            prevRelayStates = currRelayStates;
          }
          if (button1State == LOW && lastButton1State == HIGH && (millis()-lastButtonDownTime>=buttonPressTime)){
            //We have a low to high transition, so we turn the relay off
            relay1State = LOW;
            restoreState();
          }
          digitalWrite(relay1,relay1State);
          bitWrite(currRelayStates,0,relay1State); // Keep track of what our relay states are
          
          if (relay1State == HIGH){
            enforceXSw(1); // Enforce exclusivity if we are turning on this loop
          }
          
          lastButton1State = button1State;
        }
        sw_flag = 0; //Now that we have waited long enough to know if it is debounced and have taken any necessary action, reset flag to low
      }
      break;
    case 2:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(fsw2)==tempDebounceState){
          //The switch has been confirmed as debounced
          button2State = tempDebounceState;
        
          if (button2State == HIGH && lastButton2State == LOW){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay2State = !relay2State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button2State == LOW && lastButton2State == HIGH && (millis()-lastButtonDownTime<buttonPressTime)){
            // We have a low to high transition, but it hasn't been long enough to be a press/hold, so push currRelayStates to prevRelayStates
            prevRelayStates = currRelayStates;
          }
          
          if (button2State == LOW && lastButton2State == HIGH && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay2State = LOW;
            restoreState(); // Restore our previous 
          }
          digitalWrite(relay2,relay2State);
          bitWrite(currRelayStates,1,relay2State); // Keep track of what our relay states are
                    
          if (relay2State == HIGH){
            enforceXSw(2); // Enforce exclusivity if we are turning on this loop
          }
          
          lastButton2State = button2State;
        }
        sw_flag = 0;
      }
      break;

      case 3:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(fsw3)==tempDebounceState){
          //The switch has been confirmed as debounced
          button3State = tempDebounceState;
        
          if (button3State == HIGH && lastButton3State == LOW){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay3State = !relay3State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button3State == LOW && lastButton3State == HIGH && (millis()-lastButtonDownTime<buttonPressTime)){
            // We have a low to high transition, but it hasn't been long enough to be a press/hold, so push currRelayStates to prevRelayStates
            prevRelayStates = currRelayStates;
          }
          
          if (button3State == LOW && lastButton3State == HIGH && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay3State = LOW;
            restoreState();
          }
          digitalWrite(relay3,relay3State);
          bitWrite(currRelayStates,2,relay3State); // Keep track of what our relay states are
                    
          if (relay3State == HIGH){
            enforceXSw(3); // Enforce exclusivity if we are turning on this loop
          }
          
          lastButton3State = button3State;
        }
        sw_flag = 0;
      }
      break;

    case 4:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(fsw4)==tempDebounceState){
          //The switch has been confirmed as debounced
          button4State = tempDebounceState;
        
          if (button4State == HIGH && lastButton4State == LOW){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay4State = !relay4State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button4State == LOW && lastButton4State == HIGH && (millis()-lastButtonDownTime<buttonPressTime)){
            // We have a low to high transition, but it hasn't been long enough to be a press/hold, so push currRelayStates to prevRelayStates
            prevRelayStates = currRelayStates;
          }
          
          if (button4State == LOW && lastButton4State == HIGH && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay4State = LOW;
            restoreState();
          }
          digitalWrite(relay4,relay4State);
          bitWrite(currRelayStates,3,relay4State); // Keep track of what our relay states are
                    
          if (relay4State == HIGH){
            enforceXSw(4); // Enforce exclusivity if we are turning on this loop
          }
          
          lastButton4State = button4State;
        }
        sw_flag = 0;
      }
      break;

    case 5:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(fsw5)==tempDebounceState){
          //The switch has been confirmed as debounced
          button5State = tempDebounceState;
        
          if (button5State == HIGH && lastButton5State == LOW){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay5State = !relay5State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button5State == LOW && lastButton5State == HIGH && (millis()-lastButtonDownTime<buttonPressTime)){
            // We have a low to high transition, but it hasn't been long enough to be a press/hold, so push currRelayStates to prevRelayStates
            prevRelayStates = currRelayStates;
          }
          
          if (button5State == LOW && lastButton5State == HIGH && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay5State = LOW;
            restoreState();
          }
          digitalWrite(relay5,relay5State);
          bitWrite(currRelayStates,4,relay5State); // Keep track of what our relay states are
                    
          if (relay5State == HIGH){
            enforceXSw(5); // Enforce exclusivity if we are turning on this loop
          }
          
          lastButton5State = button5State;
        }
        sw_flag = 0;
      }
      break;
  }
}// end checkSwitch


void checkMIDI(){

  // Check to see if we have MIDI
  do {

    if (Serial.available()) {
      commandByte = Serial.read(); // read first byte
      patchByte = Serial.read();
      if (commandByte == patchChange) {
        // Figure out which patch we are changing by parsing
        for (int i = 0; i < 8; i++) {
          patchBits[i] = (patchByte & (mask << i)) != 0;
        }

        // Match up the bits with the right relay and
        //turn all relays on or off depending on what
        //the patch change says

        //Patch Number 1 (+1) goes to Relay 1
        if (patchBits[0] == 1) {
          digitalWrite(relay1, HIGH);
          relay1State = HIGH;
        }
        else {
          digitalWrite(relay1, LOW);
          relay1State = LOW;
        }
        bitWrite(currRelayStates,0,relay1State);

        //Patch Number 2 (+1) goes to Relay 2
        if (patchBits[1] == 1) {
          digitalWrite(relay2, HIGH);
          relay2State = HIGH;
        }
        else {
          digitalWrite(relay2, LOW);
          relay2State = LOW;
        }
        bitWrite(currRelayStates,1,relay2State);

        //Patch Number 4 (+1) goes to Relay 3 (2^2) (+1)
        if (patchBits[2] == 1) {
          digitalWrite(relay3, HIGH);
          relay3State = HIGH;
        }
        else {
          digitalWrite(relay3, LOW);
          relay3State = LOW;
        }
        bitWrite(currRelayStates,2,relay3State);

        //Patch Number 8 (+1) goes to Relay 4 (2^3) (+1)
        if (patchBits[3] == 1) {
          digitalWrite(relay4, HIGH);
          relay4State = HIGH;
        }
        else {
          digitalWrite(relay4, LOW);
          relay4State = LOW;
        }
        bitWrite(currRelayStates,3,relay4State);

        //Patch Number 16 (+1) goes to Relay 5 (2^4) (+1)
        if (patchBits[4] == 1) {
          digitalWrite(relay5, HIGH);
          relay5State = HIGH;
        }
        else {
          digitalWrite(relay5, LOW);
          relay5State = LOW;
        }
        bitWrite(currRelayStates,4,relay5State);
        
        
        prevRelayStates = currRelayStates;
        // Should exclusivity be enforced on MIDI change? Hmm...
        //enforceXSw(); // Make sure to enforce exclusivity

      }

    }
  } while (Serial.available() > 1);
}// end checkMIDI


// Function for checking exclusivity switches
void checkXSw(){

  int xsw1State = (digitalRead(xsw1)==HIGH);
  int xsw2State = (digitalRead(xsw2)==HIGH);
  int xsw3State = (digitalRead(xsw3)==HIGH);
  int xsw4State = (digitalRead(xsw4)==HIGH);
  int xsw5State = (digitalRead(xsw5)==HIGH);

  
  bitWrite(xswState,0,xsw1State);
  bitWrite(xswState,1,xsw2State);
  bitWrite(xswState,2,xsw3State);
  bitWrite(xswState,3,xsw4State);
  bitWrite(xswState,4,xsw5State);
}


// Function for handling/enforcing exclusivity switches
void enforceXSw(int currXSw){
  
  int xsw1_state = bitRead(xswState,0);
  int xsw2_state = bitRead(xswState,1);
  int xsw3_state = bitRead(xswState,2);
  int xsw4_state = bitRead(xswState,3);
  int xsw5_state = bitRead(xswState,4);

  // Write all relays that have xswX_state = 1 that are not currXSw
  if (xsw1_state == 1 && currXSw != 1) {
    relay1State = LOW;
    bitWrite(currRelayStates,0,relay1State);
    digitalWrite(relay1,relay1State);
  }

  if (xsw2_state == 1 && currXSw != 2) {
    relay2State = LOW;
    bitWrite(currRelayStates,1,relay2State);
    digitalWrite(relay2,relay2State);
  }

  if (xsw3_state == 1 && currXSw != 3) {
    relay3State = LOW;
    bitWrite(currRelayStates,2,relay3State);
    digitalWrite(relay3,relay3State);
  }

  if (xsw4_state == 1 && currXSw != 4) {
    relay4State = LOW;
    bitWrite(currRelayStates,3,relay4State);
    digitalWrite(relay4,relay4State);
  }

  if (xsw1_state == 1 && currXSw != 5) {
    relay5State = LOW;
    bitWrite(currRelayStates,4,relay5State);
    digitalWrite(relay5,relay5State);
  }
}


// Function to restore the previous relay states after a release from a press and hold
void restoreState(){
  int state1 = bitRead(prevRelayStates,0);
  int state2 = bitRead(prevRelayStates,1);
  int state3 = bitRead(prevRelayStates,2);
  int state4 = bitRead(prevRelayStates,3);
  int state5 = bitRead(prevRelayStates,4);

  int xsw1_state = bitRead(xswState,0);
  int xsw2_state = bitRead(xswState,1);
  int xsw3_state = bitRead(xswState,2);
  int xsw4_state = bitRead(xswState,3);
  int xsw5_state = bitRead(xswState,4);

  // Only restore if the relay was turned off due to having an exclusivity switch on
  if (state1==1 && xsw1_state==1){
    digitalWrite(relay1,HIGH);
  }

    if (state2==1 && xsw2_state==1){
    digitalWrite(relay2,HIGH);
  }

    if (state3==1 && xsw3_state==1){
    digitalWrite(relay3,HIGH);
  }

    if (state4==1 && xsw4_state==1){
    digitalWrite(relay4,HIGH);
  }

    if (state5==1 && xsw5_state==1){
    digitalWrite(relay5,HIGH);
  }

  //Now that we have restored, update the current states to be correct
  currRelayStates = prevRelayStates;
}
