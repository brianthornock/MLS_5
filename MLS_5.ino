// Define the relay and switch pins
int relay1 = 10;
int switch1 = 11; //D11

int relay2 = 8;
int switch2 = 9;

int relay3 = 6;
int switch3 = 7;

int relay4 = 4;
int switch4 = 5;

int relay5 = 2;
int switch5 = 3;

// Define variables for relay and button states
int relay1State = LOW;
int button1State;
int lastButton1State = LOW;

int relay2State = LOW;
int button2State;
int lastButton2State = LOW;

int relay3State = LOW;
int button3State;
int lastButton3State = LOW;

int relay4State = LOW;
int button4State;
int lastButton4State = LOW;

int relay5State = LOW;
int button5State;
int lastButton5State = LOW;

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
  
  //Configure pin modes and states
  pinMode(relay1, OUTPUT);
  pinMode(switch1, INPUT_PULLUP);
  digitalWrite(relay1, LOW);

  pinMode(relay2, OUTPUT);
  pinMode(switch2, INPUT_PULLUP);
  digitalWrite(relay2, LOW);

  pinMode(relay3, OUTPUT);
  pinMode(switch3, INPUT_PULLUP);
  digitalWrite(relay3, LOW);

  pinMode(relay4, OUTPUT);
  pinMode(switch4, INPUT_PULLUP);
  digitalWrite(relay4, LOW);

  pinMode(relay5, OUTPUT);
  pinMode(switch5, INPUT_PULLUP);
  digitalWrite(relay5, LOW);

  //Configure pin change interrupts for all the input switches
  PCICR  = 1<<PCIE2 | 1<<PCIE0; //Turn on ports B and D
  PCMSK0 |= 1<<PCINT3 | 1<< PCINT1;// Enable pins PB3 and PB1 
  PCMSK2 |= 1<<PCINT23 | 1<<PCINT21 | PCINT19;//Enable pins PD7, PD5, and PD3
  sei(); // Start interrupts

  // Start up serial comms for MIDI
  Serial.begin(31250);
}

// Interrupt routine for loops 1 and 2
ISR(PCINT0_vect){

  //Only deal with the interrupt if we aren't currently dealing with one
  if (sw_flag==0){
    int read1 = digitalRead(switch1);
    int read2 = digitalRead(switch2);
    
    if (read1 != lastButton1State){
      sw_flag = 1;
      tempDebounceState = read1;
      lastDebounceTime = millis();
    }
  
    if (read2 != lastButton2State){
      sw_flag = 2;
      tempDebounceState = read2;
      lastDebounceTime = millis();
    }
  }
}// end ISR(PCINT0_vect)

// Interrupt routine for loops 3-5
ISR(PCINT2_vect){

  //Only deal with the interrupt if we aren't currently dealing with one
  if (sw_flag==0){
    int read3 = digitalRead(switch3);
    int read4 = digitalRead(switch4);
    int read5 = digitalRead(switch5);
    
     if (read3 != lastButton3State){
      sw_flag = 3;
      tempDebounceState = read3;
      lastDebounceTime = millis();
    }
  
    if (read4 != lastButton4State){
      sw_flag = 4;
      tempDebounceState = read4;
      lastDebounceTime = millis();
    }
  
    if (read5 != lastButton5State){
      sw_flag = 5;
      tempDebounceState = read5;
      lastDebounceTime = millis();
    }
  }
}// end ISR(PCINT2_vect)



void loop() {
  //If a button was pressed, do the necessary processing
  if (sw_flag != 0) {
    checkSwitch();
  }

  //Check MIDI to see if we have any messages
  checkMIDI();
}// end loop()




