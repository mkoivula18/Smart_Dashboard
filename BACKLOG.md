# Smart Dashboard Backlog

### Partial refresh: clock slice

- Update only the clock region on a minute change.
- Verify no full-screen flash on hardware.


### Partial refresh: complete the pipeline

- Add clipping and 8-pixel byte alignment.
- Add periodic full refresh to control ghosting.

### Nysse departures widget

- Show upcoming departures for selected stops.
- Handle empty, stale, and failed responses.

### Weather forecast widget

- Show a forecast for the next few days.
- Include temperature and weather symbols.

### Date display

- Show the current date next to the clock.

### Data refresh and failures

- Define update intervals for clock, weather, and Nysse.
- Handle network and API failures gracefully.

### Dashboard UI polish

- Improve glanceability and e-ink typography.
- Handle long text and layout edge cases.

## Quality

### CI before master

- Build the ESP-IDF firmware in every pull request.
- Require a passing CI check before merging to `master`.

### Testable dashboard logic

- Separate dashboard state from platform code.
- Add unit tests for formatting and update decisions.

### Hardware reliability

- Test startup, API failures, and WiFi loss.
- Test ghosting and multi-day operation.

## Later

### Qt E-Ink Dashboard Simulator

- Preview the LVGL 800×480 I1 framebuffer on a PC.
- Edit test data and inspect dirty regions.

### AI summary field

- Add only if it has clear daily value.

### MQTT and home integrations

- Revisit when event-driven device data is needed.

