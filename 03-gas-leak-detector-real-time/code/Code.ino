#include <WiFi.h>
#include <HTTPClient.h>
#include "AdafruitIO_WiFi.h"

// Definisi pin
#define MQ7_PIN 36
#define MQ2_PIN 39
#define LED_PIN 5
#define BUZZER_PIN 18
#define FAN_EN_PIN 23
#define FAN_IN1_PIN 19
#define FAN_IN2_PIN 21

// Ambang batas gas
const int MQ2_THRESHOLD = 1000;
const int MQ7_THRESHOLD = 3000;

// Informasi Wi-Fi dan Adafruit IO
#define WIFI_SSID "WIFI USERNAME" // Sesuaikan dengan Tethring
#define WIFI_PASS "WIFI PASSWORD" // Sesuaikan dengan Tethring 
#define AIO_USERNAME "USERNAME"
#define AIO_KEY "API KEY"

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *gas_feed_co = io.feed("gas_CO");
AdafruitIO_Feed *gas_feed_butana = io.feed("gas_butana");

// Konfigurasi Telegram Bot
const String botToken = "BOT TOKEN";  // Token bot Telegram 
const String chatIDs[] = {"USERID1", "USERID2", "USERID3"};  // Chat ID pengguna (mis Mahasiswa atau Dosen)
const int numChatIDs = sizeof(chatIDs) / sizeof(chatIDs[0]);

void setup() {
  Serial.begin(115200);

  // Konfigurasi pin
  pinMode(MQ7_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_EN_PIN, OUTPUT);
  pinMode(FAN_IN1_PIN, OUTPUT);
  pinMode(FAN_IN2_PIN, OUTPUT);

  // Pastikan buzzer dan LED mati pada awal
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Koneksi ke Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!"); 

  // Koneksi ke Adafruit IO
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to Adafruit IO!");
}

void loop() {
  // Membaca data sensor
  int mq7_value = analogRead(MQ7_PIN);
  int mq2_value = analogRead(MQ2_PIN);

  // Tampilkan nilai sensor di serial monitor
  Serial.print("CO: ");
  Serial.print(mq7_value);
  Serial.print(" | Butana: ");
  Serial.println(mq2_value);

  // Cek ambang batas gas dan kirim data hanya jika melebihi
  if (mq7_value > MQ7_THRESHOLD) {
    Serial.println("CO level above threshold! Sending to Adafruit IO...");
    gas_feed_co->save(mq7_value);
  }

  if (mq2_value > MQ2_THRESHOLD) {
    Serial.println("Butane level above threshold! Sending to Adafruit IO...");
    gas_feed_butana->save(mq2_value);
  }

  // Aktifkan sistem alarm jika ada gas melebihi ambang batas
  if (mq7_value > MQ7_THRESHOLD || mq2_value > MQ2_THRESHOLD) {
    // Menyalakan LED indikator
    digitalWrite(LED_PIN, HIGH);

    // Menyalakan buzzer pasif
    tone(BUZZER_PIN, 3500); // Frekuensi buzzer 3500 Hz
    activateFan(true);

    // Kirim notifikasi Telegram
    sendTelegramMessage();
  } else {
    // Mematikan LED dan buzzer
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN); // Matikan buzzer
    activateFan(false);
  }

  delay(7000); // Delay untuk pembacaan berikutnya
}

void activateFan(bool state) {
  if (state) {
    digitalWrite(FAN_EN_PIN, HIGH);
    digitalWrite(FAN_IN1_PIN, HIGH);
    digitalWrite(FAN_IN2_PIN, LOW);
  } else {
    digitalWrite(FAN_EN_PIN, LOW);
    digitalWrite(FAN_IN1_PIN, LOW);
    digitalWrite(FAN_IN2_PIN, LOW);
  }
}

void sendTelegramMessage() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String message = "Terjadi kebocoran gas di Laboratorium!";
    for (int i = 0; i < numChatIDs; i++) {
      String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatIDs[i] + "&text=" + URLEncode(message.c_str());
      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Message sent to Chat ID " + chatIDs[i]);
      } else {
        Serial.println("Error sending message to Chat ID " + chatIDs[i]);
      }
      http.end();
    }
  } else {
    Serial.println("WiFi not connected, unable to send Telegram message");
  }
}

// Fungsi untuk URL encoding
String URLEncode(const char* msg) {
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg) {
    if (isalnum(*msg) || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 15];
    }
    msg++;
  }
  return encodedMsg;
}
