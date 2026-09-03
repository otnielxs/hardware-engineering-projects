#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Inisialisasi DFPlayer Mini via SoftwareSerial
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Pin dan Parameter Pulse Sensor
const int PulseWire = A0;
const int LED13 = 13;
int Threshold = 550;
PulseSensorPlayground pulseSensor;

// Inisialisasi Sensor Suhu Inframerah
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int currentFile = 1;

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Gagal terhubung ke DFPlayer Mini!");
    while (true);
  }
  Serial.println("DFPlayer Mini Siap.");
  myDFPlayer.volume(30);
  myDFPlayer.play(currentFile);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);

  if (!pulseSensor.begin()) {
    Serial.println("Gagal inisialisasi PulseSensor.");
    while (1);
  }

  if (!mlx.begin()) {
    Serial.println("Gagal terhubung ke sensor MLX90614.");
    while (1);
  }
}

void playFile(int fileNumber) {
  myDFPlayer.play(fileNumber);
  Serial.print("Memutar MP3 file ke-");
  Serial.println(fileNumber);
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();
  double objectTempC = mlx.readObjectTempC();

  // Logika Evaluasi Kesehatan berdasarkan Suhu & BPM
  if (objectTempC >= 33 && objectTempC <= 37 && myBPM >= 50 && myBPM <= 90 && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(4);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(3);
    delay(6000);
    lcd.clear();
  } else if (objectTempC >= 33 && objectTempC <= 37 && (myBPM < 50 || myBPM > 90) && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(4);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(2);
    delay(6000);
    lcd.clear();
  } else if (objectTempC > 37 && myBPM >= 50 && myBPM <= 90 && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(5);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(3);
    delay(6000);
    lcd.clear();
  } else if (objectTempC > 37 && myBPM < 50 && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(5);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(1);
    delay(6000);
    lcd.clear();
  } else if (objectTempC > 37 && myBPM > 90) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(5);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(2);
    delay(6000);
    lcd.clear();
  } else if (objectTempC < 33 && myBPM >= 50 && myBPM <= 90 && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(6);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(3);
    delay(6000);
    lcd.clear();
  } else if (objectTempC < 33 && myBPM < 50 && myBPM > 20) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(6);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(1);
    delay(6000);
    lcd.clear();
  } else if (objectTempC < 33 && myBPM > 90) {
    lcd.print("Suhu kamu ");
    lcd.print(objectTempC);
    playFile(6);
    delay(6000);
    lcd.clear();
    
    lcd.print("Denyut kamu ");
    lcd.print(myBPM);
    playFile(2);
    delay(6000);
    lcd.clear();
  } else if (objectTempC > 15 && myBPM == 0) {
    lcd.print("Silahkan cek ");
    playFile(8);
    delay(3000);
    lcd.clear();
    lcd.print("Silahkan cek ");
    playFile(8);
    delay(3000);
    lcd.clear();
  }

  delay(4000);
}
