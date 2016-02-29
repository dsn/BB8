#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>

#define SFX_ACT      3
#define SFX_RST      4
#define SFX_TX       5
#define SFX_RX       6
#define SFX_LED_PIN  9
#define SPK_PIN     A5

SoftwareSerial      ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

uint8_t files;
uint8_t lastfile;

long lasttime = 0;
int ledBoost  = 10;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  
  pinMode(SFX_LED_PIN, OUTPUT);
  pinMode(SFX_ACT, INPUT);
  pinMode(SPK_PIN, INPUT);

   if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");
  files = sfx.listFiles();
}

void loop() {
  
  if(digitalRead(SFX_ACT)) {
    analogWrite(SFX_LED_PIN, 0);
    if(millis() - lasttime >= random(3000, 7500)) {
      int trackNumber = random(0, files);
      if(lastfile != trackNumber) {
        sfx.playTrack(trackNumber);
        lastfile = trackNumber;
        lasttime = millis();
      }
    }
  }
  else {
    int Brightness = constrain(map(analogRead(SPK_PIN), 0, 100, 0, 255) + ledBoost, 0, 255);
    Serial.println(Brightness);
    if(Brightness >= 1) {
      analogWrite(SFX_LED_PIN, Brightness);
    }
    lasttime = millis();
  }
}