void checkSwitch(){
  // Debounce and determine if we have a button press or release condition
  switch (sw_flag) {
    case 1:
      if ((millis()-lastDebounceTime) > debounceDelay){ 
        if (digitalRead(switch1)==tempDebounceState){
          //The switch has been confirmed as debounced
          button1State = tempDebounceState;
        
          if (button1State == LOW && lastButton1State == HIGH){
            // We have a high to low transition. Mark the time and toggle the relay state.
            relay1State = !relay1State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button1State == HIGH && lastButton1State == LOW && (millis()-lastButtonDownTime>buttonPressTime)){
            //We have a low to high transition, so we turn the relay off
            relay1State = LOW;
          }
          digitalWrite(relay1,relay1State);
          lastButton1State = button1State;
        }
        sw_flag = 0; //Now that we have waited long enough to know if it is debounced and have taken any necessary action, reset flag to low
      }
      break;
    case 2:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(switch2)==tempDebounceState){
          //The switch has been confirmed as debounced
          button2State = tempDebounceState;
        
          if (button2State == LOW && lastButton2State == HIGH){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay2State = !relay2State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button2State == HIGH && lastButton2State == LOW && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay2State = LOW;
            
          }
          digitalWrite(relay2,relay2State);
          lastButton2State = button2State;
        }
        sw_flag = 0;
      }
      break;

      case 3:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(switch2)==tempDebounceState){
          //The switch has been confirmed as debounced
          button3State = tempDebounceState;
        
          if (button3State == LOW && lastButton3State == HIGH){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay3State = !relay3State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button3State == HIGH && lastButton3State == LOW && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay3State = LOW;
            
          }
          digitalWrite(relay3,relay3State);
          lastButton3State = button3State;
        }
        sw_flag = 0;
      }
      break;

    case 4:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(switch4)==tempDebounceState){
          //The switch has been confirmed as debounced
          button4State = tempDebounceState;
        
          if (button4State == LOW && lastButton4State == HIGH){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay4State = !relay4State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button4State == HIGH && lastButton4State == LOW && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay4State = LOW;
            
          }
          digitalWrite(relay4,relay4State);
          lastButton4State = button4State;
        }
        sw_flag = 0;
      }
      break;

    case 5:
      if ((millis()-lastDebounceTime) > debounceDelay){
        if (digitalRead(switch5)==tempDebounceState){
          //The switch has been confirmed as debounced
          button5State = tempDebounceState;
        
          if (button5State == LOW && lastButton5State == HIGH){
            // We have a low to high transition. Mark the time and toggle the relay state.
            relay5State = !relay5State;
            lastButtonDownTime = millis();
          }
          //Check to see if we had an on/off transition. If so, see if it has been at least buttonPressTime long
          if (button5State == HIGH && lastButton5State == LOW && millis()-lastButtonDownTime>buttonPressTime){
            //We have a high to low transition, so we turn the relay off
            relay5State = LOW;
            
          }
          digitalWrite(relay5,relay5State);
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

        //Patch Number 2 (+1) goes to Relay 2
        if (patchBits[1] == 1) {
          digitalWrite(relay2, HIGH);
          relay2State = HIGH;
        }
        else {
          digitalWrite(relay2, LOW);
          relay2State = LOW;
        }

        //Patch Number 4 (+1) goes to Relay 3 (2^2) (+1)
        if (patchBits[2] == 1) {
          digitalWrite(relay3, HIGH);
          relay3State = HIGH;
        }
        else {
          digitalWrite(relay3, LOW);
          relay3State = LOW;
        }

        //Patch Number 8 (+1) goes to Relay 4 (2^3) (+1)
        if (patchBits[3] == 1) {
          digitalWrite(relay4, HIGH);
          relay4State = HIGH;
        }
        else {
          digitalWrite(relay4, LOW);
          relay4State = LOW;
        }

        //Patch Number 16 (+1) goes to Relay 5 (2^4) (+1)
        if (patchBits[4] == 1) {
          digitalWrite(relay5, HIGH);
          relay5State = HIGH;
        }
        else {
          digitalWrite(relay5, LOW);
          relay5State = LOW;
        }

      }

    }
  } while (Serial.available() > 1);
}// end checkMIDI
