#include "SPI.h"
#include "IconHUST.c"
#include "BBLAB.c"
#include "iconcheckV.c"
#include "iconcheckX.c"
#include <Adafruit_Fingerprint.h>
#include <Adafruit_Keypad.h>
#include <Adafruit_Keypad_Ringbuffer.h>
#include <TFT_eSPI.h>
#include <Keypad_I2C.h>
#include <I2CKeyPad.h>
#include <Keypad.h>
#include <Wire.h>
#include <WiFi.h>
#include "control_moto.h"
#include "save_password.h"

#define GFXFF 1
#define Custom 1
#define FM12 &FreeMonoBoldOblique12pt7b
#define FF12 &FreeSans12pt7b
#define FMB18 &FreeMonoBold18pt7b
#define FM24 &FreeMonoBoldOblique24pt7b
#define FSB24 &FreeSerifBold24pt7b
#define FSB18 &FreeSerifBold18pt7b
#define YEL32 &Yellowtail_32
#define FSB12 &FreeSerifBold12pt7b

#define mySerial Serial2
#define MODEM_RX 16
#define MODEM_TX 17
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

TFT_eSPI tft = TFT_eSPI();

//#define IN1 12
//#define IN2 13

//const int  switch_journey = 12;
//const int button_CloseDoor = 14;
//
//int switch_journeyCounter = 0;   // số lần button được nhấn
//int switch_journeyState = 0;         // trạng thái hiện tại của button
//int lastswitch_journeyState = 0;     // trạng thái trước đó của button
//int button_CloseDoorState = 0;

unsigned long timerDelay = 0;
unsigned long timerDelayVoid = 0;

int numberCount = 0;
String stringCharacter = "";
bool pressNumber;
char keyRead;
int numberKeypad;

//byte motor_status = 0; // 0 Stop , 1 Open 2s, 2 Stop 10s, 3 Close 2s,
//byte  monitor_status = 0;// 0 Turn off , 1 Turn on Monitor
//volatile unsigned long timer_check_motor = 0;
//volatile unsigned long timer_check_monitor = 0;
//#define TIME_OPEN_DOOR 10000
//#define TIME_OPEN_MOTOR 2000
//#define TIME_CLOSE_MOTOR 2000


long KeypadPasswordDefault = 88888888;
long setKeypadPassword;
unsigned int addressPassword = 0;

long setNewKeypadPassword;
long checkChangePassword;
long setPasswordAdmin =  99999999;
#define I2C_Addr 0x20                             // I2C Address of PCF8574-board: 0x20 - 0x27
const byte NbrRows = 4;                           // Number of Rows
const byte NbrColumns = 4;                        // Number of Columns
char KeyPadLayout[NbrRows][NbrColumns] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte PinsLines[NbrRows] = {4,5,6,7};          //  ROWS Pins { 3, 2, 1, 0}
byte PinsColumns[NbrColumns] = {0,1,2,3};      //  COLUMNS Pins {7, 6, 5, 4}
Keypad_I2C i2cKeypad( makeKeymap(KeyPadLayout), PinsLines, PinsColumns, NbrRows, NbrColumns, I2C_Addr);

