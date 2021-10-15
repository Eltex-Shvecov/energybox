#include <LiquidCrystal.h>
#include <rdm6300.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RDM_TX_PIN 13
#define LED_RS 5
#define LED_E 4
#define LED_DB_4 00
#define LED_DB_5 2
#define LED_DB_6 14
#define LED_DB_7 12

#define DEBUG(var) Serial.println(var)

const int DEBUG_LED = 2;
const int BUTTERY = 16;
const char * SSID = "Notebook";
const char * PASS = "Noteb00k!";

Rdm6300 CardReader;
LiquidCrystal lcd(LED_RS, LED_E, LED_DB_7, LED_DB_6, LED_DB_5, LED_DB_4);

void LEDPrintWelcomeText();
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
  Serial.begin(115200);
  lcd.begin(16, 2);
  pinMode(DEBUG_LED, OUTPUT);
  pinMode(BUTTERY, INPUT);

  // пауза перед запуском платы
  int start_esp = 5;
  lcd.clear();
  lcd.home();
  lcd.print("Started ...");
  
  while (start_esp)
  {
    lcd.setCursor(12, 0);
    lcd.print(start_esp--);
    delay(1000);
  }

  digitalWrite(DEBUG_LED, HIGH);
  CardReader.begin(RDM_TX_PIN);
  
  // подключение к WiFi сети
  WiFi.begin(SSID, PASS);
  lcd.clear();
  lcd.home();
  lcd.print("Connecting WiFi");

  lcd.setCursor(0, 1);
  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.print(".");
    delay(500);
  }

  lcd.clear();
  lcd.home();
  lcd.print("Connected WiFi!!!");
  delay(2000);
  
  LEDPrintWelcomeText();
}

void loop()
{
  // обработчик датчика препятствия
  if (!digitalRead(BUTTERY))
  {
    while(!digitalRead(BUTTERY)) {}
    Point.plusPoint(1);
    lcd.home();
    lcd.clear();
    lcd.print("Points: ");
    lcd.setCursor(8, 0);
    lcd.print(Point.getPoint());
    delay(1000);
  }
  
  // обработчик считывания карточки
  if (CardReader.update())
  {
    String card_id = String(CardReader.get_tag_id());
    String query = "?card=";
    query += card_id;
    query += "&point=";
    String point = String(Point.getPoint());
    if (Point.getPoint() > 0)
    {
      lcd.clear();
      lcd.home();
      lcd.print("Recording points");
      delay(2000);
    }
    query += point;
 
    if(!Point.isZero())
    {
      Point.zeroPoint();
    }
    
    SendCardInfo(query);
    delay(2000);
    LEDPrintWelcomeText();
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
      int i;
      String payload = String(httpClient.getString());
      char buff[32];
      payload.toCharArray(buff, 32);
      lcd.clear();
      lcd.home();
      for (i = 0; buff[i] != '\n' && buff[i] != '\0'; i++)
      {
        lcd.setCursor(i, 0);
        lcd.print(buff[i]);
      }
      i++;
      for (; buff[i] != '\n' && buff[i] != '\0'; i++)
      {
        lcd.setCursor(i, 1);
        lcd.print(buff[i]);
      }

      //lcd.print(payload.c_str());
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

 void LEDPrintWelcomeText()
 {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("\x9F\x9E\x9C\x9B");
    lcd.setCursor(3, 1);
    lcd.print("Energy Box");
 }

