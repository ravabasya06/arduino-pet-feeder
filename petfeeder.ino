#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "wifi";
const char* password = "password";

#define BOTtoken "tokenhashblabla"

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

int servoPin = 18;
int buzzerPin = 19;

Servo myservo;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
//  Serial.println("handleNewMessages");
//  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/kelompok_3") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "/feed to feed cats \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    
    if (text == "/feed") {
      bot.sendMessage(chat_id, "Outputted a good amount", "");
      myservo.write(20);
      digitalWrite(buzzerPin, LOW);
      delay(1000);
      myservo.write(180);
      digitalWrite(buzzerPin, HIGH);
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  myservo.attach(servoPin);
  myservo.write(180);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
