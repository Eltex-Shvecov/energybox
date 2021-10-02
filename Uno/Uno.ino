#include <SoftwareSerial.h>

const int rx = 10;
const int tx = 11;
const int BUTTON = 8;
const int DEBUG = 13;

SoftwareSerial Esp_Serial(rx, tx);

void debugBlink()
{
  digitalWrite(DEBUG, HIGH);
  delay(2000);
  digitalWrite(DEBUG, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Esp_Serial.begin(9600);
  pinMode(BUTTON, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON) == HIGH)
  {
    while (digitalRead(BUTTON) == HIGH)
    {
      // ждем когда кнопка будет отпущена
    }

    debugBlink();

    // тут отправляем данные якобы карточки
    Esp_Serial.write("testcard");
  }
}
