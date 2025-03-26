#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"

// Wi-Fi config
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT config
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "67098633/weight";

// HX711
#define DT_PIN 4
#define SCK_PIN 5
HX711 scale;

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// 🧪 Calibration Factor สำหรับ Wokwi
float CALIBRATION_FACTOR = 420.0;

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  String clientId = "esp32-weight-";
  clientId += String(random(0xffff), HEX);

  while (!client.connected()) {
    Serial.print("🔗 Connecting to MQTT...");
    if (client.connect(clientId.c_str())) {
      Serial.println("✅ MQTT Connected");
    } else {
      Serial.print("❌ Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("❌ SSD1306 allocation failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Starting...");
  display.display();

  // HX711
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();
  Serial.println("⚖️ Load Cell Ready");

  // Wi-Fi & MQTT
  connectWiFi();
  connectMQTT();
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  if (scale.is_ready()) {
    float weight = scale.get_units(10);  // อ่านค่าเฉลี่ย 10 ครั้ง
    if (weight < 0) weight = 0.00;
    weight = round(weight * 100.0) / 100.0;

    // แสดงบน OLED ทุกครั้ง
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.printf("%.2f kg", weight);
    display.display();

    // ส่ง MQTT ทุก 1 วินาที
    char payload[10];
    dtostrf(weight, 4, 2, payload);
    client.publish(mqtt_topic, payload);
    Serial.printf("📦 Weight: %.2f kg\n", weight);
    Serial.printf("📤 MQTT Published: %s\n", payload);

  } else {
    Serial.println("⚠️ HX711 not ready");
  }

  delay(1000);  // ส่งทุก 1 วิ
}