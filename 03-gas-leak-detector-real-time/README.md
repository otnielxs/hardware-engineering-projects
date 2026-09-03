# Real-Time IoT Gas Leak Detection & Automatic Response System

An Internet of Things (IoT) project focused on designing and implementing a real-time gas leakage detection system for laboratory environments, capable of triggering physical safety responses and remote notifications.

📌 Features
---

* **Dual-Gas Sensing:** Real-time monitoring of Carbon Monoxide (CO) and combustible gases (Butane/LPG)
* **Automatic Actuation:** Automatic activation of 12V exhaust fan upon threshold breach to clear gas concentration
* **On-Site Alarm:** Local visual (LED) and audible (3500 Hz passive buzzer) alert mechanisms
* **Cloud & Mobile Alerts:** Instant emergency messaging via Telegram Bot API and data logging to Adafruit IO dashboard

🛠️ Hardware Components
---

* **Microcontroller:** ESP32 DevKit V1
* **Gas Sensors:** MQ-7 (Carbon Monoxide) & MQ-2 (LPG/Butane)
* **Actuators:** 12V DC Fan + L298N Motor Driver
* **Indicators:** 3.5 kHz Passive Buzzer & Red LED
* **Power Supply:** 9.6V Power Adapter & Baseboard Setup

📁 Project Files
---

* code/ : Main ESP32 firmware source code
* report/ : Complete PBL Academic Report (PDF)
* media/ : Physical build photos and video demonstration
