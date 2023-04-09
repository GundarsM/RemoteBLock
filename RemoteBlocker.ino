#include <APA102.h>
#include <SimpleKeypad.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTCZero.h>

/////////////////***  LEDS  ***///////////////////

// LED strip pins.
const uint8_t dataPinB = A1; // B - bar
const uint8_t clockPinB = 6; // P - PAD
const uint8_t dataPinP = 14;
const uint8_t clockPinP = 13;

//LED strips
APA102<dataPinB, clockPinB> ledStripB;
APA102<dataPinP, clockPinP> ledStripP;

// Set the number of LEDs to control.
const uint8_t ledCountB = 33; //-33
const uint8_t ledCountP = 28;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colorsB[ledCountB];
rgb_color colorsP[ledCountP];
const uint16_t padLeds[] = {1,  3,  5,  7,
                           14, 12, 10,  8,
                           15, 17, 19, 21,
                           28, 26, 24, 22}; 
const uint16_t padLeds2[] ={1,14,15,28,
                            3,12,17,26,
                            5,10,19,24,
                            7, 8,21,22};                           
const uint16_t correction[] = {1, 14, 15, 28,
                               3, 12, 17, 26,
                               5, 10, 19, 24,
                               7,  8, 21, 22};                           
uint16_t setLeds[16] = {};                                                  

// Set the brightness to use (the maximum is 31).
const uint8_t brightness = 1;

/////////////////***    KEYPAD    ***///////////////////
char key_chars[4][4] = {            // The symbols of the keys
  {1, 14, 15, 28},
  {3, 12, 17, 26},
  {5, 10, 19, 24},
  {7, 8, 21, 22}
};

byte rowPins[4] = {A6,0,1,2};              // The pins where the rows are connected
byte colPins[4] = {A2,A3,A4,A5};           // The pins where the columns are connected
SimpleKeypad kp1((char*)key_chars, rowPins, colPins, 4, 4);   // New keypad called kp1

uint16_t dataIn = 0;

////**   SERVO MOTORS   **////
Servo myservo0, myservo1;


////**   OLED   **////
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64
#define OLED_RESET     1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


////**   RTC   **////
RTCZero rtc;
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 0;
/* Change these values to set the current initial date */
const byte day = 4;
const byte month = 2;
const byte year = 21;
char laiks[] = "00/00/00  00:00:00";
uint64_t clockRefreshStart = millis();

////**   FOR GAME 1   **////
uint16_t currentLevel = 0;
uint8_t gameFailed = 0;

/* initial data for testing */
/*uint16_t levelData[60][2] ={{0,1},                           {1,0},                           {1,0},                           {0,1},
                           {1,0},                           {0,1},                           {1,0},                           {0,1},
                           {0,1},                           {0,1},                           {1,0},                           {1,0},
                           {0,1},                           {1,0},                           {1,0},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                           {0,1},                           {0,1},
                           {0,1},                           {0,1},                            {0,1},                          {0,1}};*/

uint16_t levelData[60][2] =  {{0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0}};

uint16_t eneteredData[60][2] ={{0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0},
                              {0,0},                              {0,0},                              {0,0},                              {0,0}};                           
uint8_t falseRead = 0;

////**   FOR GAME 2   **////
uint16_t levelDataP[16] = {0,0,0,0,
                           0,0,0,0,
                           0,0,0,0,
                           0,0,0,0};
uint16_t checkDataP[16] = {0,0,0,0,
                           0,0,0,0,
                           0,0,0,0,
                           0,0,0,0};                           
uint16_t countActiveCells = 0;
//uint16_t patternLevel =0;
uint16_t pressedTwice =0;
uint16_t previewTime = 700;

////**   FOR GLOBAL RULES   **////
uint16_t gamePassed = 0;
uint16_t outOfGame = 1; 
uint16_t watchTimeMin = 0;
uint16_t relaxTimeMin = 0;
uint16_t allowPlay = 0;
const uint16_t hours21 = 64800; //21 hours for break
const uint16_t hours3 = 10800;  //3 hours for watctime


void setup()
{
  Serial.begin(9600);
  myservo0.attach(4);  
  myservo1.attach(5);

  rtc.begin(); // initialize RTC
  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  closeServos();
  delay(2000);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }  
}

