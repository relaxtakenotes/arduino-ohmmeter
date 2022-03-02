//#include <LiquidCrystal.h>

float resistors[5] = {100.0, 1000.0, 10000.0, 100000.0, 1000000.0}; //R2, R3, R4, R5, R6
byte resistorUsed;

// Resistor pins
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6

// LCD Display pins
#define RS 7
#define EN 8
#define DB4 9
#define DB5 10
#define DB6 11
#define DB7 12

//LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

void setup() {
  Serial.begin(9600);

  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);

  resistorUsed = R6;
  switchResistor(resistorUsed);
  /*
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Resistance: ");
  lcd.setCursor(0, 1);
  */
}

void switchResistor(int num) {
  for (int i = R2; i <= R6; ++i)
    digitalWrite(i, num==i ? HIGH : LOW);
}

void loop() {
  uint16_t analogVoltage = 1023 - analogRead(A1);

  if (analogVoltage > 550 && resistorUsed < R6) {
    resistorUsed++;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }

  if (analogVoltage < 100 && resistorUsed > R2) {
    resistorUsed--;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }

  if (analogVoltage < 900) {
    float convertedVoltage = (float)analogVoltage * (5.0 / 1024.0);
    //lcd.print(convertedVoltage*resistors[resistorUsed-R2])/(5.0-convertedVoltage);
    Serial.println(convertedVoltage*resistors[resistorUsed-R2])/(5.0-convertedVoltage);
  } else {
    Serial.println("Invalid");
  }
  delay(1500);
}
