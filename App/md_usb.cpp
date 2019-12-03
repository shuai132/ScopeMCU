#include "gpio.h"
#include "usbd_cdc_if.h"
#include "log.h"

/**
 * 软件复位后USB无法正常枚举 要模拟USB拔插
 */
void usb_plugged() {
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    /*Configure GPIO pin : PA12 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 先把PA12拉低再拉高 利用D+模拟USB的拔插动作
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(10);
}

void usb_cdcSend(uint8_t* data, size_t size, uint32_t timeoutMs) {
    timeoutMs += HAL_GetTick();
    while (USBD_OK != CDC_Transmit_FS(data, size)) {
        if (HAL_GetTick() >= timeoutMs) {
            LOGW("send timeout!");
            break;
        }
    }
}
