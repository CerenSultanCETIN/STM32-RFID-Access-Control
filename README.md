# 🔐 STM32 & C# Full-Stack Access Control System

## 🚀 Project Overview
This project represents a **Full-Stack Embedded System** solution that bridges the gap between low-level hardware control and high-level software management.

It is designed to demonstrate **end-to-end engineering skills** by integrating an **STM32 microcontroller** (handling sensors and real-time logic) with a sophisticated **C# Desktop Application** (handling database management and user interface).

## 🛠️ Tech Stack & Tools

### 🟢 Firmware (Embedded Side)
* **Microcontroller:** STM32F4 Series (ARM Cortex-M4)
* **Language:** C (HAL Library)
* **IDE:** STM32CubeIDE
* **Modules:** * RC522 RFID Reader (SPI Protocol)
    * 4x4 Matrix Keypad (GPIO Scanning)
    * DS1302 Real-Time Clock
* **Communication:** UART/USART (Serial Communication with PC)

### 🔵 Software (Desktop Side)
* **Platform:** Windows Desktop Application
* **Language:** C# (.NET Framework)
* **IDE:** Visual Studio
* **Architecture:** Event-Driven Architecture with Serial Port Listener
* **Features:** * Real-time data parsing (Hex/ASCII)
    * User Authentication & Log Management
    * File I/O (Database simulation)

## ⚙️ How It Works (Full-Stack Flow)
1.  **Hardware Layer:** The STM32 continuously scans for RFID cards and Keypad inputs using non-blocking timers.
2.  **Protocol Layer:** Valid data is packetized and transmitted via **UART** to the computer.
3.  **Application Layer:** The C# application acts as a "Router", listening to the COM Port. It parses the incoming data, verifies credentials against the local database, and logs the entry/exit activity.

## 👨‍💻 About the Developer
Developed by **Ceren Sultan CETIN**, a Software Engineering student and Embedded Systems Intern passionate about bridging hardware and software.
