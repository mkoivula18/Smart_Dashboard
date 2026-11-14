# Partial Refresh Architecture

## Status

Implemented and manually validated on the Waveshare 7.5-inch V2 panel. Automated protocol tests remain future work.

## Refresh policy

| Situation | Path |
| --- | --- |
| Boot, recovery, or every 20 completed partial refreshes | Full refresh |
| Clock, weather, or Wi-Fi content change | Partial refresh |

`FULL_REFRESH_AFTER_PARTIAL_UPDATES` is deliberately conservative. Tune it only with repeated hardware tests for ghosting and contrast.

## Full refresh flow

```text
Widget state
  → LVGL renders the complete 800×480 I1 framebuffer
  → Epd::DisplayFrame(old, new)
  → controller commands 0x10, 0x13, 0x12
  → panel finishes a full refresh
```

`Epd::InitFullUpdate()` resets only the panel controller. SPI is initialized once by `Epd::Init()`, so full-refresh recovery must not call `Epd::Init()` again.

## Partial refresh flow

```text
Display::update_clock/weather/wifi
  → temporarily suppress the label's automatic invalidation
  → update widget content
  → invalidate the widget root
  → LVGL expands x to 0..799 and renders the dirty rows
  → flush_callback sends the area-local I1 pixels to Epd::WriteRegion()
  → after the final LVGL flush, Epd::EndPartialUpdate() sends 0x12
```

The widget root has an opaque white background. This replaces pixels from a longer previous value when the new text is shorter.

## Panel-specific constraints

- The Waveshare 7.5-inch V2 controller produced corrupt output for windows starting at a non-zero X coordinate. `expand_partial_refresh_area()` therefore forces every partial area to `x = 0..799` while preserving its Y range.
- `LV_COLOR_FORMAT_I1` data is byte aligned by LVGL. The driver validates that the partial area starts on an 8-pixel boundary and has a byte-aligned width.
- LVGL passes an area-local buffer, not a pointer into a full-screen framebuffer. The first `Display::LVGL_I1_PALETTE_BYTES` bytes are the I1 palette and are skipped before transfer. The build asserts packed I1 rows (`LV_DRAW_BUF_STRIDE_ALIGN == 1`) so a future stride change fails at compile time instead of scrambling pixels.
- This controller uses different data polarity in its full and partial paths. `DisplayFrame()` writes an inverted NEW frame to `0x13`; `WriteRegion()` writes the partial I1 bytes directly to `0x13`.

Do not expand a partial rectangle in `Epd::WriteRegion()`: the extra pixels would not exist in LVGL's local buffer. Expand it through the LVGL invalidation event before rendering instead.

## Ownership

| Layer | Responsibility |
| --- | --- |
| Widget | Owns content and an opaque root background. |
| Display | Selects full versus partial refresh, invalidates roots, and counts partial refreshes. |
| LVGL | Calculates and renders dirty areas. |
| EPD driver | Configures the panel and transfers already-rendered bytes. |

## Validation checklist

- After a cold boot, `XX:XX → 12:34` removes all old glyph pixels.
- A clock update refreshes only the clock's rows, without a full-panel flash.
- Wi-Fi and weather updates clear longer previous text.
- The first partial update after boot and after the 20-update full-refresh recovery is clean.
- Repeated partial updates remain legible at the intended operating temperature.

## Known scope

The full-width X workaround applies to every partial invalidation on this display. New animated, overlay, or high-frequency UI features must be measured against this cost before being enabled.

## Future work

- Make the full-refresh threshold configurable.
- Add serial diagnostics for partial areas and completed refresh counts.
- Add a hardware regression test for text shrinking and partial-refresh ghosting.
