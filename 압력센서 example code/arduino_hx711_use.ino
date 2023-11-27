#include "HX711.h"

HX711 scale(14, 27);
HX711 scale2(26, 25);    // parameter "gain" is ommited; the default value 128 is used by the library
HX711 scale3(22, 23);//오류 발생
HX711 scale4(17, 5);

long settingvalue1;
long settingvalue2;
long settingvalue3;
long settingvalue4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  settingvalue1 = scale.read(); //1번 센서 처음 세팅 값
  
  settingvalue2 = scale2.read(); //1번 센서 처음 세팅 값
  settingvalue3 = scale3.read(); //1번 센서 처음 세팅 값
  settingvalue4 = scale4.read(); //1번 센서 처음 세팅 값
}

void loop() {
  // put your main code here, to run repeatedly:
  long reading = valueconf();
  long reading2 = valueconf2();
  long reading3 = valueconf3();
  long reading4 = valueconf4();
  Serial.print(" s1:");
  Serial.print(reading-settingvalue1);
  Serial.print(" s2:");
  Serial.print(reading2-settingvalue2);
  Serial.print(" s3:");
  Serial.print(reading3-settingvalue3);
  Serial.print(" s4:");
  Serial.println(reading4-settingvalue4);
}

long valueconf(){
  long readingav=0;
  for(int i=0;i<=3;i++){
    readingav=readingav+scale.read();
  }
  return readingav/4;
}

long valueconf2(){
  long readingav=0;
  for(int i=0;i<=3;i++){
    readingav=readingav+scale2.read();
  }
  return readingav/4;
}

long valueconf3(){
  long readingav=0;
  for(int i=0;i<=3;i++){
    readingav=readingav+scale3.read();
  }
  return readingav/4;
}

long valueconf4(){
  long readingav=0;
  for(int i=0;i<=3;i++){
    readingav=readingav+scale4.read();
  }
  return readingav/4;
}
