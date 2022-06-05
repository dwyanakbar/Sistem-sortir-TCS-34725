#include "HX711.h"
#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"
 
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
HX711 scale;
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3; 
const int led1 = 8, led2 = 9, led3 = 10, led4 = 11;
String color;
Servo myservo0;
Servo myservo1;
Servo myservo2;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  myservo0.attach(4);
  myservo1.attach(5);
  myservo2.attach(6);
  myservo1.write(0);
  myservo2.write(0);
  myservo0.write(45);
  delay(2000);
  myservo0.write(0);
  delay(1000);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(1001.03);   //1001.03                   // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare(50);                // reset the scale to 0
}

void loop() {
  switch(Serial.read()){
    case '1':
      digitalWrite(led1, HIGH);
      delay(1500);
      myservo1.write(45);
      delay(3000);
      myservo2.write(90);
      delay(1000);
      myservo2.write(0);
      digitalWrite(led1, LOW);
      delay(1000);
      myservo0.write(45);
      delay(1000);
      myservo0.write(0);
      delay(1000);
    break;
    case '2':
      digitalWrite(led2, HIGH);
      delay(1500);
      myservo1.write(90);
      delay(3000);
      myservo2.write(90);
      delay(1000);
      myservo2.write(0);
      digitalWrite(led2, LOW);
      delay(1000);
      myservo0.write(45);
      delay(1000);
      myservo0.write(0);
      delay(1000);
    break;
    case '3':
      digitalWrite(led3, HIGH);
      delay(1500);
      myservo1.write(135);
      delay(3000);
      myservo2.write(90);
      delay(1000);
      myservo2.write(0);
      digitalWrite(led3, LOW);
      delay(1000);
      myservo0.write(45);
      delay(1000);
      myservo0.write(0);
      delay(1000);
    break;
    case '4':
      digitalWrite(led4, HIGH);
      delay(1500);
      myservo1.write(180);
      delay(3000);
      myservo2.write(90);
      delay(1000);
      myservo2.write(0);
      digitalWrite(led4, LOW);
      delay(1000);
      myservo0.write(45);
      delay(1000);
      myservo0.write(0);
      delay(1000);
    break;
    default:
      load_color();
    break;
  }
}

void load_color(){
  uint16_t clearcol, red, green, blue;
  float average, r, g, b;
  tcs.getRawData(&red, &green, &blue, &clearcol);
  average = (red+green+blue)/3;
  r = red/average;
  g = green/average;
  b = blue/average;
//Serial.print("RED:"); Serial.print(r); Serial.print(" ");
//Serial.print("GREEN:"); Serial.print(g); Serial.print(" ");
//Serial.print("BLUE:"); Serial.print(b); Serial.print(";");
  if ((r < 1.00) && (g > 1.3) && (b < 0.9)) { // yellow detection
    color = "HIJAU";
  }
  else if ((r > 1.15) && (g > 1.15) && (b < 0.8)) { // yellow detection
    color = "HIJAU KUNING";
  }
  else if ((r > 1.15) && (g < 1.15) && (b < 0.7)) { // yellow detection
    color = "KUNING";
  }
  else if ((r > 1.2) && (g < 1.0) && (b < 0.7)) { // orange detection
    color = "ORANYE";
  }
  else {
    color = "WARNA TAK TERDETEKSI";
  }

  float berat= scale.get_units(25);
  if(berat<1)
  {
    berat=0.0;
  }

  Serial.print(color);
  Serial.print(";");
  Serial.println(berat,1);
  scale.power_down();
  delay(1000);
  scale.power_up();
}
