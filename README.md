# STM32-Embedded-Projects.
Embedded C and STM32 firmware projects

# Smart Tracking System (RAAYAN MINI, STM32F401RBT6)

A GPS-based tracking device built on the RAAYAN MINI board (STM32F401RBT6, Cortex-M4). It reads location data from a GPS receiver, shows status on an LCD, takes a mobile number from a keypad, and is intended to send the tracking location as a Google Maps link.

> **Status: work in progress.** The GPS fix check is disabled, and the GSM/SMS output path is not active. See [Known Issues](#known-issues).

## Hardware

| Part | Details |
|---|---|
| Board | RAAYAN MINI, version 4.0 |
| MCU | STM32F401RBT6 (ARM Cortex-M4) |
| GPS receiver | NEO-6M (assumed from project notes) |
| GSM module | SIM800C (planned, not yet in `main.c`) |
| Display | Character LCD, driven by `km_lcd` |
| Input | Keypad, plus ENTER and UP switches |
| RTC / EEPROM | I2C device at address `0x68` (RTC) |

## Peripherals Used

| Peripheral | Purpose |
|---|---|
| USART1 + DMA2 Stream5 | GPS UART receive, DMA into `rx_buffer` |
| USART2 | Output of the tracking link (currently disabled) |
| I2C1 | RTC communication |
| SysTick | Millisecond delays (`KM_Delay_ms`) |
| GPIO | Keypad, switches, LCD |

## Project Structure

```
WSD_PROJECT/
├── main.c                    Application entry point and main loop
├── INC/                      Headers
│   ├── gps_extract.h         GPS (NMEA) parsing interface
│   ├── i2c.h                 I2C driver interface
│   ├── keypad.h              Keypad interface
│   ├── km_lcd.h              LCD interface
│   ├── switch.h              ENTER / UP switch interface
│   ├── systemticktimer.h     SysTick and delay interface
│   ├── uart2.h               USART2 interface
│   ├── dma.h                 DMA interface
│   └── stm32f401rbt6.h       Register and peripheral definitions
├── SRC/                      Sources
│   ├── gps_extract.c         NMEA RMC parsing, lat/long conversion
│   ├── keypad.c
│   ├── km_lcd.c
│   ├── rtc_eeprom.c
│   ├── switch.c
│   ├── systemticktimer.c
│   └── uart2.c
├── I2C_LIB/
│   └── RaayanMini_KM_HAL_I2C_Libraries.lib
├── RTE/Device/STM32F401RBTx/ Startup and system files (generated)
├── Objects/                  Build output (generated)
├── Listings/                 Build listings (generated)
├── KM_WDS.uvprojx            Keil MDK project file
├── KM_WDS.uvoptx             Keil project options
└── KM_WDS.uvguix.*           Keil debugger/window layout
```

## Toolchain

- **IDE:** Keil MDK-ARM v5 (ARM-ADS toolset, ARMCC V5.06 update 1)
- **Device pack:** Keil.STM32F4xx_DFP 2.14.0
- **Debugger:** ST-LINK/V3 (ST-LINKIII-KEIL_SWO driver)
- **Target clock setting in project:** 12 MHz (`CLOCK(12000000)`). The real system clock depends on the startup and clock configuration.

## Building

1. Open `KM_WDS.uvprojx` in Keil uVision 5.
2. Confirm the device is **STM32F401RBTx**.
3. Build with **Project → Build Target** (F7).
4. Confirm the `.lib` file in `I2C_LIB/` is found by the linker.

## Flashing and Debugging

1. Connect the ST-LINK to the board's SWD pins.
2. Choose **Flash → Download** (F8).
3. Start a debug session with **Debug → Start/Stop Debug Session** (Ctrl+F5).
4. Useful watch variables are listed in `KM_WDS.uvoptx`: `rx_buffer`, `latitude`, `longitude`, `lat_buff`, `log_buff`, `rmc_buff`, `map`.

## Operation

1. On power-up, the LCD shows `SMART TRACKING SYSTEM V1.0`, then prompts `ENTER SWTICH` and `UP SWTICH`.
2. **UP switch:** prompts `ENTER MOBILE NUMBER:` and starts keypad entry.
3. **ENTER switch:** reads the latest RMC sentence from the GPS, extracts latitude and longitude, and converts them to decimal degrees.
4. The LCD shows `INVALID` if the GPS data cannot be used.

## Known Issues

- **No GPS fix check.** The `status == 'A'` guard in `main.c` is commented out, so coordinates are used even without a valid fix.
- **Coordinates may be stale.** There is no check that a new RMC sentence has arrived since the last read.
- **No SMS output.** The Google Maps link is built into `map` and sent to `USART2_OutString`, but both lines are commented out. The `gsm_send()` call is also commented out, and no SIM800C code is in the project.
- **Mobile number not used.** The number entered on the keypad is not stored or sent anywhere.
- **RTC overwritten on every boot.** `today_date` is written to the RTC at startup, so the real time is lost after each power cycle.
- **Blocking delays.** `KM_Delay_ms` is used in the input handlers, which holds up the main loop.
- **Typos on the LCD:** `SWTICH`, `VAILD`, `HELP`.
- **Float printing.** `sprintf` with `%lf` requires float support in the Keil target settings.

## Roadmap

- [ ] Re-enable the GPS fix check (`status == 'A'`) before using coordinates.
- [ ] Add a "new data" flag so that stale RMC data is not used.
- [ ] Add SIM800C AT command handling (`AT`, `AT+CMGF=1`, `AT+CMGS`, Ctrl+Z).
- [ ] Store and use the entered mobile number.
- [ ] Set the RTC from GPS time, or only write it when it is not already running.
- [ ] Replace blocking delays with non-blocking timing.
- [ ] Fix LCD message typos.

## Notes

- `main.c` depends on functions defined in files that were not part of the review (`gps_extract.c`, `uart2.c`, `keypad.c`, `switch.c`). This README describes only what can be seen in the project files.
- Generated folders (`Objects/`, `Listings/`, `RTE/`) can be deleted and rebuilt.