void timeDelay(int timer){
  timerDelay = millis();
  int i = 1;
  while(i){
    if(millis()-timerDelay > timer){
      i = 0;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  while (!Serial);
  Wire.begin(21,22);                                         // Init I2C-Bus, GPIO4-Data, GPIO5 Clock
  Wire.beginTransmission(I2C_Addr);                        // Try to establish connection
  Serial.println(F("Device found on"));
  i2cKeypad.begin();                                     // Start i2cKeypad

  tft.init();
  tft.begin();
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setRotation(3);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(FF12);
  drawScreen();
  homeScreen();
  if(setKeypadPassword == 0){
  Serial.println("Init Password Default:");
  Serial.println(setKeypadPassword);
  setKeypadPassword = KeypadPasswordDefault;
  Serial.println(setKeypadPassword);
  }
  setKeypadPassword = getPassword();
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);
  if (finger.verifyPassword()) {
  Serial.println("Found fingerprint sensor!");
  }
  else {
    checkX();
    tft.drawString("Didn't find fingerprint sensor",8,180,1);
    while (1);
  }
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    checkX();
    tft.drawString("Sensor doesn't contain any",15,160,1);
    tft.drawString("fingerprint data. Please run",8,180,1);
    tft.drawString("the 'enroll' example.",35,200,1);  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
  motor_status = 0;
  monitor_status = 0;
}
bool exitMode = false;
uint16_t id;
uint16_t IDAddFinger;
uint16_t IDDeleteFinger;
uint8_t checkFinger = 1;
int ackKeypad = 0;

void loop(){
checkMoto();                                          //Kiem tra trang thai cua moto
getFingerprintID();                                   //Quet van tay
Serial.println(setKeypadPassword);
if(!checkFinger){                                     //checkFinger == true: co vantay dang dat vao, checkFinger == false: khong co vantay
 checkNum(40,130,245,30,ackKeypad,8,0);               //hien thi so nhap vao tren ban phim
//checkNum{locationX, locationY, WidthRect,HeightRect, ackPress,digitnumber, hideNumber}
//locationX:    toa do x cua hinh chu nhat hien thi so
//locationY:    toa do y cua hinh chu nhat hien thi so
//WidthRect:    chieu rong cua hinh chu nhat
//HeightRect:   chieu cao cua hinh chu nhat
//ackPress:     check co nhan vao ban phim hay khong
//digitnumber:  so chu so hien thi len tren man hinh
//hideNumber:   0: hien thi so, 1: hien thi ky tu " * ";
 if(ackKeypad){                                       //ackKeypad:  check xem co nhap vao ban phim hay khong
       if(numberKeypad == setKeypadPassword){
        Serial.println("success");
        checkV();
        // Moto(); // Chay motor
        motor_status = 1;
        timeDelay(1000);
        drawScreen();
        homeScreen();
        numberKeypad = 0;
        return;
        }
       if((numberKeypad != setKeypadPassword) && (numberKeypad != 17)&&(numberKeypad != 18)){
        // 17: phim A - Setting button
        // 18: phim B - Back button
        Serial.println("failue");
        checkX();
        tft.drawString("FAILED",110,180,1);
        timeDelay(1000);
        drawScreen();
        homeScreen();
        numberKeypad = 0;
         return;
        }
       if(numberKeypad == 18){
        drawScreen();
        homeScreen();
        numberKeypad = 0;
         return;
       }
       if(numberKeypad == 17){
                  unsigned long timerVoid = millis();
                  Serial.println("Log in Admin Setting");
                  bool verifyPass = false;                      //verifyPass : true: vao che do setting thanh cong, false: nguoc lai
                  numberKeypad = 0;
                  loginSettingScreen();
                  //*****************************************************************************//
                  //CHECK PASS LOGIN SETTING MODE
                  //*****************************************************************************//
                  while(!verifyPass){
                    Serial.println("Verify Pass");
                    checkNum(40,148,245,30,ackKeypad,8,0);
                  //Sau 10s khong nhap vao ban phim, auto thoat ve homescreen
                    if(!ackKeypad){
                      if( millis() - timerVoid > 10000 ){
                        Serial.println("exit verify");
                        verifyPass = true;
                        numberKeypad = 0;
                        drawScreen();
                        homeScreen();
                        return;
                         }
                    }
                      while(numberKeypad){
                        timerVoid = millis();
                        Serial.println("Check number");
                          if(numberKeypad == 18){
                              Serial.println("exit verify readnumber");
                              verifyPass = true;
                              numberKeypad = 0;
                              drawScreen();
                              homeScreen();
                              return;
                              }
                          else if(numberKeypad == 17){
                              numberKeypad = 0;
                              loginSettingScreen();
                              }
                          else if((numberKeypad != setPasswordAdmin) && numberKeypad != 18 && numberKeypad != 17){
                              checkX();
                              tft.drawString("Wrong password",70,180,1);
                              timeDelay(1000);
                              numberKeypad = 0;
                              loginSettingScreen();
                              }
                          else{
                              timerVoid = millis();
                              verifyPass = true;
                              numberKeypad = 0;
                              Serial.println("successfully logged in mode");
                              adminScreen();
                              }
                        }
                  }
                  //*****************************************************************************//
                  //                               SETTING MODE
                  //*****************************************************************************//
                  while(verifyPass){
                    while(!numberKeypad){
                      checkNum(140,101,245,30,ackKeypad,1,1);
                      //sau 20s khong nhap gi vao ban phim, auto thoat SETTING MODE
                      if(!ackKeypad){
                        if( millis() - timerVoid > 20000 ){
                          Serial.println("exit admin mode");
                          verifyPass = false;
                          numberKeypad = 0;
                          drawScreen();
                          homeScreen();
                          return;
                          }
                      }
                    }
                    uint8_t chooseMode = 0;
                    chooseMode = numberKeypad;
                    Serial.println("chooseMode");
                    Serial.print(chooseMode);
                    if(chooseMode <= 0 || (chooseMode >= 4 && chooseMode <= 16) ||chooseMode >= 19){
                            Serial.print (chooseMode);
                            checkX();
                            tft.drawString(" Please choose number ",40,160,1);
                            tft.drawString("from 1 to 4",95,185,1);
                            Serial.println(" Please choose number from 1 to 4");
                            timeDelay(2000);
                            adminScreen();
                            numberKeypad = 0;
                        }
                    else if(chooseMode == 17){
                            adminScreen();
                            numberKeypad = 0;
                    }
                    else if(chooseMode == 18) {
                      Serial.println("Exit Admin Setting");
                      drawScreen();
                      homeScreen();
                      return;
                      }

                 else{
//***************************** CHANGE PASSWORD************************************************//
                      if(chooseMode == 1){

                          Serial.println("Change Password Screen");
                          Serial.println(setKeypadPassword);
                          while(!exitMode){
                              changePasswordScreen();
                              timerVoid = millis();
                              numberKeypad = 0;
                              Serial.println("Please insert old setKeypadPassword:");
                              while(!numberKeypad){
                                  checkNum(40,108,245,30,ackKeypad,8,1);
                                  if(!ackKeypad){
                                      if( millis() - timerVoid > 20000 ){
                                        Serial.println("exit choose mode 1");
                                        numberKeypad = 0;
                                        homeScreen();
                                        exitMode = true;
                                        return;
                                        }
                                  }
                              }

                              checkChangePassword = numberKeypad;
                              Serial.println(checkChangePassword);

                              Serial.println("compare checkChangePassword");

                              if(checkChangePassword == 18) {
                                  Serial.println("Exit setting change password");
                                  numberKeypad = 0;
                                  exitMode = true;
                                  verifyPass= true;
                                  adminScreen();
                              break;
                              }
                              else if(checkChangePassword == 17) {
                                  exitMode = false;
                              }
                              else if(checkChangePassword != setKeypadPassword) {
                              checkX();
                              tft.drawString("Wrong password",90,180,1);
                              Serial.println("Wrong password");
                              timeDelay(1000);
                              exitMode = false;
                              }
                              else{
                                tft.drawString("New Password:",10,158,1);
                                tft.fillRect(40,185,245,40,TFT_LIGHTGREY);
                                tft.drawRect(39,184,247,42,TFT_BLACK);
                              while(!exitMode){
                                  timerVoid = millis();
                                  numberKeypad = 0;
                                  Serial.println("Insert setNewKeypadPassword:");

                                  while(!numberKeypad){
                                    checkNum(40,185,245,30,ackKeypad,8,1);
                                    if(!ackKeypad){
                                        if( millis() - timerVoid > 20000 ){
                                        Serial.println("exit insert setNewKeypadPassword");
                                        numberKeypad = 0;
                                        homeScreen();
                                        exitMode = true;
                                        return;
                                        }
                                    }
                                  if(numberKeypad == 17){
                                  numberKeypad = 0;
                                  }
                                  else if(numberKeypad == 18) {
                                    Serial.println("Exit setting change password");
                                    //numberKeypad = 0;
                                    exitMode = true;
                                    verifyPass= true;
                                    adminScreen();
                                             }
                                  }
                                  if(numberKeypad !=  18){
                                      setNewKeypadPassword = numberKeypad;
                                      setKeypadPassword = setNewKeypadPassword;
                                      Serial.println(setKeypadPassword);
                                      if (savePassword(setKeypadPassword)){
                                        checkV();
                                        tft.drawString("Change Password Successful!",10,180,1);
                                        timeDelay(1000);
                                        numberKeypad = 0;
                                        exitMode = true;
                                        chooseMode=0;
                                        Serial.println(chooseMode);
                                        Serial.println(verifyPass);
                                      }  
                                  }

                          }
                          }
                          exitMode = false;
                          }
                          }
                          exitMode = false;
//******************************ADD FINGERPRINT ENROLL************************************************//
                      if(chooseMode == 2){
                        Serial.println("Add Fingerprint Screen");
                        while(!exitMode){
                          addFingerScreen();
                          timerVoid = millis();
                          numberKeypad = 0;
                          Serial.println("Please choose id from 1 to 230");

                          while(!numberKeypad){
                             checkNum(40,160,235,30,ackKeypad,8,1);

                             if(!ackKeypad){
                                  Serial.println(timerDelay);
                                  Serial.print(millis());
                                  if( millis() - timerVoid > 20000 ){
                                  Serial.println("exit choose mode 2");
                                  numberKeypad = 0;
                                  homeScreen();
                                  exitMode = true;
                                  return;
                                  }
                              }
                          }

                          IDAddFinger = numberKeypad;
                          Serial.println(IDAddFinger);

                          if(IDAddFinger == 18) {
                          Serial.println("Exit setting change password");
                          numberKeypad = 0;
                          exitMode = true;
                          verifyPass= true;
                          adminScreen();
                          break;
                          }
                          else if(IDAddFinger == 17) {
                          exitMode = false;
                          }
                          else if(IDAddFinger <= 0 ||IDAddFinger == 17 || IDAddFinger == 18|| IDAddFinger >= 231) {
                              checkX();
                              tft.drawString("ID not allowed",90,180,1);
                              Serial.println(" ID not allowed");
                              timeDelay(1000);
                              exitMode = false;
                          }
                          else{
                            Serial.println("Add Fingerprint Enroll as ID:");
                            //tft.drawFloat(IDAddFinger,0,140,170,1);
                            tft.setFreeFont(FSB12);
                            Serial.println(IDAddFinger);
                            tft.setFreeFont(FF12);
                            timeDelay(1000);
                            getFingerprintEnroll();
                            checkV();
                            tft.drawString("Add Fingerprint Successful!",15,180,1);
                            timeDelay(1000);
                            exitMode = false;
                          }
                        }
                 }
                 exitMode = false;
//******************************DELETE FINGERPRINT ENROLL************************************************//
                 if(chooseMode == 3){
                  Serial.println("Delete Finger Screen");
                  while(!exitMode){
                          deleteFingerScreen();
                          timerVoid = millis();
                          numberKeypad = 0;
                          tft.drawString("Choose ""0"" to delete all ",50,180,1);
                          tft.drawString("fingerprint templates!",10,210,1);
                          Serial.println("Delete Fingerprint Screen");
                          Serial.println("Please choose id from 1 to 230");
                          while(!numberKeypad){
                             checkNum(40,130,245,30,ackKeypad,8,0);
                             if(!ackKeypad){
                                  Serial.println(timerDelay);
                                  Serial.print(millis());
                                  if( millis() - timerVoid > 20000 ){
                                  Serial.println("exit choose mode 3");
                                  numberKeypad = 0;
                                  homeScreen();
                                  exitMode = true;
                                  return;
                                  }
                              }
                          }
                          IDDeleteFinger = numberKeypad;
                          Serial.println(IDDeleteFinger);

                          if(IDDeleteFinger == 18) {
                          Serial.println("Exit setting change password");
                          numberKeypad = 0;
                          exitMode = true;
                          verifyPass= true;
                          adminScreen();
                          break;
                          }
                          else if(IDDeleteFinger == 17) {
                          exitMode = false;
                          }
                          else if(IDDeleteFinger < 0 ||IDDeleteFinger == 17 || IDDeleteFinger == 18|| IDDeleteFinger >= 231) {
                              checkX();
                              tft.drawString("ID not allowed",90,180,1);
                              Serial.println(" ID not allowed");
                              timeDelay(1000);
                              exitMode = false;
                          }
                          else{
                            Serial.println("Delete ID:");
                            tft.drawFloat(IDDeleteFinger,0,140,138,1);

                            tft.setFreeFont(FSB12);
                            Serial.println(IDDeleteFinger);
                            tft.setFreeFont(FF12);
                                   if(IDDeleteFinger == 0){
                                         drawScreen();
                                         Serial.println("Now database is empty :)");
                                         finger.emptyDatabase();
                                         tft.setFreeFont(FF12);
                                         tft.drawString("Deleting all fingerprint templates",0,140,1);
                                         timeDelay(1000);
                                         int rowdelall = 10;
                                         for(int delall = 1; delall<=10; delall++){
                                            tft.setFreeFont(FSB12);
                                            tft.drawString(".",rowdelall,190,1);
                                            rowdelall = rowdelall+25;
                                            timeDelay(100);
                                         }
                                    }
                                    else {
                                        drawScreen();
                                        Serial.print("Deleting ID: ");
                                        Serial.println(IDDeleteFinger);
                                        deleteFingerprint(IDDeleteFinger);
                                        tft.setFreeFont(FF12);
                                        tft.drawString("Deleting ID:",60,140,1);
                                        tft.setFreeFont(FSB12);
                                        tft.drawFloat(IDDeleteFinger,0,185,140,1);
                                        timeDelay(1000);
                                        int rowdel = 10;
                                        for(int del = 1; del<=10; del++){
                                            tft.setFreeFont(FSB12);
                                            tft.drawString(".",rowdel,190,1);
                                            rowdel = rowdel+25;
                                            timeDelay(100);
                                        }
                                        }

                                        checkV();
                                        tft.drawString("DELETED!",100,180,1);
                                        timeDelay(1500);
                                        exitMode = false;


                  }
                  }
                  }
                  exitMode = false;
}
}
}
}
}
}
//---------------------------------------------------------------------------------------
//                                 TFT Screen
//---------------------------------------------------------------------------------------
void drawScreen(){
  tft.fillScreen(TFT_WHITE );
  tft.fillRect(0,64,320,5,TFT_DARKGREY );
  tft.setSwapBytes(true);
  tft.pushImage(0,0,48,64,IconHUST);
  tft.pushImage(274,0,46,64,BBLAB);
}
void loginSettingScreen(){
  drawScreen();
  tft.setFreeFont(FSB12);
  tft.drawString("ADMIN SETTING",65,25,1);
  tft.setFreeFont(FF12);
  tft.drawString("Enter password to change",30,80,1);
  tft.drawString("setting:",10,105,1);
  tft.drawRect(39,147,247,42,TFT_BLACK);
  tft.fillRect(40,148,245,40,TFT_LIGHTGREY);
}
void homeScreen(){
 drawScreen();
tft.fillRect(40,130,245,40,TFT_LIGHTGREY);
tft.drawRect(39,129,247,42,TFT_BLACK);
tft.setFreeFont(FSB12);
tft.drawString("SMART LOCK",83,25,1);
tft.setFreeFont(FF12);
 Serial.println("You in Home Screen");
}
void adminScreen(){
Serial.println("Admin setting Screen");
drawScreen();
tft.setFreeFont(FSB12);
tft.drawString("ADMIN SETTING",65,25,1);
tft.drawString("PLEASE CHOOSE MODE",15,73,1);
tft.setFreeFont(FF12);
tft.drawRect(39,100,247,42,TFT_BLACK);
tft.fillRect(40,101,245,40,TFT_LIGHTGREY);
tft.drawString("Mode 1: Change Password",8,148,1);
tft.drawString("Mode 2: Add Finger Enroll",8,178,1);
tft.drawString("Mode 3: Delete Finger Enroll",8,208,1);
}
void changePasswordScreen(){
drawScreen();
tft.setFreeFont(FSB12);
tft.drawString("CHANGE",60 ,10,1);
tft.drawString("PASSWORD",115 ,35,1);
tft.setFreeFont(FF12);
tft.drawString("Old Password:",10,80,1);
tft.fillRect(40,108,245,40,TFT_LIGHTGREY);
tft.drawRect(39,107,247,42,TFT_BLACK);
}
void addFingerScreen(){
drawScreen();
tft.setFreeFont(FSB12);
tft.drawString("ADD FINGERPRINT",48,20,1);
tft.fillRect(40,160,235,40,TFT_LIGHTGREY);
tft.drawRect(39,159,237,42,TFT_BLACK);
tft.drawString("ID:",3,170,1);
tft.setFreeFont(FF12);
tft.drawString("Ready to enroll a fingerprint!",10,75,1);
tft.drawString("Save this finger as ID (from 1",10,103,1);
tft.drawString("to 230):",10,128,1);
}
void deleteFingerScreen(){
drawScreen();
tft.setFreeFont(FSB12);
tft.drawString("DELETE",60 ,10,1);
tft.drawString("FINGERPRINT",85 ,35,1);
tft.fillRect(40,130,245,40,TFT_LIGHTGREY);
tft.drawRect(39,129,247,42,TFT_BLACK);
tft.setFreeFont(FF12);
tft.drawString("Delete Fingerprint ID :",10,75,1);
tft.drawString("(Number ID from 1 to 230)",10,105,1);
}
void checkV(){
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE );
  tft.pushImage(110,50,100,100,iconcheckV);
  tft.setFreeFont(FSB12);
  //timeDelay(1000);
  }