void loop()
{
  /*while (!Serial.available()){}
  dataIn = Serial.parseInt();
  Serial.flush();*/
  if(procesKeypad()!=0){    //Izvēlās Random spēli 
    randomSeed(millis());
    dataIn = random(0,19)%2+1;
    //Serial.println(dataIn);
    delay(1000);
    }

  if(gamePassed){           //Ja spēli uzvar tad atbrīvo pulti!
    //Serial.println("OPEN sesamy ");
    openServos();  
    allowPlay = 0; 
    //fillBarColors(watchTimeMin/20*100); 
    if( watchTimeMin>20){        //20 is for testing, should be 10800 - 3 h - the remote must be placed on top off device
        //Serial.println("TIME ELAPSED");
        soundOn(1500);
        while(!readMegnet()){
          //Serial.println("PUT THE MAGNET"); 
          //Serial.println(readMegnet());
          fillBarColors(100);
          delay(200);
          fillBarColors(0);
          delay(200);
        }                
        relaxTimeMin=0;
        soundOn(0);
        gamePassed=0;
        watchTimeMin=0;
        for(uint8_t i=0;i<5;i++){
          fillBarWhiteColors(100);
          delay(500);
          fillBarWhiteColors(0);
          delay(500);
        }
        
        closeServos();
        rtc.setHours(0);
        rtc.setMinutes(0);
        rtc.setSeconds(0);
        
    }
  }//END of GAME PASSED

  if(relaxTimeMin>20){   //Ja atpūtas brīdis pavadīts, atslēdz iespēju vinnēt spēles
    allowPlay = 1;    
  }

  
  if (millis()-clockRefreshStart>1000){ //once in a second refresh OLED
      clockRefreshStart = millis();
      
      drawText(); //normal cloks
      
      if(gamePassed && outOfGame){
        watchTimeMin++;//skaitam sekundes, kas pienākās, lai skatītos tv
        if (watchTimeMin<20){
          uint32_t powerRed = (double)watchTimeMin/20*100;
          if (powerRed >100) powerRed = 100;
          fillBarColors((uint8_t)powerRed);
        }
        //Serial.print("Watch time ");
        //Serial.println(watchTimeMin);
      } else {
        relaxTimeMin++;
        //Serial.print("Relax time ");
        //Serial.println(relaxTimeMin);
        if (relaxTimeMin<20){
          uint32_t powerGreen = (double)relaxTimeMin/20*100;
          //Serial.print("Power: ");
          //Serial.println(powerGreen);
          if (powerGreen >100) powerGreen = 100;
          fillBarGreebColors((uint8_t)powerGreen);
        }
      }
    } 
  
  if(dataIn==2){   // testing game - GAME 2
    outOfGame = 0;
    drawGame2();
    delay(500);
    dataIn = 0;
    drawLevel();
    
  while(!gameFailed){
    //generate pattern
    randomSeed(millis());
    if (currentLevel==0){
        levelDataP[random(0,15)]=1;
    }//level 0
    else if (currentLevel>=16){
      levelDataP[random(0,1)]=1;
      levelDataP[random(2,4)]=1;
      levelDataP[random(5,7)]=1;
      levelDataP[random(8,9)]=1;
      levelDataP[random(10,12)]=1;
      levelDataP[random(13,15)]=1;
      previewTime = 300;
    }
    else if (currentLevel>=11){
      levelDataP[random(0,2)]=1;
      levelDataP[random(3,5)]=1;
      levelDataP[random(6,8)]=1;
      levelDataP[random(9,12)]=1;
      levelDataP[random(12,15)]=1;
      previewTime = 400;
    }
    else if (currentLevel>=7){
      levelDataP[random(0,3)]=1;
      levelDataP[random(4,7)]=1;
      levelDataP[random(8,11)]=1;
      levelDataP[random(12,15)]=1;
      previewTime = 500;
    }
    else if (currentLevel>=3){
      levelDataP[random(0,5)]=1;
      levelDataP[random(6,11)]=1;
      levelDataP[random(12,15)]=1;
      previewTime = 600;
    }
    else if (currentLevel>=1){
      levelDataP[random(0,7)]=1;
      levelDataP[random(8,15)]=1;
    }
    
    showPatter(); //Shows GAME PATTERN
    delay(previewTime);
    erasePattern();

    //RECORD PATTERN
    for (int i =0; i<16;i++){
      if(levelDataP[i]==1)
        countActiveCells++;
    }
    int key = kp1.getKey();
    for(int k = 0; k<countActiveCells; k++){
      //Serial.println(k);      
        while (!key) {key = kp1.getKey();} // Gaidām kamēr nospiežtaustiņu
        //Serial.print("taustinss "); 
        //Serial.println(key);
        //Serial.print("Dati: ");
        for(uint8_t i = 0; i < 16; i++){ //ieraksta ieslēdzamo segmentu
          if(padLeds[i] == key && key != 0){
              setLeds[i] = key;
          }//end of if
          
          if(correction[i] == key ){ //ja taustiņš sakrīt ar nolasīto no korekcijas matricas (vajag transponēšanai)
            if(checkDataP[i]==1) pressedTwice = 1;
              checkDataP[i]=1;        //pierakstām nospiesto taustiņu
              if(checkDataP[i] != levelDataP[i] || pressedTwice){   //ja piespeiest nepareizais taustiņš
                //Serial.println(" PATTERN NOT mATCH");             
                  for(int j=0; j<16;j++){               //iekrāsojam tos segmentus kuriem bija jābūt
                      if(levelDataP[j] == 1)
                        colorsP[padLeds2[j]-1] = rgb_color(0, 0, 255);
                      else
                        colorsP[padLeds2[j]-1] = rgb_color(0, 0, 0);
                    }
                  colorsP[key-1] = rgb_color(255, 0, 0);  //ierkāsojam nepareizo segmentu
                  ledStripP.write(colorsP, ledCountP, brightness+5);
                  delay(800);
                  erasePattern(); 
                  k=countActiveCells; 
                  gameFailed = 1;                    
                  break;                      //IZLECAM no segmentu ierakstes cikla           
              } else {
                //Serial.println(" PASS");
              }//end of else          
          }//END OF IF correction matrica un pārbaude vai ievadīts pareiz taustiņš          
        }// END OF FOR 16 - ieraksta ieslēdzamo taustiņu
          
        fillPadColors(); // parāda piespiesto taustiņu
        key = kp1.getKey();
        //Serial.print("k = ");
        //Serial.print(k);
        //Serial.println(" TAUSTINS nolasits");
    } // END OF READING the KEYPAD for N times

    delay(300);
    erasePattern();
    
    //Serial.print("END of LEVEL ");
    //Serial.println(currentLevel);
    for(int i =0;i<16;i++) levelDataP[i] =0;
    countActiveCells=0;
    currentLevel++;
    
    delay(500);
    if(!gameFailed)
      drawLevel();
    
  } // while game 2 not failed   
  
    //Serial.println("END of GAME");
    if(currentLevel>5 && allowPlay){
      gamePassed=1;
    }
    if (gamePassed){
        //Serial.println("GAME 2 WON");
        rtc.setSeconds(0);
        rtc.setMinutes(0);
        rtc.setHours(0);}
    currentLevel=0;
    countActiveCells=0;
    gameFailed = 0;
    pressedTwice =0;
    outOfGame = 1;
    
  }//while Game 2 not failed

  
  if(dataIn==1){    //testing game - GAME 1
    outOfGame = 0;
    drawGame1();
    delay(500);
    dataIn = 0;
    //create new LEVEL
    randomSeed(millis());
    for(int i =0; i<60; i++){
      levelData[i][random(0,10)%2]=1;
      //Serial.print(levelData[i][0]);
      //Serial.print(" ");
      //Serial.println(levelData[i][1]);
    }
    drawLevel();
    
    while(!gameFailed){
      for(int i = 0; i<=currentLevel;i++)  //Play pattern
        {
          //Serial.println(i);
          if(levelData[i][0] == 1) {
            onBlue();
            delay(400);
            offBlue();
            delay(200);
            }
          if(levelData[i][1] == 1) {
            onRed();
            delay(400);
            offRed();
            delay(200);}
      }//end of i for loop
  
      //********* RECORD PATTERN *************//
      for (int i = 0; i<=currentLevel; i++){
        
        while(readBlueButton() && readRedButton()){} //gaidām kamēr nospiež kādu no pogām
        //Serial.println("Button pressed");
        if (!readBlueButton()){
          eneteredData[i][0]=1;
          //Serial.println("Blue button pressed");
          blinkBlue();
        }
        else if (!readRedButton()){
          eneteredData[i][1]=1;
          //Serial.println("RED button pressed");
          blinkRed();
        }
        else{
          //Serial.println("NEKAAS NAV");
          falseRead = 1;
        }
        /*Serial.print(currentLevel);
        Serial.print(" ");
        Serial.print(eneteredData[i][0]);
        Serial.print(" ");
        Serial.print(eneteredData[i][1]);
        Serial.print(" <> ");
        Serial.print(levelData[i][0]);
        Serial.print(" ");
        Serial.println(levelData[i][1]);*/
        
        if((eneteredData[i][0]!=levelData[i][0] || eneteredData[i][1]!=levelData[i][1])&&!falseRead){
          /*Serial.print(currentLevel);
          Serial.print(" ");
          Serial.println("NESAKRĪT");*/
          for (int i=0; i<=currentLevel;i++){
            eneteredData[i][0]=0;
            eneteredData[i][1]=0;
          }
          gameFailed = 1;
          break;
        }//END of NESAKRIIT
        else {/*Serial.println("LABI");*/falseRead=0;}
        delay(200);
        
      }//end of RECORD pattern loop
  
      
      for (int i=0; i<=currentLevel;i++){
        eneteredData[i][0]=0;
        eneteredData[i][1]=0;
      }

      if (gameFailed ==0){
       // Serial.println("LEVEL PASSED");
        currentLevel++;
        
        drawLevel();
        delay(300);
        }
      else
        break;
    }//end of GAME 1 FAILED
    
    //Serial.println("END of GAME");
    //Serial.println(currentLevel);
    if(currentLevel > 5 && allowPlay){
      gamePassed=1;
    }
    //Serial.println(gamePassed);
    if (gamePassed){ //RESET the CLOCK
        //Serial.println("GAME 1 WON");
        rtc.setSeconds(0);
        rtc.setMinutes(0);
        rtc.setHours(0);}
    currentLevel=0;
    gameFailed = 0;
    outOfGame = 1;
    offRed();
    offBlue();
    for(int i =0; i<60; i++){
      levelData[i][0]=0;
      levelData[i][1]=0;
    }
  }//end of GAME 1 TEST
  delay(10);
} // END OF LOOP()



