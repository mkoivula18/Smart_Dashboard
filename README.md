# Smart Dashboard

ESP32 + E-ink smart screen project

> Very much WIP..

## Preview

Photo here:

```md
PHOTO HERE SOMEHOW, FIGURE IT OUT
```

## Features

- Custom display driver
- LVGL
- Feature 3
- Feature 4
- Feature 5

## Hardware

| Part | Notes |
| --- | --- |
| MCU | Firebeetle 2 ESP32-C6 |
| Display | Waveshare 7.5 inch e-ink |
| Power | USB |
| Other | TBA |
|

## LVGL Configuration

This project uses the ESP-IDF managed LVGL component.

LVGL configuration done through ESP-IDF Kconfig settings `sdkconfig` / `menuconfig`:

Configure everything LVGL related here

```bash
idf.py menuconfig

-> Component config -> LVGL configuration
```

