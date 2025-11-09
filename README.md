# ✈️ ESP32 Flight Controller

Proyek ini adalah implementasi **Flight Controller berbasis ESP32** yang dirancang untuk mengontrol pesawat atau drone sederhana dengan input dari receiver **SBUS** dan output ke **ESC/servo** melalui sinyal **PWM 50 Hz**.  
Selain itu, sistem mendukung **telemetri UART** dan **sensor berbasis I²C** seperti MPU6050, barometer, atau magnetometer.

---

## ⚙️ Fitur Utama
- Kontrol **5 channel PWM output** untuk motor dan servo.
- Input **SBUS** (inverted UART, 100000 baud, 8E2).
- **UART Telemetry** untuk komunikasi dengan ground station.
- Antarmuka **I²C** untuk berbagai sensor penerbangan.
- Kompatibel dengan **PlatformIO** atau **Arduino framework**.

---

## 🧩 Konfigurasi Pin

| Fungsi               | Pin GPIO | Keterangan / Catatan                                   |
|----------------------|-----------|---------------------------------------------------------|
| **R_aileron (kanan)** | 13        | PWM output ke servo aileron kanan                      |
| **L_aileron (kiri)**  | 14        | PWM output ke servo aileron kiri                       |
| **Elevator**          | 25        | PWM output ke servo elevator                           |
| **Rudder**            | 26        | PWM output ke servo rudder                             |
| **Motor (ESC)**       | 27        | PWM output ke ESC utama                                |
| **UART TX**           | 17        | Telemetry output / komunikasi serial (Serial1 TX)      |
| **UART RX**           | 16        | Telemetry input / komunikasi serial (Serial1 RX)       |
| **I²C SDA**           | 21        | Jalur data sensor I²C                                  |
| **I²C SCL**           | 22        | Jalur clock sensor I²C                                 |
| **SBUS RX**           | 33        | Input SBUS (inverted UART, 100000 baud, 8E2)           |

---

## 🛰️ Konfigurasi Komunikasi Serial

| Tipe Komunikasi | Baudrate | Format Data | Catatan |
|-----------------|-----------|--------------|----------|
| **UART (telemetri)** | 115200 | 8N1 | Gunakan `Serial1` pada ESP32 |
| **SBUS (receiver)** | 100000 | 8E2 | Hanya RX, tidak perlu TX |

---

## 🧠 Rekomendasi Software

- **PlatformIO (VS Code Extension)** – Direkomendasikan untuk manajemen proyek.
- **Arduino Framework for ESP32** – Untuk kemudahan porting kode.
- **Serial Monitor** – Untuk melihat data telemetri (115200 baud).

---

## 🚀 Cara Clone dan Menjalankan Proyek

1. **Clone repository ini**
   ```bash
   git clone https://github.com/<username>/esp32-flight-controller.git
   cd esp32-flight-controller
