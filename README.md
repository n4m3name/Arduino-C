Assignment 4 of CSC230 - Intro to Computer Architecture at UVic, written in C

A program for controlling LED patterns and animations on an embedded system using timer interrupts and PWM.

## Features

- LED state control with multiple patterns
- SOS signal implementation
- PWM-based LED brightness control
- Custom light show sequences
- Timer-based interrupt handling

## Core Components

**LED Control Functions**
- `led_state()`: Controls individual LED states
- `SOS()`: Implements SOS pattern sequence
- `glow()`: PWM-based LED brightness control
- `light_show()`: Custom LED animation sequence

## Technical Implementation

**Timer Configuration**
```c
#define F_CPU 16000000UL
#define PRESCALE_DIV1 8
#define PRESCALE_DIV3 64
```

**PWM Settings**
```c
#define PWM_PERIOD ((long int)500)
#define DELAY1 0.000001
#define DELAY3 0.01
```

## LED Patterns

**SOS Signal**
- Short blinks (S): 3 quick flashes
- Long blinks (O): 3 extended flashes
- Short blinks (S): 3 quick flashes
- Appropriate timing delays between sequences

**Light Show**
- Sequential LED patterns
- Variable duration timings
- Multiple LED combinations
- Brightness transitions

## Hardware Requirements

- ATmega2560 microcontroller
- 4 LEDs connected to PORTL
- 16MHz clock frequency
- Appropriate power supply

## Dependencies

- AVR standard libraries
- Timer interrupt capability
- PWM hardware support

## Author

Evan Strasdin

Based on code provided by Mike Zastre (2022-Nov-22)
