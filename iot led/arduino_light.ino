#include <ESP8266WiFi.h>       // 와이파이 라이브러리 
#include <ESP8266HTTPClient.h> // HTTPClient 라이브러리 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int pin = 4; // DI와 연결된 핀번호
int num_of_led = 4; // 네오픽셀링 led 개수

#define PIN      D2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 4 // Popular NeoPixel ring size
#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid     = "jiseok";
const char* password = "jiseokji";

const String endpoint = "http://leejiseok.iptime.org:30/open.php";
String line = ""; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  get_weather();

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void get_weather() {
  if ((WiFi.status() == WL_CONNECTED)) {
    //Serial.println("Starting connection to server...");
    HTTPClient http;
    http.begin(endpoint);       
    int httpCode = http.GET();  
    if (httpCode > 0) {
      line = http.getString();
    }
    else {
      Serial.println("Error on HTTP request");
    }

    parsing();
    http.end(); //Free the resources
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  get_weather();  
  delay(500);
}

void parsing() {
  String pin,pin2,pin3,pin4,pin5;  
  int tm_start= line.indexOf(F("<h1>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int  tm_end= line.indexOf(F("</h1>"));
  
  pin = line.substring(tm_start + 4, tm_end);  
  
  int myInt = pin.toInt();
  
  Serial.println(myInt);
   pixels.clear();
  if(myInt == 0){
     pixels.clear();
     for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  
      pixels.show();   // Send the updated pixel colors to the hardware.
  
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
   else{
    Serial.println("led off");
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(65, 194, 255));
  
      pixels.show();   // Send the updated pixel colors to the hardware.
  
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
}
