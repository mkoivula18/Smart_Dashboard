# Refresh Specification

## Environment

- ESP-IDF C++ project with LVGL 9.5 and a Waveshare 7.5-inch V2, 800×480, 1-bit e-ink panel.
- Relevant code: `main/Display.*`, `main/drivers/epd7in5_V2.*`, and `main/widgets/*`.
- LVGL I1 flush buffers begin with an 8-byte palette; transfer pixels begin after `Display::LVGL_I1_PALETTE_BYTES`.
- I1 rows are packed: `LV_DRAW_BUF_STRIDE_ALIGN == 1` is compile-time enforced.

## Full refresh

- Use at boot, for recovery, and after 20 completed partial refreshes.
- `Epd::Init()` initializes SPI once; `Epd::InitFullUpdate()` resets only the panel controller.
- LVGL renders the full 800×480 framebuffer in `LV_DISPLAY_RENDER_MODE_FULL`.
- `Epd::DisplayFrame()` transfers the full frame through `0x13`, inverting LVGL I1 bits, then sends `0x12`.
- Full refresh is slow and visible, but clears accumulated ghosting.

## Partial refresh

- Widget setters suppress their automatic label invalidation, change content, then invalidate the widget root.
- Widget roots use an opaque white background so shorter text overwrites old glyphs.
- LVGL expands every partial invalidation to `x = 0..799` while retaining the dirty Y range.
- This full-width band is a Waveshare V2 workaround for corrupt non-zero-X partial windows.
- `Epd::BeginPartialUpdate()` enters partial controller mode once per LVGL refresh cycle.
- `Epd::WriteRegion()` configures `0x90`, writes direct LVGL I1 bytes through `0x13`, and does not visibly refresh.
- `Epd::EndPartialUpdate()` sends `0x12` only after the last LVGL flush.
- Partial refresh uses different data polarity from full refresh; do not invert partial bytes.

## Safety rules

- `lv_area_t` coordinates are inclusive.
- The driver accepts only in-panel, non-empty, 8-pixel-aligned partial areas.
- Validate a partial area before opening the controller transaction.
- On a partial failure, schedule a safe full refresh rather than counting the cycle.
- Do not enlarge a region inside the EPD driver: LVGL has rendered only the supplied local buffer.

## Verification

- Run `ninja -C build` and `git diff --check`.
- Hardware smoke test: `XX:XX → 12:34`, then repeated minute changes.
- Test longer-to-shorter Wi-Fi and weather values.
- Confirm boot and threshold-triggered full refresh still work.
