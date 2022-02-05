float resistors[5] = {100.0, 1000.0, 10000.0, 104700.0, 1035000.0}; //R2, R3, R4, R5, R6
float resistance;
byte resistorUsed;
uint16_t analogVoltage;
float resistanceOfUnknown;
float convertedVoltage;

void setup() {
  Serial.println("Ohmmeter, epic!");
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  resistorUsed = 5;
  switchResistor(resistorUsed);
}

void switchResistor(int num) {
  for (int i = 2; i < 7; ++i) {
    if (num == i) {
      digitalWrite(i, HIGH);
      Serial.println("Resistor at the pin " + String(i) + " is set to high.");
    }
    else {
      digitalWrite(i, LOW);
      Serial.println("Resistor at the pin " + String(i) + " is set to low.");
    }
  }
  resistance = resistors[num-2];

  Serial.println("Switched to resistor at pin " + String(num) + ".");
  Serial.println("Current known resistance is " + String(resistance) + ".");
}

void loop() {
  Serial.println("----------------");
  analogVoltage = analogRead(A1);
  analogVoltage = 1023 - analogVoltage;
  Serial.println("resistorUsed: " + String(resistorUsed));
  Serial.println("analogVoltage: " + String(analogVoltage));
  if (analogVoltage > 600 && resistorUsed < 6) {
    resistorUsed++;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }

  if (analogVoltage < 100 && resistorUsed > 2) {
    resistorUsed--;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }
  /*
  if(analogVoltage <= 650 && resistorUsed < 6) {
    resistorUsed++;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }
 
  if(analogVoltage >= 100 && resistorUsed > 2) {
    resistorUsed--;
    switchResistor(resistorUsed);
    delay(50);
    return;
  }
*/
  if (analogVoltage < 900) {
    convertedVoltage = (float)analogVoltage * (5.0 / 1024.0);
    resistanceOfUnknown = (float)(convertedVoltage*resistance)/(float)(5.0-convertedVoltage);
    Serial.println("Voltage: " + String(convertedVoltage));
    Serial.println("Resistance of the unknown resistor: " + String(resistanceOfUnknown));
  } else {
    Serial.println("Too much/Too little.");
  }
  delay(1500);
}
