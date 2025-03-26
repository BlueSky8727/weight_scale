# 💡 Project M.1 - Smart Weight Scale (ESP32)

โปรเจคนี้คือ **เครื่องชั่งน้ำหนักอัจฉริยะ (Smart Weight Scale)** โดยใช้บอร์ด **ESP32** และโมดูล **HX711 + Load Cell** พร้อมรองรับการส่งข้อมูลผ่าน MQTT และ Bluetooth ไปยังแอปมือถือที่พัฒนาด้วย Flutter

---

## 📦 โครงสร้างโปรเจค

Project_M1_1/
├── .vscode/            # การตั้งค่า VSCode
├── .pio/               # โฟลเดอร์ build จาก PlatformIO
├── include/            # Header files (หากมี)
├── lib/                # ไลบรารีที่ใช้ (เช่น HX711, OLED, ฯลฯ)
├── src/                # โค้ดหลักของโปรเจค (main.cpp)
├── test/               # ไฟล์ทดสอบ (unit test)
├── platformio.ini      # คอนฟิกของ PlatformIO

---

## ⚙️ คุณสมบัติของระบบ

- ใช้ **ESP32** เชื่อมต่อ **Load Cell ผ่าน HX711**
- คำนวณและส่งน้ำหนักผ่าน:
  - MQTT (Topic: `67098633/weight`)
  - Bluetooth (BLE)
- รองรับการเชื่อมต่อกับ **แอปมือถือ Flutter**
- รองรับการใช้งานกับจอ OLED (ผ่าน I2C)
- สามารถคำนวณ BMI และแสดงประวัติน้ำหนักย้อนหลังได้ (ฝั่งแอป)

---

## 🚀 การติดตั้งและใช้งาน

### 1. ติดตั้ง PlatformIO บน VSCode  
👉 [https://platformio.org/install](https://platformio.org/install)

### 2. เปิดโปรเจคนี้ใน VSCode และ Build/Upload

```bash
> pio run --target upload

3. เชื่อมต่อกับ MQTT Broker
	•	Broker: broker.emqx.io
	•	Topic: 67098633/weight


📱 แอปพลิเคชันมือถือ (Flutter)

รองรับการแสดงน้ำหนักจาก ESP32 ผ่าน MQTT และ BLE

🧠 ผู้พัฒนา

Thanakrit Ponsing (Sky)
📌 วิศวกรรมคอมพิวเตอร์ มหาวิทยาลัยศรีปทุม
💡 สนใจด้าน AI, IoT, Embedded Systems