void checkX(){
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE );
  tft.pushImage(110,50,100,100,iconcheckX);
  tft.setFreeFont(FSB12);
  // tft.drawString("FAILED",110,180,1);
 // timeDelay(1000);
  }
//---------------------------------------------------------------------------------------
//                                 FINGER ENROLL
//---------------------------------------------------------------------------------------
uint8_t getFingerprintEnroll() {
  int q = -1;
  tft.fillScreen(TFT_WHITE);
  tft.drawString("Enrolling ID",10,10,1);
  Serial.print("Looking finger enroll as ID:"); Serial.println(IDAddFinger);
  tft.drawString("Looking fingerenroll as ID: ",10,35,1);
  tft.drawFloat(IDAddFinger,0,285,35,1);

  while (q != FINGERPRINT_OK) {
q = finger.getImage();
    switch (q) {
    case FINGERPRINT_OK:{
      Serial.println("Image taken");
      tft.drawString("Image taken",10,85,1);
      break;}
    case FINGERPRINT_NOFINGER:{
      Serial.print(".");
      int rowdel3 = 10;
         for(int del3 = 1; del3<=15; del3++){
         tft.drawString(".",rowdel3,60,1);
         rowdel3 = rowdel3+20;
         timeDelay(100);
         }
      break;}
    case FINGERPRINT_PACKETRECIEVEERR:{
      Serial.println("Communication error");
      checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;}
    case FINGERPRINT_IMAGEFAIL:{
      Serial.println("Imaging error");
      checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;}
    default:
      Serial.println("Unknown error");
      checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;
    }
  }
  // OK success!
q = finger.image2Tz(1);
  switch (q) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      tft.drawString("Image converted",10,110,1);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return q;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    default:
      Serial.println("Unknown error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
  }
  Serial.println("Remove finger");
  tft.drawString("Remove finger",10,135,1);
  timeDelay(2000);
  q = 0;
  while (q != FINGERPRINT_NOFINGER) {
q = finger.getImage();
  }
  Serial.print("ID "); Serial.println(IDAddFinger);
  tft.drawString("ID: ",10,160,1);tft.drawFloat(IDAddFinger,0,45,160,1);
  q = -1;
  Serial.println("Place same finger again");
  tft.drawString("Place same finger again!",10,185,1);
  while (q != FINGERPRINT_OK) {
q = finger.getImage();
    switch (q) {
    case FINGERPRINT_NOFINGER:{
      Serial.print(".");
       int rowdel4 = 10;
         for(int del4 = 1; del4<=15; del4++){
         tft.drawString(".",rowdel4,205,1);
         rowdel4 = rowdel4+20;
         timeDelay(100);
         }
      break;}
          case FINGERPRINT_OK:
      Serial.println("Image taken");
      tft.fillScreen(TFT_WHITE);
      tft.drawString("Image taken",10,10,1);
      timeDelay(300);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;
    default:
      Serial.println("Unknown error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      break;
    }
  }
  // OK success!
q = finger.image2Tz(2);
  switch (q) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
       tft.drawString("Image converted",10,40,1);
       timeDelay(300);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
    default:
      Serial.println("Unknown error");
       checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
      return q;
  }
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(IDAddFinger);
  tft.drawString("Creating model for ID: ",10,70,1);
  tft.drawFloat(IDAddFinger,0,240,70,1);
       timeDelay(300);
q = finger.createModel();
  if (q == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    tft.drawString("Prints matched!",10,100,1);
    timeDelay(300);
  } else if (q == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
    return q;
  } else if (q == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
     tft.drawString("Fingerprints did not match!",10,100,1);
    timeDelay(300);
    return q;
  } else {
    Serial.println("Unknown error");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
    return q;
  }
  Serial.print("ID "); Serial.println(IDAddFinger);
q = finger.storeModel(IDAddFinger);
  if (q == FINGERPRINT_OK) {
    Serial.println("Stored!");
     tft.drawString("Stored!",10,130,1);
    timeDelay(300);
  } else if (q == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
    return q;
  } else if (q == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
    return q;
  } else if (q == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(300);
    return q;
  } else {
    Serial.println("Unknown error");
     checkX();
      tft.drawString("ERROR",110,180,1);
//      timeDelay(300);
    return q;
  }
  return true;
}

