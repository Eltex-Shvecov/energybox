#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

using namespace std;
using namespace BearSSL;

const int DEBUG = 2;
const char * SSID = "licey22";
const char * PASS = "Qwerty123";
unique_ptr<WiFiClientSecure>client(new WiFiClientSecure);

void DebugBlink();
void SendCardInfo(String url, String param);

void setup()
{
  Serial.begin(115200);
  pinMode(DEBUG, OUTPUT);
  int start_esp = 10;

  // пауза перед запуском платы
  while(start_esp)
  {
    Serial.printf("ESP8266 start ... (%d)\n", start_esp--);
    DebugBlink();
  }

  // подключение к wifi
  WiFi.begin(SSID, PASS);
  Serial.print("Connecting .");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  // вывод информации о n-point
  Serial.println("");
  Serial.print("Connect SUCCESS! IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (Serial.available() > 0)
  {
    // пришел номер карты и количество очков
    // отправляем данные на сервер
    String param = Serial.readStringUntil('\r');
    SendCardInfo("", param);
  }

  while(true)
  {
    SendCardInfo("", "?card=5609522&point=1");
    delay(5000);
  }
}

void DebugBlink()
{
  digitalWrite(DEBUG, LOW);
  delay(500);
  digitalWrite(DEBUG, HIGH);
  delay(500);
}

void SendCardInfo(String url, String param)
{
  WiFiClient wf_client;
  HTTPClient httpClient;
  client->setInsecure();

  String fullURL = "http://xn--90afcfknpnnq6j.xn--22-mlclgj2f.xn--p1ai/script.php";
  fullURL += param;

  Serial.println(fullURL);
  if (httpClient.begin(wf_client, fullURL))
  {
    int httpCode = httpClient.GET();

    if (httpCode > 0)
    {
      Serial.printf("GET request status code: %d\n", httpCode);
      Serial.write(httpCode);
      //if (httpCode == 200)
      //{
        //String payload = httpClient.getString();
        //Serial.write(payload.c_str());
      //}
    }
    else
    {
      Serial.printf("GET request ERROR: %s\n", httpClient.errorToString(httpCode).c_str());
    }
    httpClient.end();
  }
  else
  {
    Serial.println("Client Unreachable...");
  }
}
