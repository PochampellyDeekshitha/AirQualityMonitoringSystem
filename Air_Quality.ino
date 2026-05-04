#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>

// 🌐 WiFi
const char* ssid = "R14";
const char* password = "password08";

// 🤖 Telegram
#define BOT_TOKEN "8637801084:AAEvQVrWDuqjNRxNIPyTw4Gbzdd5JcLviFM"
//6017560316
String chatIds[] = {"6128971206"};
int numChats = sizeof(chatIds) / sizeof(chatIds[0]);

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ☁️ ThingSpeak
String apiKey = "CBTA0JVUYJNCEMOI";

// 🌐 Web Server
WebServer server(80);

// 📟 LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 🌡️ DHT
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 🌫️ MQ135
#define MQ135_PIN 34

// 💡 LEDs
#define GREEN_LED 18
#define YELLOW_LED 19
#define RED_LED 23

// 🔊 Buzzer
#define BUZZER 5

// 🌀 Relay
#define RELAY 25

int gas = 0;
float temp = 0, hum = 0;
bool alertSent = false;

unsigned long lastCloud = 0;

// 🌐 Web Page
String getHTML() {

  String status = "GOOD";
  String color = "#00e676";

  if (gas >= 1500) {
    status = "BAD";
    color = "#ff1744";
  } 
  else if (gas >= 1350) {
    status = "MEDIUM";
    color = "#ff9100";
  }

  String page = "<!DOCTYPE html><html><head>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<meta http-equiv='refresh' content='5'>";
  page += "<style>";

  page += "body{margin:0;font-family:Arial;background:linear-gradient(135deg,#0f2027,#203a43,#2c5364);color:white;display:flex;justify-content:center;align-items:center;height:100vh;}";

  page += ".container{width:100%;max-width:380px;padding:10px;}";

  page += "h1{text-align:center;margin-bottom:10px;}";

  page += ".card{background:rgba(255,255,255,0.08);margin:10px 0;padding:15px;border-radius:12px;text-align:center;}";

  page += ".big{font-size:26px;font-weight:bold;}";

  page += ".status{background:" + color + ";color:black;}";

  page += ".alert{background:#ff1744;}";

  page += "</style></head><body>";

  page += "<div class='container'>";

  page += "<h1>Air Quality Monitor</h1>";

  page += "<div class='card'><p>Gas</p><p class='big'>" + String(gas) + "</p></div>";

  page += "<div class='card'><p>Temperature</p><p class='big'>" + String(temp) + " C</p></div>";

  page += "<div class='card'><p>Humidity</p><p class='big'>" + String(hum) + " %</p></div>";

  page += "<div class='card status'><p>Status</p><p class='big'>" + status + "</p></div>";

  if (status == "BAD") {
    page += "<div class='card alert'><p class='big'>BAD AIR QUALITY!</p></div>";
  }

  page += "</div></body></html>";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

// ☁️ Send to Cloud
void sendToCloud(float temp, float hum, int gas, int statusValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey;
    url += "&field1=" + String(gas);
    url += "&field2=" + String(temp);
    url += "&field3=" + String(hum);
    url += "&field4=" + String(statusValue);

    http.begin(url);
    int code = http.GET();
    http.end();

    Serial.println("Cloud Response: " + String(code));
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.print("Starting...");
  delay(1500);
  lcd.clear();

  lcd.print("Air Monitor");
  delay(1500);
  lcd.clear();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  client.setInsecure();
  //client.setTimeout(15000);

  lcd.setCursor(0,0);
  lcd.print("IP:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();

  server.on("/", handleRoot);
  server.begin();

  dht.begin();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);

  // Telegram test
  for (int i = 0; i < numChats; i++) {
    bot.sendMessage(chatIds[i], "ESP32 Connected ✅", "");
  }
}

void loop() {

  gas = analogRead(MQ135_PIN);
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    temp = 0;
    hum = 0;
  }

  String status = "GOOD";
  int statusValue = 0;

  // GOOD
  if (gas < 1350) {
    status = "GOOD";
    statusValue = 0;

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(RELAY, LOW);

    alertSent = false;
  }

  // MEDIUM
  else if (gas < 1500) {
    status = "MEDIUM";
    statusValue = 1;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(RELAY, LOW);
  }

  // BAD
  else {
    status = "BAD";
    statusValue = 2;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(RELAY, HIGH);

    if (!alertSent) {
      String msg = "ALERT!\nAir Quality BAD\n\nGas: " + String(gas) +
                   "\nTemp: " + String(temp) +
                   "\nHumidity: " + String(hum);

      for (int i = 0; i < numChats; i++) {
        bot.sendMessage(chatIds[i], msg, "");
      }

      alertSent = true;
    }
  }

  // LCD
  lcd.setCursor(0,0);
  lcd.print("Gas:");
  lcd.print(gas);
  lcd.print("   ");

  lcd.setCursor(10,0);
  lcd.print(status);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" H:");
  lcd.print(hum);
  lcd.print("   ");

  // Send to Cloud
  //sendToCloud(temp, hum, gas, statusValue);
  
  // ☁️ Send to cloud every 15 sec
  if (millis() - lastCloud > 15000) {
    sendToCloud(temp, hum, gas, statusValue);
    lastCloud = millis();
  }
  server.handleClient();

  delay(15000); // ThingSpeak limit
}