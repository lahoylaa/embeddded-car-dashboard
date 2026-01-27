# embeddded-car-dashboard

Note: This project is implemented using bare-metal C on top of CMSIS. No STM32 HAL or LL drivers were used. All peripheral configuraiton and interrupt handling are written manually using register-level access.

## Key Features
- Register-level peripheral configuration
- Interrupt-driven architecture
- I2C communication without HAL
- SPI communication without HAL

## Software Stack
- C (bare-metal)

## Key Source Files

## Concepts Demonstrated
- NVIC configuration
- Interrupt vector table
- Peripheral register mapping
- Memory-mapped I/O