////**   FUNCTIONS   **////
//padLeds - glabā ledu adreses
//levelData - glabā parādāmo patternu

/*padLeds[] =             {1, 3, 5, 7,
                           14,12,10,8,
                           15,17,19,21,
                           28,26,24,22};
char key_chars[4][4] = {            // The symbols of the keys
  {1, 14, 15, 28},
  {3, 12, 17, 26},
  {5, 10, 19, 24},
  {7, 8, 21, 22}
};       
levelDataP[16] = {         0,1,1,0,
                           0,0,0,0,
                           1,0,0,1,
                           0,0,0,1};                    
                           */
void showPatter(){
  for(int i=0; i<16;i++){
    if(levelDataP[i] == 1)
      colorsP[padLeds2[i]-1] = rgb_color(0, 0, 255);
    else
      colorsP[padLeds2[i]-1] = rgb_color(0, 0, 0);
  }
  
  ledStripP.write(colorsP, ledCountP, brightness+5);
}

void erasePattern(){
  for(int i=0; i<28;i++){
    colorsP[i] = rgb_color(0, 0, 0);
  }

  for(int i =0; i<16; i++){
    setLeds[i]=0;
    checkDataP[i]=0;
  }
  ledStripP.write(colorsP, ledCountP, brightness+5);
}

