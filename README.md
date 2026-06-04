# Mega Drive 3-Button Controller → USB HID Adapter

> Plug your old Sega Mega Drive / Genesis controller into a PC via USB — no drivers, no nonsense.

![Controller plugged into the adapter](docs/adapter_photo.jpg)

---

## What is this?

This project turns an Arduino Micro into a USB HID gamepad adapter for the classic **Sega Mega Drive / Genesis 3-button controller** (DB9 connector).

The controller plugs straight into the Arduino via a DB9 female socket wired to the digital pins. The Arduino enumerates as a standard USB gamepad — Windows, Linux, and macOS all recognise it instantly.

The whole thing is, frankly, disarmingly simple. There's no custom USB descriptor wizardry, no FPGA, no kernel module. It's a handful of wires, a DB9 connector, an Arduino Micro, and about 100 lines of C++. The Mega Drive controller protocol dates back to 1988 and could hardly be more straightforward: pull SELECT low, read four lines, pull SELECT high, read four more. That's it. The Arduino `Joystick` library handles the USB HID side. The result is a lag-free, plug-and-play adapter you can build in an afternoon.

---

## Hardware

### Parts

| Part | Notes |
|------|-------|
| Arduino Micro | Must be a **Micro** (ATmega32U4 with native USB) — Uno won't work |
| DB9 female socket | Panel-mount or PCB type |
| Jumper wires | 7 signal + 1 GND + 1 VCC |

### DB9 → Arduino Micro wiring

The Mega Drive DB9 female pinout (looking at the socket from the front):

```
  ___________
 \ 1 2 3 4 5 /
  \ 6 7 8 9 /
   ---------
```

| DB9 Pin | Signal     | Arduino Pin |
|---------|------------|-------------|
| 1       | UP         | D5          |
| 2       | DOWN       | D4          |
| 3       | LEFT       | D3          |
| 4       | RIGHT      | D2          |
| 5       | VCC (+5V)  | 5V          |
| 6       | B / A      | D8          |
| 7       | SELECT     | D7          |
| 8       | GND        | GND         |
| 9       | C / START  | D6          |

All signal pins use the Arduino's internal pull-ups. No external resistors needed.

### Button mapping

| Controller button | USB HID button index |
|-------------------|---------------------|
| UP                | 0                   |
| DOWN              | 1                   |
| LEFT              | 2                   |
| RIGHT             | 3                   |
| C                 | 4                   |
| B                 | 5                   |
| A                 | 6                   |
| START             | 7                   |

If no controller is detected (LEFT or RIGHT lines are floating), all buttons are released and the adapter stays silent.

---

## Software

This project is built with **Visual Studio Code** and the **PlatformIO** extension. No Arduino IDE required.

### Prerequisites

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode) from the VS Code marketplace
3. PlatformIO will handle the toolchain and library installation automatically

### Build & flash

1. Clone this repository and open the folder in VS Code
2. PlatformIO detects `platformio.ini` and prompts you to install dependencies
3. Connect the Arduino Micro via USB
4. Click **Upload** (→) in the PlatformIO toolbar, or run:

```
pio run --target upload
```

The `ArduinoJoystickLibrary` dependency is declared in `platformio.ini` and fetched automatically — no manual library install needed.

### Serial debug output

Open the PlatformIO serial monitor at **115200 baud** to see live button states:

```
UP:0 DN:0 LF:0 RT:1 A:0 B:1 C:0 ST:0
```

---

## `platformio.ini`

```ini
[env:micro]
platform  = atmelavr
board     = micro
framework = arduino
lib_deps  = MHeironimus/ArduinoJoystickLibrary @ ^2.1.1
```
## License

MIT
