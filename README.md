# Embedded Car Dashboard (STM32, Bare-Metal)

> **Bare-metal STM32 automotive dashboard implemented using CMSIS register-level programming.**

![Embedded Car Dashboard Enclosure](Docs/photos/front-enclosure.jpeg)

This project implements an embedded system that replicates an automotive dashboard using two STM32 microcontrollers communicating over I2C.  
All peripheral configuration, interrupt handling, and communication protocols are implemented manually using register-level access — no STM32 HAL or LL drivers are used.

---

## System Overview

- **Master STM32 (Dashboard Controller)**
  - Handles UI rendering on a TFT display
  - Processes user input (rotary encoder / buttons)
  - Manages application logic and state
  - Acts as the I2C master

- **Slave STM32 (Peripheral Controller)**
  - Drives a seven-segment display used as a speedometer
  - Controls a reverse flashing LED indicator
  - Acts as an I2C slave responding to master commands

This dual-MCU architecture offloads timing-critical display and LED control tasks from the main dashboard controller, resulting in a modular and scalable embedded design.

---

## Key Features

- Bare-metal C firmware (no HAL / no LL)
- Register-level peripheral configuration
- Interrupt-driven architecture
- SPI communication without HAL (TFT display)
- I2C master/slave communication without HAL
- Cross-microcontroller system design

---

## Concepts Demonstrated

- CMSIS-based register access
- NVIC configuration and interrupt handling
- Interrupt vector table and startup code
- Memory-mapped I/O
- Timer-driven peripheral control
- Embedded communication protocol design

---

## Software Stack

- **Language:** C (bare-metal)
- **Libraries:** CMSIS (core and device headers only)

---

## Notes

- All source files were designed and implemented by the author **except**:
  - `font_freemono_mono_bold_24`
  - `ili9341` display driver files

These files are included for display support and font rendering.
