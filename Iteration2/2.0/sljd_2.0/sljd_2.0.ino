// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

//Dino positions
#define run_1 0//run position 1
#define run_2 1//run position 2
#define jump_high 2//jump high
#define jump_half_head 3//jump half upper
#define jump_half_body 4//jump half lower

#define upperObject1 5
#define lowerObject1 6
#define empty 7

int dino_up_pos=0;
int dino_low_pos=0;

static bool buttonPushed = false;
static byte currentPos;
static byte currentPosUp;
static byte currentPosDown;
static int score = 0;
static bool playing = false;
static bool scoring =false;
const int buttonPin = 2;
int buttonState = 0;
static int upperBG[17] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
static int lowerBG[17] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

// 'Dino_Run_1', 20x28px
const unsigned char run1[] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xc0, 0x00, 0xff,
0xc8, 0x00, 0xff, 0xc0, 0x00, 0xff, 0xc0, 0x00, 0xff, 0xc0, 0xf0, 0xff, 0xc1, 0xf0, 0xff, 0x80,
0x00, 0xff, 0x80, 0xf0, 0xff, 0x80, 0x30, 0xff, 0x00, 0x10, 0xfe, 0x01, 0xd0, 0x7e, 0x0f, 0xf0,
0xbc, 0x0d, 0xf0, 0x80, 0x1d, 0xf0, 0xc0, 0x1b, 0xf0, 0xe0, 0x17, 0xf0, 0xf8, 0x27, 0xf0, 0xff,
0x93, 0xf0, 0xff, 0xb9, 0xf0, 0xff, 0x9b, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char run2 [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char halfhead [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char halfbody [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char jumphigh [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char empt [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char top1 [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};

const unsigned char bot1 [] PROGMEM = {
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0,
0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff,
0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff,
0xf0, 0xff, 0xff, 0xf0
};


*unsigned char objects(int n){
  switch(n){
  case upperObject1:
    return *top1;
  case lowerObject1:
    return *bot1;
  case empty:
    return *empt;
  }
}

void drawDino0(){
  //Starting screen
  tft.setCursor (0,0);
  tft.println("Press to Start!");
  tft.drawBitmap(0, 36, run_1,40, 56, RED);
}
void drawDinoWalk(){
  dino_low_pos=1;
  tft.setCursor(0,1);
  tft.drawBitmap(0, 36, run_2,20, 28, RED);
  delay(200);
  tft.drawBitmap(0, 36, run_1,20, 28, RED);
  delay(200);
  detect();
}
void drawDinoJump(){
  //half up + down
  dino_low_pos=1;
  dino_up_pos=1;
  tft.setCursor (0,0);
  tft.drawBitmap(0, 8, halfhead,20, 28, RED);
  tft.drawBitmap(0, 36, halfbody,20, 28, RED);
  detect();
  
  delay(200);
  drawObject();
  up
  dino_low_pos=0;
  dino_up_pos=1;
  lcd.setCursor (0,0);
  tft.drawBitmap(0, 8, jumphigh,20, 28, RED);
  tft.drawBitmap(0, 36, empt,20, 28, RED);
  detect();
  
  
  delay(200);
  drawObject();
  detect();
  delay(200);
  drawObject();
  detect();
  
  //down
  dino_low_pos=1;
  dino_up_pos=1;
  lcd.setCursor (0,0);
  tft.drawBitmap(0, 8, halfhead,20, 28, RED);
  tft.drawBitmap(0, 36, halfbody,20, 28, RED);
  delay(200);
  tft.drawBitmap(0, 8, empt,20, 28, RED);
  dino_up_pos=0;
  dino_low_pos=1;
  detect();
}
void objectRandom (int upperBG[], int lowerBG[]){

  int ObjectPos = random(5,10);
  
  //Object Array
  switch (ObjectPos){
  case upperObject1:
    lowerBG[16] = empty;
    //upper and lower too close
    if (lowerBG[15] == lowerObject1) upperBG[16] = empty;
    else upperBG[16] = upperObject1;    
   break;
  case lowerObject1:
    //Max two lower objects in a row or upper and lower too close
    if ((lowerBG[15] == lowerObject1)||(lowerBG[14] == lowerObject1)|| (lowerBG[13] == lowerObject1)||(upperBG[15] == upperObject1)){
      lowerBG[16] = empty;
    }
    else lowerBG[16] = lowerObject1;
   break;
  case empty:
  case 8: //empty2
  case 9: //empty3
  case 10: //empty4
    upperBG[16] = empty;
    lowerBG[16] = empty;
   break;
  }
}
void buttonPush() {
  buttonPushed = true;
}

void detect(){
  if((dino_up_pos==1&&upperBG[0]!=7)||(dino_low_pos==1&&lowerBG[0]!=7)){
    tft.fillScreen(BLACK);
    playing = false;
    scoring = false;
  }
}

void drawObject (){
  objectRandom(upperBG,lowerBG);
  
  for (int i = 0; i < 16; i++){
    upperBG[i]=upperBG[i+1];
    lowerBG[i]=lowerBG[i+1];
  }
  if(upperBG[0]!=7){
      tft.drawBitmap(0, 8, top1, 20, 28, RED);
  }
  else if(dino_up_pos==0){
      tft.drawBitmap(0, 8, empt, 20, 28, RED);
  }


  if(upperBG[0]!=7){
      tft.drawBitmap(0, 36, bot1, 20, 28, RED);
  }
  else if(dino_up_pos==0){
      tft.drawBitmap(0, 36, empt, 20, 28, RED);
  }

  for (int i = 1; i < 16; i++){
      tft.drawBitmap(20*i, 8, objects(upperBG[i]), 20, 28, RED);
      tft.drawBitmap(20*i, 36, objects(lowerBG[i]), 20, 28, RED);
  }

}

void over(){
   tft.fillScreen(BLACK);
   tft.setCursor (0,0);
   tft.println ("Game Over!");
   tft.setCursor (0,8);
   tft.println ("Score:");
   tft.setCursor (20,8);
   tft.println (score);
   delay(100000000000000000);
}

void setup(void) {

  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
  }
  tft.begin(identifier);

 
}

void loop(void) {
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  //detect();
  if (playing == true){
  drawObject();
  }
  if (!playing) {
    if(score==0){
      drawDino0();
    }
    else{
      over();
    }
  }
  else {
    drawDinoWalk();
    if(buttonPushed){
      drawDinoJump();
      buttonPushed = false;
    }
  }
  
  if (buttonPushed) {
    graphics();
    playing = true;
    scoring = true;
    buttonPushed = false;
    tft.fillScreen(BLACK);
  }
  if (scoring == true){
    score++;
  }
  //Generate object
  
    return;
}
