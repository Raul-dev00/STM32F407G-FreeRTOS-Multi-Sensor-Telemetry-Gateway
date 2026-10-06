## STM32F407G FreeRTOS Multi-Sensor Telemetry Gateway

A real-time telemetry gateway built on the STM32F407G Discovery board using FreeRTOS. This project demonstrates advanced embedded systems concepts, including asynchronous event handling via Queue Sets, task priority management, hardware-level random number generation, and safe memory management.

## Project Architecture & Features

The system simulates an embedded telemetry architecture (similar to UAV or industrial motor monitoring systems) where multiple sensor tasks run at different frequencies and feed data into an asynchronous central receiver.
```text
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
```

## Task Breakdown & Priorities
1. Battery Sender Task (Priority 1 - 1000ms periodicity):
  *Generates simulated battery voltage levels (11.4V - 12.6V) using the STM32 Hardware Random Number Generator (RNG).
  *Passes data via Pass-by-Value into Queue 1 and toggles the Green LED (PD12).
2. Motor Sender Task (Priority 1 - 500ms periodicity):
  *Generates simulated motor temperature ($80^\circ\text{C}$ – $120^\circ\text{C}$) and RPM (7000 - 8000).
  *Passes data into Queue 2 and toggles the Orange LED (PD13).
3. Error / Emergency Task (Priority 3 - High Urgency):
  *Monitors the on-board Blue Push-Button (PA0).
  *When pressed, triggers an emergency event packet into Queue 3, lighting up the Red LED (PD14).
4. Telemetry Gateway / Receiver Task (Priority 2):
  *Utilizes FreeRTOS Queue Sets (xQueueSelectFromSet) to block efficiently (portMAX_DELAY) and monitor all three queues simultaneously without CPU polling.
  *Dynamically formats data strings and transmits them safely via UART using calculated payload lengths (msg_len).
