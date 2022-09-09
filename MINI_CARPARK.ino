
#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define trigPin 8
#define echoPin 9

int tim;
int distance;

Servo motor1;
Servo motor2;

int motor_pos = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  motor1.attach(3);
  motor2.attach(6);

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}

void loop() {
  //  digitalWrite(trigPin, LOW);
  //  delayMicroseconds(5);
  //
  //  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(5);
  //
  //  digitalWrite(trigPin, LOW);
  //  delayMicroseconds(5);
  //
  //  tim = pulseIn(echoPin, HIGH , 11600);
  //  distance = tim * 0.0345 / 2;
  //  delay(250);
  //
  //  if (distance < 15) {
  //    motor1.write(90);
  //    delay(100);
  //
  //    motor2.write(90);
  //    delay(100);
  //
  //  } else {
  //    motor1.write(175);
  //    delay(500);
  //    motor2.write(0);

  //  }


  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "84 C3 53 2E") // Make sure you change this with your own UID number
  {
    Serial.println("Authorised access");
    Serial.println();
    delay(100);

    motor1.write(90);
    motor2.write(90);
    delay(2000);
  }

  else   {
    Serial.println(" Access denied");
    delay(3000);
  }

  motor1.write(0);
  motor2.write(178);
  delay(500);


}
