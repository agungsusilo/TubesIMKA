#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "XperiaX"; 
char password[] = "11223344";
#define BOTtoken "884418776:AAHqfWORKzV4nAew6KctbfV4zrsYGbyw33w"
String chatid = "490932184";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(9600);
  WifiStatus();

  bot.sendMessage(chatid, "Tes Kelas Robot");
  Serial.println("Pesan Terkirim ke Telegram");
}

void loop() {

}

void WifiStatus() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
