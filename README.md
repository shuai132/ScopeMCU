# ScopeMCU

[![Build Status](https://github.com/shuai132/ScopeMCU/workflows/build/badge.svg)](https://github.com/shuai132/ScopeMCU/actions?workflow=build)

Oscilloscope for MCU

MCU: STM32F103C8Tx

需配合[ScopeGUI](https://github.com/shuai132/ScopeGUI)使用

[截图和二进制文件](https://github.com/shuai132/ScopeGUI/wiki)

## Introduction

用最少的硬件成本，做一个实用的虚拟示波器。

这是硬件部分，基于STM32最小系统，不使用任何片外外设。
可以方便地移植到其他硬件（只需要ADC、定时器、串口/USB即可）。
采样率和采样深度等参数取决于芯片性能。

## Features

* Timer触发ADC+DMA 精确采样
* ADC数值已校准（使用内部基准电压）
* 采样率无级可调 最高采样率1Msps
* 采样深度可调 默认最大2048点
* 内置自测信号（1kHz方波信）
* 多种触发方式（自动、正常、单次）
* FFT频谱分析（上位机实现）
* USB CDC 高速数据传输

注: 
* 未设计前端运放电路，只用于采集0~3.3V信号。
* 为了采样精度和校准，当前最高采样率: 70ksps

## IO Map

### 必需

| 管脚 | 作用 |
| :---- | :---- |
PA0  | ADC通道
PA11 | USB DM   USB接口
PA12 | USB DP   USB接口

### 可选

| 管脚 | 作用 |
| :---- | :---- |
PC13 | LED指示灯 采样过程为低电平
PA8  | 1kHz方波信号 用于自测
PA9  | USART1 TX 调试串口
PA10 | USART1 RX 调试串口

## 移植

设计上把可移植性作为重要目标，可以很方便地移植到其他单片机、Arduino、DSP等硬件。

只需硬件提供指定功能实现函数即可，具体可参考[App/Scope.h](App/Scope.h)中public方法的注释或移植步骤。

注: 
* 需要C++11支持，推荐使用arm-none-eabi编译器和cmake构建，已在Windows、macOS、Ubuntu下测试通过。
* 为了方便软件设计和数据解析，直接打包结构体作为消息单元。注意要和上位机保持一致的字节序（arm、x86、arduino均为小端模式）。

### 移植步骤

* 初始化scope
```cpp
    auto& scope = Scope::getInstance();
    scope.setVolLimits(0, 3300);
    scope.setFsLimits(1, 10000);
    scope.setMaxSn(2048);
    scope.setMcuImpl(
            {
                    .sendData = [](uint8_t* data, size_t size) {
                        // 用串口发送数据
                    },
                    .startADC = []{
                        // 开始采样
                    },
                    .stopADC = []{
                        // 停止采样
                    },
                    .setSampleFs = [](uint32_t fs) {
                        // 设置采样率 返回实际的采样率
                    },
                    .onSampling = [](bool sampling) {
                        // 可用来控制指示灯状态
                    },
            });
```
* 当收到串口数据
```cpp
    Scope::getInstance().onRead(data, size);
```
* 当一次ADC转换完成  
```cpp
    Scope::getInstance().onADC(vol);
```
注：`startADC`之前不要触发`onADC`

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

## Usage
* [blog](https://blog.csdn.net/u012456479/article/details/101543344)

## Links

* [arm-none-eabi-gcc](https://developer.arm.com/tools-and-software/open-source-software)
