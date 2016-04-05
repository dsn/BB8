#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <Adafruit_NeoPixel.h>

#define SFX_RST          4  // Sound FX Board Reset
#define SFX_TX           5  // Sound FX Board Transmit
#define SFX_RX           6  // Sound FX Board Recieve
#define SFX_ACT          7  // Sound FX Board Activity
#define HOLO_PIN         8  // Hololens LED
#define SFX_LED_PIN      9  // PSI Sound Reactive LED
#define SPK_PIN         A5  // Speaker Line In

uint8_t files;              // Stores list of files
int     lasttrack;          // Stores the last track played (prevent same random track playe twice)

// Software Serial for Sound FX Board
SoftwareSerial      ss  = SoftwareSerial(SFX_TX, SFX_RX);

// Sound FX Board
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

// Neo Pixel for PSI
Adafruit_NeoPixel   psi = Adafruit_NeoPixel(1, SFX_LED_PIN, NEO_GRB + NEO_KHZ800);

// Software Serial for XBee
//SoftwareSerial     xbee = SoftwareSerial(2, 3);

void setup() {
  // Initialize Hardware Serial
  Serial.begin(9600);

  // Initialize XBee Software Serial
  //xbee.begin(9600);

  // Initialize Sound FX Board Software Serial
  ss.begin(9600);

  // Initialize PSI Neo Pixel
  psi.begin();
  // Set Pixel to Off
  psi.setPixelColor(0, 0, 0, 0);
  // Update Pixel Display
  psi.show();

  // Set Sound FX Board Activity as Input
  pinMode(SFX_ACT, INPUT);
  // Set Speaker Line IN as Input
  pinMode(SPK_PIN, INPUT);
  // Set Hololens as Output
  pinMode(HOLO_PIN, OUTPUT);
  pinMode(SFX_LED_PIN, OUTPUT);

  ss.listen();
  // Initialize SFX Board
  if (!sfx.reset()) {
    //Serial.println("SFX Board Not found");
    while (1);
  }

  // Get a list of files from Sound FX Board
  files = sfx.listFiles();

  Serial.println("File Listing");
      Serial.println("========================");
      Serial.println();
      Serial.print("Found "); Serial.print(files); Serial.println(" Files");
      for (uint8_t f=0; f<files; f++) {
        Serial.print(f); 
        Serial.print("\tname: "); Serial.print(sfx.fileName(f));
        Serial.print("\tsize: "); Serial.println(sfx.fileSize(f));
      }
      Serial.println("========================");

  // Play Power up sound
  sfx.playTrack("STARTUP OGG");

  // Turn Hololens On
  digitalWrite(HOLO_PIN, HIGH);
}

void loop() {
  // Variable to store input from XBee / Serial
  char input;
  if (digitalRead(SFX_ACT) == LOW) {
    int Brightness = constrain(map(analogRead(SPK_PIN), 0, 100, 0, 255) + 10, 0, 255);
    psi.setPixelColor(0, 0, 0, Brightness);
    psi.show();
  }

  if (digitalRead(SFX_ACT) == HIGH) {
    psi.setPixelColor(0, 0, 0, 0);
    psi.show();

    //xbee.listen();
    // Check for incoming data
    if (Serial.available() > 0)  {
      input = Serial.read();
      Serial.println(input);
      //xbee.end();
      switch (input) {
        case '1': {
            sfx.playTrack("00000007OGG");
          }
          break;
        case '2': {
            sfx.playTrack("00000005OGG");
          }
          break;
        case '3': {
            uint8_t rTrack = random(1, files);
            if (rTrack != 0 && rTrack != 3 && rTrack != 5) {
              sfx.playTrack(rTrack);
            }
          }
          break;
        default:
          //Serial.println(input);
          break;
      }
      lasttrack = input;
    }
  }
}
