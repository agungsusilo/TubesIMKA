/*******************************************************************
    Control an LED strip using Inline Keyboard on Telegram
    By Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>

// Library for interacting with the Telegram API
// Search for "Telegegram" in the Library manager and install
// The universal Telegram library
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot


#include <ArduinoJson.h>
// Library used for parsing Json from the API responses
// NOTE: There is a breaking change in the 6.x.x version,
// install the 5.x.x version instead
// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------

char ssid[] = "MATA SAPI";         // your network SSID (name)
char password[] = "telurmatasapi"; // your network password

#define TELEGRAM_BOT_TOKEN "insert your telegram token API"


//------- ---------------------- ------


// This is the Wifi client that supports HTTPS
WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, client);

#define LED_PIN 2 // Same as D4 for Wemos


//const int kunciPintu = 5;
Servo kunciku;
const int tombol = 4;
int statusKunci = 0;
int buttonState = 0;


int delayBetweenChecks = 1000;
unsigned long lastTimeChecked;   //last time messages' scan has been done

unsigned long lightTimerExpires;
boolean lightTimerActive = false;

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  //button
  //pinMode(kunciPintu, OUTPUT):
  pinMode(tombol, INPUT);

  //servo
  kunciku.attach(5);
  kunciku.write(0);
  
  // attempt to connect to Wifi network:
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

  // Only required on 2.5 Beta
  client.setInsecure();

  
  // longPoll keeps the request to Telegram open for the given amount of seconds if there are no messages
  // This hugely improves response time of the bot, but is only really suitable for projects
  // where the the initial interaction comes from Telegram as the requests will block the loop for
  // the length of the long poll
  //bot.longPoll = 60;
}

void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {

//    // If the type is a "callback_query", a inline keyboard button was pressed
//    if (bot.messages[i].type ==  F("callback_query")) {
//      String text = bot.messages[i].text;
//      Serial.print("Call back button pressed with text: ");
//      Serial.println(text);
//
//      if (text == F("ON")) {
//        digitalWrite(LED_PIN, HIGH);
//      } else if (text == F("OFF")) {
//        digitalWrite(LED_PIN, LOW);
//      } else if (text.startsWith("TIME")) {
//        text.replace("TIME", "");
//        int timeRequested = text.toInt();
//        
//        digitalWrite(LED_PIN, HIGH);
//        lightTimerActive = true;
//        lightTimerExpires = millis() + (timeRequested * 1000 * 60);
//      }
//    } else {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      String from_name = bot.messages[i].from_name;

      //if (text == F("/opsi")) {
        

        // Keyboard Json is an array of arrays.
        // The size of the main array is how many row options the keyboard has
        // The size of the sub arrays is how many coloums that row has

        // "The Text" property is what shows up in the keyboard
        // The "callback_data" property is the text that gets sent when pressed  
        
//        String keyboardJson = F("[[{ \"text\" : \"OFF\", \"callback_data\" : \"ON\" },{ \"text\" : \"ON\", \"callback_data\" : \"OFF\" }],");
//        keyboardJson += F(" ");
//        // [{\"text\" : \"10 Mins\", \"callback_data\" : \"TIME10\" }, { \"text\" : \"20 Mins\", \"callback_data\" : \"TIME20\" }, { \"text\" : \"30 Mins\", \"callback_data\" : \"TIME30\" }]]
//        bot.sendMessageWithInlineKeyboard(chat_id, "Opsi Lain", "", keyboardJson);
     // }

      // When a user first uses a bot they will send a "/start" command
      // So this is a good place to let the users know what commands are available
      
      if (text == "/start") {
      String welcome = "Selamat Datang di bot Tubes IMKA, " + from_name + ".\n";
      welcome += "Berikut adalah contoh perintah yang dapat kamu lakukan :\n\n";
      welcome += "/cek : untuk mengecek status kunci\n";
      welcome += "/kunci : untuk mengunci pintu\n";
      welcome += "/bukakunci : untuk membuka kunci\n";
      //welcome += "/opsi : untuk menampilkan opsi\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
      }
      
      if (text == "/cek") {
        if (statusKunci==0){
          bot.sendMessage(chat_id, "Pintu belum dikunci, apakah anda ingin men /kunci ?", "");
        } else {
          bot.sendMessage(chat_id, "Pintu sudah terkunci, anda ingin /bukakunci ?", "");
        }
      }

      if (text == "/kunci") {
        statusKunci = 1;
        kunciku.write(90);
        bot.sendMessage(chat_id, "Pintu dikunci!", "");
      }

      if (text == "/bukakunci") {
        statusKunci = 0;
        kunciku.write(0);
        bot.sendMessage(chat_id, "Pintu dibuka!", "");
      }
      
    }
//  }
}

void loop() {
  if (millis() > lastTimeChecked + delayBetweenChecks)  {

    // getUpdates returns 1 if there is a new message from Telegram
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
    }

    lastTimeChecked = millis();

    if (lightTimerActive && millis() > lightTimerExpires) {
      lightTimerActive = false;
      digitalWrite(LED_PIN, LOW);
    }
  }
}
