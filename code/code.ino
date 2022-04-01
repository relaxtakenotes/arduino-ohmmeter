#include <LiquidCrystal.h>

float resistors[5] = {98.7, 999.7, 9999.7, 104599.7, 1033999.0}; //R2, R3, R4, R5, R6
float resistorCorrections[5] = {-3.0, 4.2, 40.0, 0, 0}; // these are always added so if u need to subtract do negative values
byte resistorUsed;
char lcdBufferTop[16];
char lcdBufferBottom[16];
char calculatedResistanceChar[16];
char calculatedCapacitanceChar[16];

unsigned long time0, time1, time2; 
float c, null0;
byte kn, mk;
bool capacitanceCalibrated = false;

bool capacitanceFound = false;
bool resistanceFound = false;


// Capacitance/Capacitor pins
#define cCharge A3
#define cDrain A2
#define cRead A0

// Resistor pins
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define rRead A1

// LCD Display pins
#define RS 7
#define EN 8
#define DB4 9
#define DB5 10
#define DB6 11
#define DB7 12

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);

  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);

  resistorUsed = R6;
  switchResistor(resistorUsed);

  lcd.begin(16, 2);

  capacitance(); // here so that it has time to calibrate the zero point
  null0=c+0.02;
}

// I don't really want to to lcd.clear everytime because it can cause flicker so lets do this instead.
char* ftws(char text[]) {
  size_t length = strlen(text);
  while(length < 17){
    text[length] = ' ';
    length++;
  }
  return text;
}

void switchResistor(int num) {
  for (int i = R2; i <= R6; ++i)
    digitalWrite(i, num==i ? HIGH : LOW);
}

void ohmmeter() {
  uint16_t analogVoltage = 1023 - analogRead(rRead);
  if (analogVoltage > 550 && resistorUsed < R6) {
    resistorUsed++;
    switchResistor(resistorUsed);
    delay(50);
    ohmmeter();
  }

  if (analogVoltage < 100 && resistorUsed > R2) {
    resistorUsed--;
    switchResistor(resistorUsed);
    delay(50);
    ohmmeter();
  }

  if (analogVoltage < 900) {
    float convertedVoltage = (float)analogVoltage * (5.0 / 1024.0);
    float calculatedResistance = convertedVoltage*resistors[resistorUsed-R2]/(5.0-convertedVoltage)+resistorCorrections[resistorUsed-R2];
    dtostrf(calculatedResistance, 4, 2, calculatedResistanceChar);
    resistanceFound = true;
  } else {
    resistanceFound = false;
  }

  delay(400);
}

void capacitance() {
  if (mk == 0) {
    pinMode(cCharge,OUTPUT);
    pinMode(cDrain,INPUT);
    digitalWrite(cCharge,HIGH);
  }

  if (mk == 1) {
    pinMode(cDrain,OUTPUT);
    pinMode(cCharge,INPUT);
    digitalWrite(cDrain,HIGH);
  }

  time0 = micros();
  while (analogRead(cRead) < 644){
    time2 = micros() - time0;
    if (time2 >= 1000000 && mk == 0){
      mk = 1;
      time0 = 100000000;
      break;
    }
  }

  time1 = micros() - time0; 

  while (analogRead(cRead) > 0) { 
    pinMode(cDrain,OUTPUT); 
    pinMode(cCharge,OUTPUT); 
    digitalWrite(cDrain,LOW); 
    digitalWrite(cCharge,LOW);
  }

  if (mk == 1 && time1 < 1000) {
    mk=0;
  }

  c = time1;
  c = c / 1000 - null0;
  c = abs(c);

  if (time1 < 10000000) {
    dtostrf(abs(c), 4, 2, calculatedCapacitanceChar);
    capacitanceFound = true;
  } else {
    capacitanceFound = false;
  }

  if (!capacitanceCalibrated) {
    null0 = c;
    capacitanceCalibrated = true;
  }
}

void fillBufferCapacitance() {
  if (!resistanceFound)
    sprintf(lcdBufferTop, "Capacitance:");
  sprintf(lcdBufferBottom, "%s", calculatedCapacitanceChar);
  if (mk == 0) {
    sprintf(lcdBufferBottom, "%snF", calculatedCapacitanceChar);
  }
  if (mk == 1) {
    sprintf(lcdBufferBottom, "%suF", calculatedCapacitanceChar);
  }  
}

void loop() {
  capacitance();
  ohmmeter();
  if (capacitanceFound && !resistanceFound) {
    fillBufferCapacitance()
  } else if (!capacitanceFound && resistanceFound) {
    sprintf(lcdBufferTop, "Resistance:");
    sprintf(lcdBufferBottom, "%sohm", calculatedResistanceChar);
  } else if (capacitanceFound && resistanceFound) {
    sprintf(lcdBufferTop, "%sohm", calculatedResistanceChar);
    fillBufferCapacitance();
  } else {
    sprintf(lcdBufferTop, "Nothing found...");
    sprintf(lcdBufferBottom, "...");
  }
  lcd.setCursor(0, 0);
  lcd.print(ftws(lcdBufferTop));
  lcd.setCursor(0, 1);
  lcd.print(ftws(lcdBufferBottom));

  delay(100);
}