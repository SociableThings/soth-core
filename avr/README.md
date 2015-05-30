# soth-core-avr

---

## Specification

### MCU
- Type name: ATXMEGA32D4-AU
- Foot print: TQFP44
- Spec
  - 32kB Flash
  - 2kB EEPROM
  - 4kB SRAM
- Voltage: 3.3V
- Xtal: 1MHz

### GPS
- Type name: LS20031(LS23060)
- Voltage: 3.3V
- Update rate: 1 sample/s (default: 5 sample/s)
- Baud rate: 9600bps (default: 57600bps)
- Software UART
  - Rx: PC6
  - Tx: PC7

### Communication
- Baud rate: 115200bps
- Hardware UART
  - USARTC

### Command servo
- Type name: RS304MD
- Motor voltage: 5.0V
- Signal voltage: 3.3V
- Baud rate: 115200bps
- Hardware UART
  - USARTD
- Servo ID
  - 1: Front right
  - 2: Front left
  - 3: Back right
  - 4: Back left
  - 5: Yaw