void fillPadColors(void){
for(uint8_t i = 0; i < 16; i++)
  {
    if(padLeds[i] == setLeds[i])
      colorsP[padLeds[i]-1] = rgb_color(0, 0, 255); //R G B
    else
      colorsP[padLeds[i]-1] = rgb_color(0, 0, 0);
  }
  ledStripP.write(colorsP, ledCountP, brightness+5);
}

void fillBarColors(uint8_t procenti){
  //5-33 = 28 diodes
  
  for(uint8_t i = 4; i < ledCountB; i++)
  {
    if (i-4<28*procenti/100)
      colorsB[i] = rgb_color(255, 0, 0); //R G B
    else
      colorsB[i] = rgb_color(0, 0, 0); //R G B
  }

  ledStripB.write(colorsB, ledCountB, brightness+2);
}

void fillBarGreebColors(uint8_t procenti){
  //5-33 = 28 diodes
  
  for(uint8_t i = 4; i < ledCountB; i++)
  {
    if (i-4<28*procenti/100)
      colorsB[i] = rgb_color(0, 255, 0); //R G B
    else
      colorsB[i] = rgb_color(0, 0, 0); //R G B
  }

  ledStripB.write(colorsB, ledCountB, brightness+2);
}

void fillBarWhiteColors(uint8_t procenti){
  //5-33 = 28 diodes
  
  for(uint8_t i = 4; i < ledCountB; i++)
  {
    if (i-4<28*procenti/100)
      colorsB[i] = rgb_color(255, 255, 255); //R G B
    else
      colorsB[i] = rgb_color(0, 0, 0); //R G B
  }

  ledStripB.write(colorsB, ledCountB, brightness+2);
}

