#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// 🌐 WiFi
const char* ssid = "12345678";
const char* password = "12345678";

// 🌐 Web Server
WebServer server(80);

// 📟 LCD (try 0x27 or 0x3F)
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

// 🌐 HTML PAGE (MATCHES LCD VALUES)
String getHTML() {

  String status = "GOOD";
  String color = "#00c853";

  if (gas >= 1500) {
    status = "BAD";
    color = "#d50000";
  } 
  else if (gas >= 1350) {
    status = "MEDIUM";
    color = "#ff9800";
  }

  String page = "<!DOCTYPE html><html>";
  page += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<meta http-equiv='refresh' content='2'>";
  page += "<style>";
  page += "body{font-family:Arial;text-align:center;background:#111;color:white;}";
  page += ".card{padding:20px;margin:20px;border-radius:10px;}";
  page += ".big{font-size:28px;}";
  page += "</style></head><body>";

  page += "<h1>Air Monitoring System</h1>";

  page += "<div class='card' style='background:#222'>";
  page += "<p>Gas</p><p class='big'>" + String(gas) + "</p></div>";

  page += "<div class='card' style='background:#222'>";
  page += "<p>Temperature</p><p class='big'>" + String(temp) + " C</p></div>";

  page += "<div class='card' style='background:#222'>";
  page += "<p>Humidity</p><p class='big'>" + String(hum) + " %</p></div>";

  page += "<div class='card' style='background:" + color + "'>";
  page += "<p>Status</p><p class='big'>" + status + "</p></div>";

  if (status == "BAD") {
    page += "<div class='card' style='background:red'>";
    page += "<h2>⚠️ ALERT: BAD AIR QUALITY!</h2></div>";
  }

  page += "</body></html>";

  return page;
}

// 🌐 HANDLE ROOT
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {
  Serial.begin(115200);

  // I2C for ESP32
  Wire.begin(21, 22);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();

  //Air Monitor screen
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Air Monitor");
  delay(1500);
  lcd.clear();

  // 📶 WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // ✅ EXACT OUTPUT YOU WANTED
  Serial.println("\n======================");
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("======================");

  // 🔥 STOP SERIAL COMPLETELY HERE
  Serial.end();

  // Show IP on LCD briefly
  lcd.setCursor(0,0);
  lcd.print("IP Address:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();

  // Server routes
  server.on("/", handleRoot);
  server.begin();

  // Sensors
  dht.begin();

  // Pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);
}

void loop() {

  // 🔹 Read sensors
  gas = analogRead(MQ135_PIN);
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  // 🔹 Serial (matches LCD + Web)
//  Serial.print("Gas: ");
//  Serial.print(gas);
//  Serial.print(" | Temp: ");
//  Serial.print(temp);
//  Serial.print(" | Hum: ");
//  Serial.println(hum);

  String status = "GOOD";

  // 🔥 Logic
  if (gas < 1350) {
    status = "GOOD";
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(RELAY, LOW);
  }
  else if (gas < 1500) {
    status = "MEDIUM";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(RELAY, LOW);
  }
  else {
    status = "BAD";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(RELAY, HIGH);
  }

  // 📟 LCD (same values as web)
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

  // 🌐 Web server
  server.handleClient();

  delay(1000);
}