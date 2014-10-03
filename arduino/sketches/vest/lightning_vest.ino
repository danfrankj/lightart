// Total Control Lighting vest
// Written by Dan Frank
// 2014-09-01

#include <SPI.h>
#include <TCL.h>

const int LEDS = 25;
const byte DOTS = 3;
const byte DELAY = 50;
byte allcolors[LEDS][3];
float currentcolor[3] = {255/10, 128/10, 20};
float previouscolor[3];
float nextcolor[3];
byte color_incr[3];
boolean dimming = false;
int counter = 0;
const byte NMANUALS = 6;
byte mcolors[NMANUALS][3]; 


void setup(){
  TCL.begin();
  // setup manual colors
  mcolors[0][0] = 10; mcolors[0][1] = 1; mcolors[0][2] = 1;
  mcolors[1][0] = 1; mcolors[1][1] = 10; mcolors[1][2] = 1;
  mcolors[2][0] = 1; mcolors[2][1] = 1; mcolors[2][2] = 10;
  mcolors[3][0] = 10; mcolors[3][1] = 10; mcolors[3][2] = 1;
  mcolors[4][0] = 1; mcolors[4][1] = 10; mcolors[4][2] = 10;
  mcolors[5][0] = 10; mcolors[2][1] = 1; mcolors[2][2] = 10; 
  
  Serial.begin(9600);
  TCL.sendEmptyFrame();
}

void loop(){
  bumpcolors()
  changecolor();
  setcolor(0);
  sendallcolors();
  counter += 1;
  delay(DELAY);
}

void setcolor(int index) {
  allcolors[index][0] = currentcolor[0]
  allcolors[index][1] = currentcolor[1]
  allcolors[index][2] = currentcolor[2]
}

void changeColor() {
    byte ind = random(0, NMANUALS - 1);
    currentcolor[0] = mcolors[ind][0];
    currentcolor[1] = mcolors[ind][1];
    currentcolor[2] = mcolors[ind][2];
}

void printColor() {
  Serial.print('R');
  Serial.print(currentcolor[0]);
  Serial.print(' ');
  Serial.print('G');
  Serial.print(currentcolor[1]);
  Serial.print(' ');
  Serial.print('B');
  Serial.print(currentcolor[2]);
  Serial.print('\n');
}

void sendOutput(){
  int i;
  for(i=0;i<LEDS;i++){
    TCL.sendColor(allcolors[i][0], allcolors[i][1], allcolors[i][2]);
  }
  TCL.sendEmptyFrame();
}


