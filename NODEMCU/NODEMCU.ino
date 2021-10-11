#include <rdm6300.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RDM_TX_PIN 13
#define DEBUG(var) Serial.println(var)

const int DEBUG_LED = 2;
const int BUTTON = 16;
const char * SSID = "licey22";
const char * PASS = "cYsRpDY4";

Rdm6300 CardReader;

void SendCardInfo(String param);

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


void setup()
{
  CardReader.begin(RDM_TX_PIN);
  Serial.begin(115200);
  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, HIGH);

  // пауза перед запуском платы
  int start_esp = 5;
  while (start_esp)
  {
    Serial.printf("NODE MCU Started ... (%d)\n", start_esp--);
    delay(1000);
  }

  // подключение к WiFi сети
  WiFi.begin(SSID, PASS);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(DEBUG_LED, LOW);
  delay(2000);
  Serial.printf("\nConnecting Success!\n");
  digitalWrite(DEBUG_LED, HIGH);
}

void loop()
{
  // обработчик датчика препятствия
  if (!digitalRead(BUTTON))
  {
    while(!digitalRead(BUTTON)) {}
    Point.plusPoint(1);
    DEBUG("Plus point");
    digitalWrite(DEBUG_LED, LOW);
    delay(1000);
    digitalWrite(DEBUG_LED, HIGH);
    delay(1000);
  }
  
  // обработчик считывания карточки
  if (CardReader.update())
  {
    digitalWrite(DEBUG_LED, LOW);
    delay(1000);
    digitalWrite(DEBUG_LED, HIGH);
    String card_id = String(CardReader.get_tag_id());
    String query = "?card=";
    query += card_id;
    query += "&point=";
    String point = String(Point.getPoint());
    query += point;
 
    if(!Point.isZero())
    {
      Point.zeroPoint();
    }
    
    SendCardInfo(query);
    delay(2000);
  }

  // обработчик serial порта
  if (Serial.available())
  {
    String str = String(Serial.readString());
    Serial.println(str);
    SendCardInfo(str);
  }
}

void SendCardInfo(String param)
{
  WiFiClient wf_client;
  HTTPClient httpClient;
  //client->setInsecure();
  String fullURL = "http://xn--90afcfknpnnq6j.xn--22-mlclgj2f.xn--p1ai/script.php";
  fullURL += param;

  if (httpClient.begin(wf_client, fullURL))
  {
    int httpCode = httpClient.GET();

    if (httpCode > 0)
    {
      String payload = String(httpClient.getString());
      DEBUG(payload.c_str());
    }
    else
    {
      Serial.printf("GET request ERROR: %s\n", httpClient.errorToString(httpCode).c_str());
    }
    httpClient.end();
  }
  else
  {
    DEBUG("Client Unreachable...");
  }
}
