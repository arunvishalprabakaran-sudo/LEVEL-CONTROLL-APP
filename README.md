# IoT Enabled Level Control System

An IoT-based automatic water tank monitoring and level control system built using the ESP32 microcontroller. The system continuously monitors the water level using an HC-SR04 ultrasonic sensor and automatically controls a water pump through a relay based on user-defined threshold values. Real-time monitoring and remote control are achieved through ThingSpeak, Firebase, and a custom web application. :contentReference[oaicite:0]{index=0}

---

## Features

- Real-time water level monitoring
- Automatic pump ON/OFF control
- User-defined low and high water level thresholds
- Manual pump control through web application
- Remote monitoring using Firebase and ThingSpeak
- Wi-Fi enabled ESP32-based system
- Prevents water overflow and dry running

---

## Hardware Components

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- 5V Relay Module
- Submersible Mini Water Pump
- BC547 Transistor
- Lithium-Ion Battery
- Breadboard and Jumper Wires :contentReference[oaicite:1]{index=1}

---

## Software & Technologies

- Arduino IDE
- ESP32
- Firebase Realtime Database
- ThingSpeak
- HTML / CSS / JavaScript (Web Dashboard)
- Wi-Fi Communication :contentReference[oaicite:2]{index=2}

---

## Working Principle

1. The HC-SR04 ultrasonic sensor measures the water level inside the tank.
2. ESP32 converts the measured distance into a water level percentage.
3. The controller retrieves threshold values from Firebase.
4. If the level falls below the low threshold, the pump turns ON automatically.
5. If the level exceeds the high threshold, the pump turns OFF.
6. Between the thresholds, users can manually control the pump through the web interface.
7. Water level and pump status are uploaded to ThingSpeak and synchronized with Firebase for remote monitoring. :contentReference[oaicite:3]{index=3}

---

## Project Architecture

```
HC-SR04 Ultrasonic Sensor
            │
            ▼
         ESP32 MCU
            │
     ┌──────┴──────┐
     ▼             ▼
Relay Module   Wi-Fi
     │             │
Water Pump   ThingSpeak
                  │
                  ▼
              Firebase
                  │
                  ▼
          Web Dashboard
```

---

## Applications

- Smart Water Tank Monitoring
- Home Automation
- Industrial Water Storage
- Smart Buildings
- IoT-based Utility Management

---

## Future Improvements

- Mobile application integration
- SMS/Email alerts
- Water consumption analytics
- Multiple tank monitoring
- AI-based water usage prediction

---

## Learning Outcomes

- Embedded Systems Design
- ESP32 Programming
- Sensor Interfacing
- Relay Control
- IoT Communication
- Firebase Integration
- ThingSpeak Cloud Platform
- Real-Time Monitoring
- Web Dashboard Development

---

## Repository Structure

```
├── Code/
│   └── ESP32 Source Code
├── Circuit/
│   └── Schematic Diagram
├── Images/
│   └── Hardware Setup
├── WebApp/
│   └── Dashboard Source
└── README.md
```

---

## Authors

- Arun Vishal P
- Shutharsan B
- Sibiraj S
- Aravind M
- Jagadeeswaran S :contentReference[oaicite:4]{index=4}
