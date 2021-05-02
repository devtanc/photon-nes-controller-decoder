#include <Particle.h>

void createBinaryString(unsigned int data, char result[]) {
  for (int i = 0; i < 8; i++) {
    if (data & 1) {
      // 7 - i makes sure that we keep the bits in the right order
      result[7 - i] = '1';
    } else {
      result[7 - i] = '0';
    }
    data >>= 1;
  }
  result[8] = '\0';
}

void clearConsole() {
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void logData(unsigned int data) {
  char dataString[9];
  createBinaryString(data, dataString);
  Serial.println(dataString);
}

void printBitStatus(unsigned int data, unsigned int bitmap) {
  if (data & bitmap) {
    Serial.println("GOOD");
  } else {
    Serial.println("");
  }
}

void printTestStatus(unsigned int buttonBits) {
  clearConsole();
  Serial.println("\n*** TEST STATUS ***");
  Serial.print("Button A:\t\t");
  printBitStatus(buttonBits, 0b00000001);
  Serial.print("Button B:\t\t");
  printBitStatus(buttonBits, 0b00000010);
  Serial.print("Button Select:\t\t");
  printBitStatus(buttonBits, 0b00000100);
  Serial.print("Button Start:\t\t");
  printBitStatus(buttonBits, 0b00001000);
  Serial.print("Button Up:\t\t");
  printBitStatus(buttonBits, 0b00010000);
  Serial.print("Button Down:\t\t");
  printBitStatus(buttonBits, 0b00100000);
  Serial.print("Button Left:\t\t");
  printBitStatus(buttonBits, 0b01000000);
  Serial.print("Button Right:\t\t");
  printBitStatus(buttonBits, 0b10000000);
}