uint8_t getFingerprintID() {
  uint8_t q = finger.getImage();
  switch (q) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      checkFinger = 0;
      return q;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return q;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return q;
    default:
      Serial.println("Unknown error");
      return q;
  }
  // OK success!
q = finger.image2Tz();
  switch (q) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return q;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return q;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return q;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return q;
    default:
      Serial.println("Unknown error");
      return q;
  }
  // OK converted!
q = finger.fingerFastSearch();
  if (q == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (q == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return q;
  } else if (q == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    checkX();
    tft.drawString("Finger ID not Found",60,180,1);
    timeDelay(500);
    drawScreen();
    homeScreen();
    return q;
  } else {
    Serial.println("Unknown error");
    return q;
  }
  // found a match!
  Serial.print("Found ID: "); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  checkV();
  motor_status = 1; // Bat dau cho chay motor
  tft.drawString("Found ID :",80,180,1);
  tft.drawFloat(finger.fingerID,0,200,180,1);
  timeDelay(500);
  drawScreen();
  homeScreen();
  return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(1000);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(1000);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(1000);
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
     checkX();
      tft.drawString("ERROR",110,180,1);
      timeDelay(1000);
  }
  return p;
}
//------------------------------------------------------------------------------------
//                                 KEYPAD
//------------------------------------------------------------------------------------
int checkNum(uint16_t locationXRect, uint16_t locationYRect,uint16_t widthRect,uint16_t heightRect,int &ackPress,uint8_t digitNumber ,uint8_t hideNumber){
  uint16_t locationXChar = locationXRect+8;
do {
     keyRead = i2cKeypad.getKey();
       switch(keyRead){
          case '0':
            pressNumber = true;
            break;
        case '1':
            pressNumber = true;
            break;
        case '2':
            pressNumber = true;
            break;
        case '3':
            pressNumber = true;
            break;
        case '4':
            pressNumber = true;
            break;
        case '5':
            pressNumber = true;
            break;
        case '6':
            pressNumber = true;
            break;
        case '7':
            pressNumber = true;
            break;
        case '8':
            pressNumber = true;
            break;
        case '9':
            pressNumber = true;
            break;
        case 'A':
            pressNumber = false;
            ackPress = 1;
            return numberKeypad = 17;

        case 'B':
            pressNumber = false;
            return numberKeypad = 18;
        case 'C':
            tft.fillRect(locationXRect,locationYRect,widthRect,heightRect,TFT_LIGHTGREY);
            locationXChar = locationXRect+8;
            stringCharacter = "";
            numberCount = 0;
            pressNumber = false;
            break;
        case 'D':
            numberKeypad = stringCharacter.toInt();
            Serial.print("Day so vua nhap la:");
            Serial.println(stringCharacter);
            stringCharacter="";
            numberCount = 0;
            pressNumber = false;
            return numberKeypad;
        case '*':
            pressNumber = false;
            break;
        case '#':
            pressNumber = false;
            break;
     }
      if(numberCount == 0){
       timerDelayVoid = millis();
        ackPress = 0;
       }

       if(millis()-timerDelayVoid > 10000){
                     tft.fillRect(locationXRect,locationYRect,widthRect,heightRect,TFT_LIGHTGREY);
                     locationXChar = locationXRect + 8;
                     stringCharacter = "";
                     numberCount = 0;
                     pressNumber = false;
                     //return numberKeypad = 0;
                  }
      if (keyRead && (numberCount < digitNumber)){

           if(pressNumber){
                  timerDelay = millis();

                  stringCharacter = stringCharacter + keyRead;
                  tft.drawChar(locationXChar,locationYRect+27,keyRead,TFT_BLACK,TFT_LIGHTGREY,1);
                     int i = 1;
                     while(i){
                      if(millis()-timerDelay > 100){
                        timerDelay = millis();
                        if(hideNumber == 0){
                          tft.fillRect(locationXChar,locationYRect,widthRect-220,heightRect,TFT_LIGHTGREY);
                          tft.drawChar(locationXChar-3,locationYRect+57,'*',TFT_DARKGREY,TFT_WHITE,3);
                        }
                        locationXChar = locationXChar + 30;
                        numberCount++;
                        if(numberCount == 1){
                          ackPress = 1;
                        }
                        pressNumber = false;
                        i = 0;
                               }
                       }
                }
        }
}while(numberCount);
}
