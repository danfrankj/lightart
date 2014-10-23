#include <SPI.h>
#include <TCL.h>

const int LEDS = 18;
const int ALLLEDS = 18;
const byte DELAY = 50;
const float MAXBRIGHT = 50; 
byte allcolors[LEDS][3];
float currentcolor[3] = {0, 0, 0};
float startcolor[3] = {0, 0, 0};
float nextcolor[3] = {0, 0, 0};
float mixfactor = 0.0;
float brightnessfactor = 0.0;
boolean dimming = false;
boolean fwdmix = true;
int counter = 0;
const byte NMANUALS = 9;
float mcolors[NMANUALS][3];


void setup(){
  TCL.begin();
  // setup manual colors
  // color = 2 ^ intensity - 1
  mcolors[0][0] = 10; mcolors[0][1] = 0; mcolors[0][2] = 2;
  mcolors[1][0] = 1; mcolors[1][1] = 10; mcolors[1][2] = 0;
  mcolors[2][0] = 0; mcolors[2][1] = 0; mcolors[2][2] = 10;
  mcolors[3][0] = 10; mcolors[3][1] = 10; mcolors[3][2] = 1;
  mcolors[4][0] = 0; mcolors[4][1] = 10; mcolors[4][2] = 10;
  mcolors[5][0] = 10; mcolors[5][1] = 0; mcolors[5][2] = 10; 
  mcolors[6][0] = 10; mcolors[6][1] = 10; mcolors[6][2] = 10;
  mcolors[7][0] = 0;  mcolors[7][1] = 10; mcolors[7][2] = 5;
  mcolors[8][0] = 3; mcolors[8][1] = 10; mcolors[8][2] = 2; 
  randomcolor(startcolor);
  randomcolor(nextcolor);
  memcpy(&currentcolor, &startcolor, sizeof(startcolor)); 
  Serial.begin(9600);
  TCL.sendEmptyFrame();
  clearallcolors();
}

void loop() {
  
  if (random(500) == 1) {
    randomcolor(startcolor);
    memcpy(&currentcolor, &startcolor, sizeof(startcolor));
    randomcolor(nextcolor);
    mixfactor = 0.0;
    brightnessfactor = 0.01;
    Serial.print("flash");
    printColor(startcolor);
    printColor(nextcolor);
    dimming = false;
    fwdmix = true;
    flash();
  }
  
  combinecolors(startcolor, nextcolor, currentcolor, mixfactor);
  if (fwdmix && mixfactor > .99) {
    fwdmix = false;
  } 
  if (!fwdmix && mixfactor < .01) {
    fwdmix = true;
  }
  
  if (fwdmix) {
    mixfactor += .01;
  } else {
    mixfactor -= .01;
  }
  
  
  
  setbrightness(currentcolor, brightnessfactor);
  printColor(currentcolor);
  if (dimming && brightnessfactor < .1) {
    dimming = false;  
  }
  if (!dimming && brightnessfactor > .99) {
    dimming = true; 
  }
  
  if (dimming) {
    brightnessfactor -= .05;
  } else {
    brightnessfactor += .05;
  }
  
  setcolor(0, currentcolor);
  bumpcolors();
  mirrorcolors();
  sendallcolors();
  delay(DELAY);
}

// utility functions
float brightness(float* color) {
  return(.299 * color[0] + .587 * color[1] + .114 * color[2]);
}

float maxcolorval(float* color) {
  return(max(max(color[0], color[1]), color[2])); 
}

float mincolorval(float* color) {
  return(min(min(color[0], color[1]), color[2])); 
}

void setbrightness(float* color, float brightnessfac) {
  float multiplier = (brightnessfac * MAXBRIGHT) / brightness(color);
  for (int i = 0; i < 3; i++ ) {
    color[i] = color[i] * multiplier;
  }    
}

void flash() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      allcolors[i][j] = 100;
    } 
  }
}

void bumpcolors() {
  for (int i = LEDS - 2; i >= 0; i--) {
    for (int j = 0; j < 3; j++ ) {
      allcolors[i+1][j] = allcolors[i][j];
    }
  }
}

void mirrorcolors() {
  for (int i = 0; i < LEDS / 2; i++) {
    for (int j = 0; j < 3; j++ ) {
      allcolors[LEDS - i - 1][j] = allcolors[i][j];
    }
  }
}

void setcolor(int index, float* color) {
  allcolors[index][0] = max(0, min(color[0], 255));
  allcolors[index][1] = max(0, min(color[1], 255));
  allcolors[index][2] = max(0, min(color[2], 255));
}

void randomcolor(float* color) {
    int ind = random(0, NMANUALS - 1);
    color[0] = mcolors[ind][0];
    color[1] = mcolors[ind][1];
    color[2] = mcolors[ind][2];
}

void combinecolors(float* color1, float* color2, float* outcolor, float mix) {  
  for (int i = 0; i < 3; i++ ) {
    outcolor[i] = mix * color1[i] + (1 - mix) * color2[i];
  }  
}


void clearallcolors() {
  for(int i = 0; i < ALLLEDS; i++) {
    TCL.sendColor(0, 0, 0);
  } 
  TCL.sendEmptyFrame();
}

void sendallcolors() {
  for(int i = 0;i < LEDS; i++) {
    TCL.sendColor(allcolors[i][0], allcolors[i][1], allcolors[i][2]);
  } 
  TCL.sendEmptyFrame();
}

void printColor(float* color) {
  Serial.print('R');
  Serial.print(color[0]);
  Serial.print(' ');
  Serial.print('G');
  Serial.print(color[1]);
  Serial.print(' ');
  Serial.print('B');
  Serial.print(color[2]);
  Serial.print('\n');
}

