# STM32_TIMER_ADC

MCU: STM32F103C8Tx

## DevelopTools

* STM32CubeMX
* arm-none-eabi-gcc
* CLion(CMake)
* OpenOCD
* ST-LINK

## Install Requirements(for macOS)

* config OpenOCD and ST-LINK
```bash
brew install openocd
brew install stlink
brew cask install gcc-arm-embedded
```

* add environment variable
```bash
PATH=$PATH:~/Library/STM32/gcc-arm-none-eabi-8-2018-q4-major/bin
```

## Write to flash

```bash
st-flash write *.bin 0x8000000
```

## Links

* [arm-none-eabi-gcc](https://developer.arm.com/tools-and-software/open-source-software)
