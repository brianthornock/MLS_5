int relay1 = 10;
int switch1 = 11;

int relay2 = 8;
int switch2 = 9;

int relay3 = 6;
int switch3 = 7;

int relay4 = 4;
int switch4 = 5;

int relay5 = 2;
int switch5 = 3;

int loopDelay = 50;

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

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;
unsigned long lastDebounceTime5 = 0;
unsigned long debounceDelay = 10;

unsigned char patchBits[8];
unsigned char mask = 1;
byte patchChange = 193; //Uses MIDI channel 1 (2 on devices where channels are 1 indexed)
byte commandByte;
byte patchByte;

void setup() {
  Serial.begin(31250);

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
}

void switchDebounce() {
  int reading1 = digitalRead(switch1);
  int reading2 = digitalRead(switch2);
  int reading3 = digitalRead(switch3);
  int reading4 = digitalRead(switch4);
  int reading5 = digitalRead(switch5);

  //Relay 1
  if (reading1 != lastButton1State) {
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {

    if (reading1 != button1State) {

      button1State = reading1;

      if (button1State == HIGH) {
        relay1State = !relay1State;

      }
    }
  }

  lastButton1State = reading1;

  //Relay 2
  if (reading2 != lastButton2State) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {

    if (reading2 != button2State) {

      button2State = reading2;

      if (button2State == HIGH) {
        relay2State = !relay2State;
      }
    }
  }

  lastButton2State = reading2;

  //Relay 3
  if (reading3 != lastButton3State) {
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime3) > debounceDelay) {

    if (reading3 != button3State) {

      button3State = reading3;

      if (button3State == HIGH) {
        relay3State = !relay3State;
      }
    }
  }

  lastButton3State = reading3;

  //Relay 4
  if (reading4 != lastButton4State) {
    lastDebounceTime4 = millis();
  }

  if ((millis() - lastDebounceTime4) > debounceDelay) {

    if (reading4 != button4State) {

      button4State = reading4;

      if (button4State == HIGH) {
        relay4State = !relay4State;
      }
    }
  }

  lastButton4State = reading4;

  //Relay 5
  if (reading5 != lastButton5State) {
    lastDebounceTime5 = millis();
  }

  if ((millis() - lastDebounceTime5) > debounceDelay) {

    if (reading5 != button5State) {

      button5State = reading5;

      if (button5State == HIGH) {
        relay5State = !relay5State;
      }
    }
  }

  lastButton5State = reading5;
}

void checkSwitchandMIDI() {

  // Check to see if switch has been pressed
  switchDebounce();
  if (button1State == HIGH) {
    digitalWrite(relay1, relay1State);
  }

  if (button2State == HIGH) {
    digitalWrite(relay2, relay2State);
  }

  if (button3State == HIGH) {
    digitalWrite(relay3, relay3State);
  }

  if (button4State == HIGH) {
    digitalWrite(relay4, relay4State);
  }

  if (button5State == HIGH) {
    digitalWrite(relay5, relay5State);
  }

  delay(2);

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

}

void loop() {
  checkSwitchandMIDI();
  // Check with MIDI to see if it needs to run on a timer
  // at a faster polling rate. This would require putting the
  // MIDI stuff in its own function
  delay(loopDelay);
}
