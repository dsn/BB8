#include <SoftwareSerial.h>

#define BUTTON1_PIN 4
#define BUTTON2_PIN 5
#define BUTTON3_PIN 6

int buttonOneState;
int buttonOneLastState = HIGH;
long buttonOneDebounceTime = 0;

int buttonTwoState;
int buttonTwoLastState = HIGH;
long buttonTwoDebounceTime = 0;

int buttonThreeState;
int buttonThreeLastState = HIGH;
long buttonThreeDebounceTime = 0;

long debounceDelay = 50;

SoftwareSerial xbee = SoftwareSerial(2, 3);

void setup() {
  // Used to transmit data across the xbee
  xbee.begin(9600);
  // Setup our Buttons
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
}

void loop() {

  int buttonOne   = digitalRead(BUTTON1_PIN);
  int buttonTwo   = digitalRead(BUTTON2_PIN);
  int buttonThree = digitalRead(BUTTON3_PIN);
  long now = millis();
  if (buttonOne != buttonOneLastState) {
    buttonOneDebounceTime = now;
  }

  if (buttonTwo != buttonTwoLastState) {
    buttonTwoDebounceTime = now;
  }

  if (buttonThree != buttonThreeLastState) {
    buttonTwoDebounceTime = now;
  }

  if ((millis() - buttonOneDebounceTime) > debounceDelay) {
    if (buttonOne != buttonOneState) {
      buttonOneState = buttonOne;
      if (buttonOneState == HIGH) {
        xbee.write("1");
      }
    }
  }
  if ((millis() - buttonTwoDebounceTime) > debounceDelay) {
    if (buttonTwo != buttonTwoState) {
      buttonTwoState = buttonTwo;
      if (buttonTwoState == HIGH) {
        xbee.write("2");
      }
    }
  }
  if ((millis() - buttonThreeDebounceTime) > debounceDelay) {
    if (buttonThree != buttonThreeState) {
      buttonThreeState = buttonThree;
      if (buttonThreeState == HIGH) {
        xbee.write("3");
      }
    }
  }

  buttonOneLastState   = buttonOne;
  buttonTwoLastState   = buttonTwo;
  buttonThreeLastState = buttonThree;
}
