float resistors[5] = {98.8-0.3, 1002.0-0.3, 10000.0-0.3, 104700.0-0.3, 1035000.0-0.3}; //R2, R3, R4, R5, R6
float diodesDrops[5] = {0.01, 0.38, 0.38, 0.38, 0.38};
float resistance;
byte resistorUsed;
uint16_t analogVoltage;
float resistanceOfUnknown;
float convertedVoltage;
float diodeVoltageDrop;

#define R2 2;
#define R3 3;
#define R4 4;
#define R5 5;
#define R6 6;

void setup() {
  analogReference(EXTERNAL);
  Serial.println("Ohmmeter, epic!");
  Serial.begin(9600);

  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);

  resistorUsed = R6;
  switchResistor(resistorUsed);
}

void switchResistor(int num) {
  for (int i = R2; i < R6+1; ++i) {
    if (num == i) {
      digitalWrite(i, HIGH);
      Serial.println("Resistor at the pin " + String(i) + " is set to high.");
    }
    else {
      digitalWrite(i, LOW);
      Serial.println("Resistor at the pin " + String(i) + " is set to low.");
    }
  }
  resistance = resistors[num-R2];
  diodeVoltageDrop = diodesDrops[num-R2];

  Serial.println("Switched to resistor at pin " + String(num) + ".");
  Serial.println("Current known resistance is " + String(resistance) + ".");
  Serial.println("Diode voltage drop is " + String(diodeVoltageDrop) + ".");
}

void loop() {
  Serial.println("--------Beginning--------");
  analogVoltage = analogRead(A1);
  
  if(analogVoltage >= 550 && resistorUsed < R6) {
    resistorUsed++;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }
 
  if(analogVoltage <= 90 && resistorUsed > R2) {
    resistorUsed--;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }

  Serial.println("Current analogRead(A1): " + String(analogVoltage));
  if (analogVoltage < 900) {
    convertedVoltage = (float)analogVoltage * (5.0 / 1024.0) + (float)diodeVoltageDrop;
    resistanceOfUnknown = (float)(convertedVoltage*resistance)/(float)(5.0-convertedVoltage);
    Serial.println("Voltage: " + String(convertedVoltage));
    Serial.println("Resistance of the unknown resistor: " + String(resistanceOfUnknown));
  } else {
    Serial.println("Too much/Too little.");
  }
  delay(1500);
}
