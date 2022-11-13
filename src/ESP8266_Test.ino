//-----------------------------------------------
// Server_GAS_ID: AKfycbzx4WXjxoFIKBK_UaYlNzMdKfXytv38JNR9mobo2jtKUhuEYkxq9k04uMELBrdSpQ
//-----------------------------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11

#define R_Led D7
#define G_Led D6
#define B_Led D5

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "varlamnet";
const char* password = "34513511";
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client;

const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbzx4WXjxoFIKBK_UaYlNzMdKfXytv38JNR9mobo2jtKUhuEYkxq9k04uMELBrdSpQ";
int it;
int ih;
void setup() {
  dht.begin();
  Serial.begin(115200);
  Serial.println();

  pinMode(R_Led, OUTPUT);
  pinMode(G_Led, OUTPUT);
  pinMode(B_Led, OUTPUT);

  digitalWrite(R_Led, HIGH);
  digitalWrite(G_Led, LOW);
  digitalWrite(B_Led, LOW);
  
  Serial.print("Connect to ");
  Serial.println(ssid);
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  digitalWrite(R_Led, LOW);
  digitalWrite(G_Led, LOW);
  digitalWrite(B_Led, HIGH);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temp = ");
  Serial.print(t);
  Serial.print(" HUM= ");
  Serial.println(h);
  it = (int) t;
  ih = (int) h;
  sendData(it, ih);

  Serial.println("DeepSleep");
  ESP.deepSleep(3600e6);
}

void sendData(int tem, int hum) {
  digitalWrite(R_Led, LOW);
  digitalWrite(G_Led, LOW);
  digitalWrite(B_Led, HIGH);
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    digitalWrite(R_Led, HIGH);
    digitalWrite(G_Led, LOW);
    digitalWrite(B_Led, LOW);
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String string_temperature =  String(tem, DEC);
  String string_humidity =  String(hum, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;

  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  digitalWrite(R_Led, LOW);
  digitalWrite(G_Led, HIGH);
  digitalWrite(B_Led, LOW);
}
