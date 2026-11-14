---
name: refresh-expert
description: Expert workflow for this ESP-IDF and LVGL e-ink dashboard's full and partial refresh behavior. Use whenever a request implements, changes, debugs, tests, reviews, or explains display refreshes, LVGL invalidation or flushing, e-paper controller commands, ghosting, pixel polarity, refresh performance, or the Waveshare 7.5-inch V2 display driver.
---

# Refresh Expert

Start responses with `Mode: Refresh Expert`.

Read [refresh-spec.md](references/refresh-spec.md) before changing refresh behavior.

## Workflow

1. Inspect the relevant `Display`, widget, LVGL flush, and EPD-driver code before proposing a change.
2. Keep full refresh and partial refresh as separate protocols.
3. Treat `flush_callback(area, px_map)` as the LVGL-to-panel boundary. `px_map` is area-local in partial mode.
4. Preserve the V2 partial-window workaround: expand through LVGL invalidation before render, never by expanding a driver area after render.
5. Preserve opaque widget backgrounds for partial updates that must erase old content.
6. Verify with `ninja -C build` and `git diff --check`. State clearly when only hardware can validate a change.

## Guardrails

- Do not reuse full-refresh polarity or command assumptions in partial refresh without hardware evidence.
- Do not call `Epd::Init()` for routine full refresh recovery; it initializes SPI.
- Do not make an unvalidated partial-region failure silently visible. Prefer a safe full-refresh fallback.
- Do not alter LVGL I1 stride assumptions without updating the transport API and tests.
- Keep the full-width partial workaround explicit and document its energy/performance cost.

## Scope

Focus on correctness and maintainability of the refresh pipeline. Escalate unrelated widget layout, network, or application-data work to the appropriate owner after identifying any refresh impact.
