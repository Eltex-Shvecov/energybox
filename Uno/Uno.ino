#include <rdm6300.h>
#include <SoftwareSerial.h>

#define RDM6300_TX_PIN 9

const int rx_esp = 10;
const int tx_esp = 11;
const int BUTTON = 8;
const int DEBUG = 13;

Rdm6300 CardReader;
SoftwareSerial Esp_Serial(rx_esp, tx_esp);

class PointSystem
{
  public:
    PointSystem() : point(0) {}
    ~PointSystem() {}

    void zeroPoint() { point = 0; }
    int getPoint() { return this->point; }
    void plusPoint(int cnt) { point += cnt; }
    bool isZero()
    {
      if (this->point)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  private:
    int point;
} Point;

void debugBlink()
{
  digitalWrite(DEBUG, HIGH);
  delay(2000);
  digitalWrite(DEBUG, LOW);
}

void setup()
{
  Serial.begin(115200);
  Esp_Serial.begin(9600);
  CardReader.begin(RDM6300_TX_PIN);
  pinMode(BUTTON, INPUT);
}

void loop()
{
  if (CardReader.update())
  {
    if (Point.isZero())
    {
      // пишем на экран количество очков на карте
      digitalWrite(DEBUG, HIGH);
      delay(500);
      digitalWrite(DEBUG, LOW);
    }
    else
    {
      String param = "?card=";
      String cardNumber = String(CardReader.get_tag_id());
      param += cardNumber;
      param += "&point=";
      String point = String(Point.getPoint());
      param += point;
      Esp_Serial.write(param.c_str());
      Point.zeroPoint();
      debugBlink();
    }    
  }
  
  if (digitalRead(BUTTON) == HIGH)
  {
    while (digitalRead(BUTTON) == HIGH)
    {
      // ждем когда кнопка будет отпущена
    }

    debugBlink();

    // тут отправляем данные якобы карточки
    Point.plusPoint(1);
    //Esp_Serial.write("testcard");
  }
}
