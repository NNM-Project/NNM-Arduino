#include "HX711.h" //로드셀 hx711 라이브러리
#include <WiFi.h> //와이파이 연결 라이브러리
#include <HTTPClient.h> //인터넷 서버 라이브 러리
#include <Wire.h>

//(로드셀 센서 정의){
HX711 scale(14, 27); // 매트 1번 센서
HX711 scale2(26, 25); //매트 2번 센서
HX711 scale3(22, 23); //매트 3번 샌서
HX711 scale4(17, 5);  //매트 4번 센서
//}

//(캘리브레이션값 저장){
long settingvalue1;
long settingvalue2;
long settingvalue3;
long settingvalue4;
//}

const char* ssid     = ""; //와이파이 이름
const char* password = ""; //와이파이 비밀번호

const char* serverName = ""; //값정송 서버 도메인 주소

int countnum=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);//시리얼 정의
  //(로드셀 캘리브레이션 초기 값){
  settingvalue1 = scale.read(); //1번 센서 처음 세팅 값
  settingvalue2 = scale2.read(); //2번 센서 처음 세팅 값
  settingvalue3 = scale3.read(); //3번 센서 처음 세팅 값
  settingvalue4 = scale4.read(); //4번 센서 처음 세팅 값
  //}
  
  //(와이파이 연결){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { //와이파이 연결 동안 기다림
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  //}
}

void loop() {
  //(로드셀 센서 값 가져오기){
  long reading = valueconf();
  long reading2 = valueconf2();
  long reading3 = valueconf3();
  long reading4 = valueconf4();
  //}
  
  Serial.print(" s1:");
  Serial.print(reading-settingvalue1);
  Serial.print(" s2:");
  Serial.print(reading2-settingvalue2);
  Serial.print(" s3:");
  Serial.print(reading3-settingvalue3);
  Serial.print(" s4:");
  Serial.println(reading4-settingvalue4);

  
  if(reading-settingvalue1<-16000){
    countnum++;
    if(countnum>=3){
      if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        String httpRequestData;
        String httpRequestData2;
        http.begin(client, serverName);
        
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        httpRequestData = "inputmat1=" + String(1);
        
        int httpResponseCode = http.POST(httpRequestData);
        
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          //Serial.println(httpResponseCode);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        // Free resources
            http.end();
        }
        else {
        Serial.println("WiFi Disconnected");
      }
    }
  }
  else{
    countnum=0;
  }
  
}

//(편균값을 측정){
long valueconf(){
  long readingav=0;
  for(int i=0;i<=1;i++){
    readingav=readingav+scale.read();
  }
  return readingav/2;
}

long valueconf2(){
  long readingav=0;
  for(int i=0;i<=1;i++){
    readingav=readingav+scale2.read();
  }
  return readingav/2;
}

long valueconf3(){
  long readingav=0;
  for(int i=0;i<=1;i++){
    readingav=readingav+scale3.read();
  }
  return readingav/2;
}

long valueconf4(){
  long readingav=0;
  for(int i=0;i<=1;i++){
    readingav=readingav+scale4.read();
  }
  return readingav/2;
}
//}
