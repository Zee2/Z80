#define SER_CLK 3
#define SER_DATA 2
#define ROM_WRITE 4
#define ROM_DATA_0 5
#define ROM_READ 13

int hasTested = 0;
unsigned int codeAddress = 0;
unsigned int maxAddress = 1000;
int dataBusMode = 0;

void setup() {
  Serial.begin(57600);
  randomSeed(analogRead(0));

  digitalWrite(SER_CLK,LOW);
  digitalWrite(ROM_WRITE,HIGH);
  digitalWrite(ROM_READ,HIGH);

  pinMode(ROM_READ, OUTPUT);
  pinMode(ROM_WRITE, OUTPUT);
  
  pinMode(SER_CLK, OUTPUT);
  pinMode(SER_DATA,OUTPUT);

  setDataPinMode(OUTPUT);
  
  digitalWrite(SER_CLK,LOW);
  shiftOutAddress(1024);
  delay(2);
  //writeZeros();
  //memtest(0);
  dumpContents();
  //writeZeros();
  codeAddress = 0;
}

void shiftOutAddress(unsigned int address){
  byte bigByte = (address >> 8) & 0xFF;
  byte littleByte = (address) & 0xFF;
  
  digitalWrite(SER_CLK,LOW);
  shiftOut(SER_DATA,SER_CLK,LSBFIRST,littleByte);
  shiftOut(SER_DATA,SER_CLK,LSBFIRST,bigByte);
  digitalWrite(SER_CLK,HIGH);
  digitalWrite(SER_CLK,LOW);
  
}

void setDataPinMode(int mode){
  for(int i = 0; i < 8; i++){
    pinMode(ROM_DATA_0 + i, mode);
  }
  dataBusMode = mode;
  delay(5);
}

void writeByte(unsigned int address, byte data){
  digitalWrite(ROM_READ, HIGH);
  if(dataBusMode != OUTPUT){
    setDataPinMode(OUTPUT);
  }

  
  
  shiftOutAddress(address);
  delay(6);
  digitalWrite(ROM_WRITE,LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(ROM_DATA_0 + i, ((data >> i)&0x01)); //put data on bus
  }
  digitalWrite(ROM_WRITE,HIGH);
  shiftOutAddress(0);
} 

byte readByte(unsigned int address){
  if(dataBusMode != INPUT){
    setDataPinMode(INPUT);
  }

  shiftOutAddress(0);
  shiftOutAddress(address);
  
  byte number = 0x00;
  digitalWrite(ROM_READ,LOW);
  delay(1);
  
  for(int i = 0; i < 8; i++){
    number = (number<<1) | (digitalRead(ROM_DATA_0 + 7 - i) & 0x01);
  }
  digitalWrite(ROM_READ,HIGH);
  shiftOutAddress(0);
  return number;
}

void writeZeros(){
  Serial.println("Writing zeros");
  byte data = 0x0;

  for(unsigned int address = 0; address < maxAddress; address++){
    writeByte(address, data);
    if(address%100 == 0){
      Serial.print((address/(maxAddress*1.0f))*100.0f);
      Serial.println("% done....");
    }
  }
  dumpContents();
}

void dumpContents(){
  Serial.println("Dumping contents");
  for(unsigned int address = 0; address < maxAddress; address++){
    byte b = readByte(address);
    printByte(b);
  }
}

void memtest(unsigned int startingAddress){
  Serial.print("Performing memory test starting at location ");
  Serial.print(startingAddress);
  Serial.print(" and ending at ");
  Serial.println(maxAddress);
  byte test = 0;

  unsigned long start = millis();
  int errors = 0;
  for(unsigned int address = 0; address < maxAddress - startingAddress; address++){
    writeByte(startingAddress + address, test);
    if(address%100 == 0){
      Serial.print((address/(maxAddress*1.0f))*100.0f);
      Serial.println("% done....");
    }
    if(test < 15){
      test++;
    }else{
      test = 0;
    }
  }
  Serial.print("Took ");
  unsigned long endTime = millis();
  Serial.print((endTime - start)/1000.0f);
  Serial.print(" seconds to write data, average of ");
  Serial.print((maxAddress-startingAddress)/((endTime - start)/1000.0f));
  Serial.println(" B/s");
  Serial.print("Memory dump: ");

  test = 0;
  for(unsigned int address = 0; address < maxAddress - startingAddress; address++){
    byte b = readByte(startingAddress + address);
    Serial.print(b);
    if(b != test){
      errors++;
      Serial.println("");
      Serial.print("Error at address ");
      Serial.println(address);
    }

    if(test < 15){
      test++;
    }else{
      test = 0;
    }
  }

  Serial.print("\nVerification complete: ");
  Serial.print(errors);
  Serial.println(" errors occurred");
  
}

void loop() {
  if(Serial.available()>0){
    
    byte b = Serial.read();
    
    writeByte(codeAddress, b);

    //byte v = readByte(codeAddress);
    Serial.write(b);
    codeAddress++;
    
  }
}

void printByte(byte b){
  if(b < 16){
      Serial.print("0");
    }
    Serial.print(b, HEX);
    Serial.print(" ");
}


