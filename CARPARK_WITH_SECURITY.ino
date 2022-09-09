
#define SS_PIN 10
#define RST_PIN 9

#define trigPin 7
#define echoPin 8
#define buzzerPin 2

#define RED 4
#define GREEN 5

#define motor1Pin 3
#define motor2Pin 6

#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo motor1;
Servo motor2;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);

  SPI.begin();
  mfrc522.PCD_Init();


}

void loop() {
  door_controller(false);

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }


  if (card_data().substring(1) == "84 c3 53 2e")
  {
    door_controller(true);
    delay(2000);
  }


}

bool door_permission(int distance) {
  if (distance < 20) {
    return false;
  } else {
    beep_sound(false);
    return true;
  }
}

int find_distance() {
  int tim;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  tim = pulseIn(echoPin, HIGH , 11600);
  distance = tim * 0.0345 / 2;

  return distance;
}

String card_data() {

  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {

    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  return content;

}

void door_controller(bool ok) {

  if (ok) {
    motor1.write(90);
    motor2.write(90);
    led_controller(true);
    beep_sound(true);

  } else {

    if (door_permission(find_distance())) {
      motor1.write(0);
      motor2.write(178);
      led_controller(false);
    }

  }

}

void led_controller(bool ok) {
  if (ok) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  } else {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
  }

}

void beep_sound(bool ok){
  if(ok){
    tone(buzzerPin,440);
  }else{
    noTone(buzzerPin);
  }
}
