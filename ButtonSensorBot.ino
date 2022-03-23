#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Set GPIOs for LED and reedswitch
const int buttonSwitch = 22;
const int led = 23; 

// Detects whenever the button state is changed
bool changeState = true;

// holds button state (1=opened,0=close)
bool state;
String doorState;

// Auxiliary variables (it will only detect changes that are 1500 milliseconds apart)
unsigned long previousMillis = 0;
const long interval = 1500;

const char* ssid ="TP-Link_EA6A";
const char* password ="51948592";

// Initialize Telegram BOT
#define BOTtoken "5119312503:AAFrV9kTm9h4FRT5koHmhj6TkZxhTre7ChA"


// Use @myidbot to findout the chat ID of an individual or a group
// Also note taht you need to click "start" on a bot before it can message you

#define CHAT_ID "1871827331"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Runs whenever the reedswitch changes state
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State Changed");
  changeState = true;
}



void setup() {
  // put your setup code here, to run once:
  // Serial port for debugging purposes
  Serial.begin(115200);

  //Read the current door state
  pinMode(buttonSwitch, INPUT_PULLUP);
  state = digitalRead(buttonSwitch);

  // Set LED state to match door state
  pinMode(led, OUTPUT);
  digitalWrite(led, !state);

  // Set the buttonSwitch pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(buttonSwitch), changeDoorStatus, CHANGE);

  //connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   
}
  Serial.println("");
  Serial.println("WiFi connected");

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}
void loop() {
  // put your main code here, to run repeatedly:
  if (changeState) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // If a state has occured, invert the current door state
       state = !state;
       if(state) {
          doorState = "closed";
       
    }
    else {
      doorState = "open";
    }
    digitalWrite(led, !state);
    changeState = false;
    Serial.println(state);
    Serial.println(doorState);

    //Send notification
    bot.sendMessage(CHAT_ID, "The door is " + doorState, "");
  }
    }

}
