#include <SPI.h>
#include <RFID.h>
#include <Servo.h>

RFID rfid(10, 9);
Servo myservo;

byte myKey[] = {
  45, 46, 53, 91, 109
};
byte myKey2[] = {
  110, 166, 52, 0, 252
};
byte wrongCount = 0;
byte locki = 1;

void setup()
{
  Serial.begin(230400);
  SPI.begin();
  rfid.init();
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  myservo.attach(7);
  myservo.write(0);
}

void loop()
{
  if (rfid.isCard()) {
    digitalWrite(8, HIGH);
    digitalWrite(6, LOW);
    delay(50);
    digitalWrite(6, HIGH);
    //Serial.println("Find the card!");
    if (rfid.readCardSerial()) {
      //Serial.print("The card's number is  : ");
      /*Serial.print(rfid.serNum[0], DEC);
        Serial.print(",");
        Serial.print(rfid.serNum[1], DEC);
        Serial.print(",");
        Serial.print(rfid.serNum[2], DEC);
        Serial.print(",");
        Serial.print(rfid.serNum[3], DEC);
        Serial.print(",");
        Serial.print(rfid.serNum[4], DEC);
        Serial.println(" ");
        //Serial.print("Is Doorcard: ");
        //Serial.println(rfid.serNum[0] == 110 && rfid.serNum[1] == 166 && rfid.serNum[2] == 52 && rfid.serNum[3] == 0 && rfid.serNum[4] == 252);*/
      /*Serial.print(rfid.serNum[0], HEX);
        Serial.print(rfid.serNum[1], HEX);
        Serial.print(rfid.serNum[2], HEX);
        Serial.print(rfid.serNum[3], HEX);
        Serial.println(rfid.serNum[4], HEX);*/
    }
    //选卡，可返回卡容量（锁定卡片，防止多数读取），去掉本行将连续读卡
    //Serial.print(" ");
    rfid.selectTag(rfid.serNum);
    /*Serial.print(rfid.selectTag(rfid.serNum));
      Serial.println(" Sectors.");*/
    if ((rfid.serNum[0] == myKey[0] && rfid.serNum[1] == myKey[1] && rfid.serNum[2] == myKey[2] && rfid.serNum[3] == myKey[3] && rfid.serNum[4] == myKey[4])) {
      wrongCount = 0;
      delay(30);
      Serial.print("Hello, XAS-712. ");
      myservo.write(90);
      digitalWrite(6, LOW);
      delay(200);
      digitalWrite(6, HIGH);
      Serial.println("Door is opening...");
      delay(7800);
      Serial.println("Door will close after 5 seconds.");
      delay(100);
      digitalWrite(8, LOW);
      digitalWrite(6, LOW);
      delay(100);
      digitalWrite(8, HIGH);
      digitalWrite(6, HIGH);
      delay(4800);
      Serial.println("Door is closed.");
      myservo.write(0);
      digitalWrite(8, LOW);
      digitalWrite(6, LOW);
      delay(50);
      digitalWrite(6, HIGH);
      delay(50);
      digitalWrite(6, LOW);
      delay(50);
      digitalWrite(6, HIGH);
    } else {
      Serial.println("Sorry, wrong card.");
      wrongCount++;
      digitalWrite(8, LOW);
      digitalWrite(6, HIGH);
      delay(100);
      digitalWrite(8, HIGH);
      digitalWrite(6, LOW);
      delay(100);
      digitalWrite(8, LOW);
      digitalWrite(6, HIGH);
      delay(100);
      digitalWrite(8, HIGH);
      digitalWrite(6, LOW);
      delay(100);
      digitalWrite(8, LOW);
      digitalWrite(6, HIGH);
      delay(100);
      digitalWrite(8, HIGH);
      digitalWrite(6, LOW);
      delay(100);
      digitalWrite(8, LOW);
      digitalWrite(6, HIGH);
      if (wrongCount > 4)
      {
        wrongCount = 5;
        Serial.println("A wrong card has been tried for more than 5 times. The doorlock has been locked for 1 minute.");
        digitalWrite(8, LOW);
        for (int i = 0; i < 60 * locki; i++)
        {
          delay(500);
          digitalWrite(8, HIGH);
          digitalWrite(6, LOW);
          delay(500);
          digitalWrite(8, LOW);
          digitalWrite(6, HIGH);
        }
      }
    }
  } else {
    digitalWrite(8, LOW);
  }
  rfid.halt();
}