void closeServos()
{
  myservo0.write(50);
  myservo1.write(150);
}

void openServos()
{
  myservo0.write(150);
  myservo1.write(50);
}

void procesKeypad2(void)
{
  int key = kp1.getKey();
  while (!key) {
    //Serial.print("taustinss "); 
    //Serial.println(key);
    for(uint8_t i = 0; i < 16; i++)
      {
        if(padLeds[i] == key && key != 0)
        {
          setLeds[i] = key;
        }
      }//end of for
  }
}

int procesKeypad(void)
{
  int key = kp1.getKey();
  if (key) {
    //Serial.print("taustinss "); 
    //Serial.println(key);
    for(uint8_t i = 0; i < 16; i++)
      {
        if(padLeds[i] == key && key != 0)
        {
          setLeds[i] = key;
        }
      }//end of for
  }
  return key;
}

void drawGame2(){
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(20, 24);     // Start at top-left corner
  display.write("PATTERN");
  display.display();
  }

void drawGame1(){
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(20, 24);     // Start at top-left corner
  display.write("SEQUENCE");
  display.display();
  }  
  
void drawLevel(){
  char tmp[] = "00";
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(37, 4);     // Start at top-left corner  
  display.write("LEVEL");
  display.setTextSize(3);
  display.setCursor(52, 31);
  sprintf(tmp,"%02d",currentLevel+1);
  display.write(tmp[0]);
  display.write(tmp[1]);
  display.display();
  }

void drawText(){
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(18, 22);     // Start at top-left corner
  sprintf(laiks, "%02d:%02d:%02d",rtc.getHours(),rtc.getMinutes(),rtc.getSeconds());
  for (char i =0; i<8;i++)
    {display.write(laiks[i]);}
  display.display();
  }

void drawRemainingTime(){
  //startHours
  char laiks2[]="00:00:00";
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(18, 22);     // Start at top-left corner
  sprintf(laiks2, "%02d:%02d:%02d",(rtc.getHours()),((rtc.getMinutes())),((rtc.getSeconds())));
  for (char i =0; i<8;i++)
    {display.write(laiks2[i]);}
  display.display();
  }

void onRed (void){
  digitalWrite(9,HIGH);
}

void onBlue (void){
  digitalWrite(10,HIGH);
}

void offRed (void){
  digitalWrite(9,LOW);
}

void offBlue (void){
  digitalWrite(10,LOW);
}

uint16_t readMagnet(void){
  return analogRead(A0);
}

void soundOn(uint16_t freq){
  tone(3,freq);
}

uint8_t readMegnet(void){
  uint16_t data = analogRead(A0);
  if (data >1000) return 1;
  else return 0;
}

uint8_t readBlueButton(void){
  return digitalRead(8);
}

uint8_t readRedButton(void){
  return digitalRead(7);
}

void blinkRed(){
  onRed();
  delay(300);
  offRed();
}

void blinkBlue(){
  onBlue();
  delay(300);
  offBlue();
}
