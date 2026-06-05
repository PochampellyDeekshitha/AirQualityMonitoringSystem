🌫️ Air Quality Monitoring System using ESP32

An IoT-based Smart Air Quality Monitoring and Automated Ventilation System developed using ESP32. The system continuously monitors air quality, temperature, and humidity, provides real-time alerts, automatically controls ventilation, and enables remote monitoring through a web dashboard, Telegram notifications, and ThingSpeak cloud integration.


📖 Overview

Air pollution has become a major concern in homes, offices, hospitals, and industrial environments. This project provides a low-cost and intelligent solution for monitoring indoor air quality and automatically improving environmental conditions when harmful gas levels are detected.

The system uses an MQ135 Gas Sensor to detect pollutants and a DHT11 Sensor to measure temperature and humidity. The collected data is processed by an ESP32 microcontroller and displayed on a 16×2 LCD display. When poor air quality is detected, the system activates alerts and automatically turns ON a fan through a relay module.

Additionally, the ESP32 provides IoT capabilities such as:

🌐 Web-based monitoring
📲 Telegram alert notifications
☁️ ThingSpeak cloud integration
🚀 Features
Real-time air quality monitoring
Temperature and humidity monitoring
LCD display for live sensor readings
Air quality classification:
✅ GOOD
⚠️ MEDIUM
🚨 BAD
LED status indicators
Buzzer alerts for hazardous conditions
Automatic fan control using relay module
Web dashboard using ESP32 IP address
Telegram notifications for poor air quality
ThingSpeak cloud data logging and visualization
Low-cost and energy-efficient design


🛠️ Hardware Components
Component	Purpose
ESP32 - Main controller and Wi-Fi communication
MQ135 Gas Sensor - Air quality monitoring
DHT11 Sensor - 	Temperature and humidity measurement
16×2 LCD (I2C) - Real-time data display
Relay Module - Fan control
DC Fan - Automatic ventilation
LEDs (Green, Yellow, Red) - Air quality indication
Buzzer - Audio alert
Breadboard & Jumper Wires - Circuit connections
Power Supply - System operation


🔌 Circuit Connections
Component	ESP32 Pin
MQ135 Sensor	GPIO34
DHT11 Sensor	GPIO4
LCD SDA	GPIO21
LCD SCL	GPIO22
Green LED	GPIO18
Yellow LED	GPIO19
Red LED	GPIO23
Buzzer	GPIO5
Relay Module	GPIO25


⚙️ Software Requirements
Arduino IDE
ESP32 Board Package
Required Libraries
WiFi.h
WebServer.h
Wire.h
LiquidCrystal_I2C.h
DHT.h
HTTPClient.h
WiFiClientSecure.h
UniversalTelegramBot.h
ArduinoJson.h


📊 Working Principle
MQ135 continuously detects harmful gases.
DHT11 measures temperature and humidity.
ESP32 processes sensor readings.
Air quality is classified as GOOD, MEDIUM, or BAD.
Values are displayed on LCD and web dashboard.
If air quality is poor:
Red LED turns ON
Buzzer sounds
Relay activates DC fan
Telegram alert is sent
Sensor data is uploaded to ThingSpeak cloud.
The process repeats continuously for real-time monitoring.


🌐 IoT Features
Web Dashboard

Users can access real-time environmental data using the ESP32 IP address through a web browser.

Telegram Alerts

Instant notifications are sent whenever air quality reaches hazardous levels.

Example:

⚠️ ALERT!
Air Quality is BAD

Gas: 1650
Temp: 31°C
Humidity: 68%
ThingSpeak Cloud Integration

Sensor data is uploaded to ThingSpeak for:

Data storage
Graphical visualization
Historical analysis
Remote monitoring

📈 System Outputs
LCD Display
Gas: 1450 MEDIUM
T: 30°C H: 65%
LED Status
Air Quality	LED
GOOD	Green
MEDIUM	Yellow
BAD	Red
Fan Control
Air Quality	Fan Status
GOOD	OFF
MEDIUM	OFF
BAD	ON

🎯 Applications
Smart Homes
Offices
Hospitals
Classrooms
Laboratories
Industrial Monitoring
Indoor Environmental Monitoring


🔮 Future Enhancements
Mobile Application Integration
Multi-Gas Detection
AI-Based Air Quality Prediction
Outdoor Monitoring Support
Energy Optimization
Smart Home Integration


📚 Technologies Used
ESP32
Arduino IDE
IoT
ThingSpeak Cloud
Telegram Bot API
Embedded Systems
Sensor Networks
