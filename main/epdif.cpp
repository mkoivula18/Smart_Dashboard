/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements C++ conversions of original Arduino driver functions
 *  @author     :   Mikko Koivula
*/

#include "epdif.hpp"
// #include <SPI.h>

// #include "epdif.hpp"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_check.h"



spi_device_handle_t spi_device = nullptr;

void EpdIf::DigitalWrite(int pin, int value)
{
    gpio_set_level((gpio_num_t)pin, value ? 1 : 0); // PROVIDED BY "driver/gpio.h"
}

int EpdIf::DigitalRead(int pin)
{
    return gpio_get_level((gpio_num_t)pin);
}

void EpdIf::DelayMs(unsigned int delaytime)
{
    vTaskDelay(pdMS_TO_TICKS(delaytime));
}

void EpdIf::SpiTransfer(unsigned char data)
{
    if (spi_device == nullptr) {
        printf("SpiTransfer: spi_device is null\n");
        return;
    }

    spi_transaction_t transaction{};
    transaction.tx_buffer = &data;
    transaction.length = 8;

    // printf("SpiTransfer: 0x%02X\n", data);

    DigitalWrite(CS_PIN, LOW);
    spi_device_transmit(spi_device, &transaction);
    DigitalWrite(CS_PIN, HIGH);
}

int EpdIf::IfInit(void)
{
    // Output from ESP32 to the E-Ink screen
    gpio_config_t config_output{};
    gpio_config_t config_input{};

    uint64_t output_pin_mask = (1U << CS_PIN) | (1U << RST_PIN) | (1U << DC_PIN) | (1U << PWR_PIN);
    uint64_t input_pin_mask = (1U << BUSY_PIN);

    setupGpioConfig(&config_output, output_pin_mask, GPIO_MODE_OUTPUT);
    setupGpioConfig(&config_input, input_pin_mask, GPIO_MODE_INPUT);

    printf("config_output mask: 0x%llx\n", config_output.pin_bit_mask);
    printf("config_input mask: 0x%llx\n", config_input.pin_bit_mask);

    DigitalWrite(PWR_PIN, HIGH);
    DigitalWrite(CS_PIN, HIGH);

    spi_bus_config_t config_bus{};

    config_bus.mosi_io_num = MOSI_PIN;
    config_bus.miso_io_num = -1;        // E-Ink doesn't send data back
    config_bus.sclk_io_num = SCK_PIN;
    config_bus.quadhd_io_num = -1;      // Quad pins are unused (?)
    config_bus.quadwp_io_num = -1;      // Quad pins are unused (?)
    config_bus.max_transfer_sz = 4;

    spi_device_interface_config_t config_device{};
    config_device.clock_speed_hz = 2 * 1000 * 1000;     // 2 MHz
    config_device.mode = 0;                             // SPI MODE 0
    config_device.spics_io_num = -1;                    // CS handled manually
    config_device.queue_size = 1;                       // One transaction at a time

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &config_bus, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &config_device, &spi_device));

    // pinMode(PWR_PIN, OUTPUT);
    // DigitalWrite(PWR_PIN, 1);

    // SPI.begin();
    // SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    return 0;
}

void EpdIf::setupGpioConfig(gpio_config_t *config, uint64_t pin_mask, gpio_mode_t mode)
{
    config->pin_bit_mask = pin_mask;

    // Defines that the pins act as outputs
    config->mode = mode;

    // Basically, puts the pins in clean electrical state to avoid weird issues
    config->pull_down_en = GPIO_PULLDOWN_DISABLE;
    config->pull_up_en = GPIO_PULLUP_DISABLE;
    config->intr_type = GPIO_INTR_DISABLE;

    ESP_ERROR_CHECK(gpio_config(config));
}
