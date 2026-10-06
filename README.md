## STM32F407G FreeRTOS Multi-Sensor Telemetry Gateway

A real-time telemetry gateway built on the STM32F407G Discovery board using FreeRTOS. This project demonstrates advanced embedded systems concepts, including asynchronous event handling via Queue Sets, task priority management, hardware-level random number generation, and safe memory management.

## Project Architecture & Features

The system simulates an embedded telemetry architecture (similar to UAV or industrial motor monitoring systems) where multiple sensor tasks run at different frequencies and feed data into an asynchronous central receiver.

┌──────────────────────┐        ┌──────────────────┐
│ Battery Sender (1Hz) │───────>│                  │
└──────────────────────┘        │                  │
                                │   FreeRTOS       │
┌──────────────────────┐        │   Queue Set      │───> [Receiver Task] ───> UART Terminal
│ Motor Sender (0.5Hz) │───────>│ (xQueueSelect)   │      (Priority 2)        (9600 Baud)
└──────────────────────┘        │                  │
                                │                  │
┌──────────────────────┐        │                  │
│ Error / Button (PA0) │───────>│                  │
└──────────────────────┘        └──────────────────┘
