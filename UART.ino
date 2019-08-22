#include <Adafruit_SSD1306.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <QueueArray.h>

#define IORQ 2
//#define WR 11
#define RD 5
#define CS 14
#define DC 13
#define RES 16
#define D1_DATA 11
#define D0_CLOCK 12
#define IO 3

Adafruit_SSD1306 display(D1_DATA, D0_CLOCK, DC, RES, CS);

volatile QueueArray<byte> buf;

unsigned long lastTime = 0;
bool useStringBuffer = true;
byte IOmode = INPUT;
byte numChars = 0;
void setup() {
  
  pinMode(CS, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(RES, OUTPUT);
  pinMode(D1_DATA, OUTPUT);
  pinMode(D0_CLOCK, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.display();
  display.setRotation(2);
  display.clearDisplay();
  display.display();
  pinMode(IORQ, INPUT);
  //pinMode(RD, INPUT);
  //pinMode(WR, INPUT);
  
  Serial.begin(57600);

  for(int i = 0; i < 8; i++){
    pinMode(IO + i, INPUT);
  }
  IOmode = INPUT;
  
  attachInterrupt(digitalPinToInterrupt(IORQ), IOrequest, FALLING);
}

void IOrequest(){
  //buf.enqueue('x');
  if(true){ //Z80 is writing to IO device, read from data bus

    byte number = 0x00;
    
    for(int i = 0; i < 8; i++){
      if(IOmode != INPUT){
        pinMode(IO + i, INPUT);
      }
      
      number = (number<<1) | ((digitalRead(IO +7- i)) & 0x01);
    }

    
    buf.enqueue(number);
  }else if(digitalRead(RD) == LOW){
    //not implemented
  }
  
}

void loop() {
  //if UART buffer has data, print to serial connection
  if(buf.isEmpty() == false){
    
    if(millis() - lastTime > 40){
      if(display.getCursorY() > 60){
        display.setCursor(0,0);
        display.clearDisplay();
      }
      while(buf.count() > 0){
        display.print((char)buf.peek());
        Serial.write(buf.dequeue());
      }
      lastTime = millis();
      display.display();
    }
    
    
    
    
    
  }
  
}
