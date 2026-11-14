# Refresh work delegation

For any task that changes, debugs, reviews, tests, or explains display refresh behavior, LVGL invalidation or flushing, e-ink controller commands, ghosting, pixel polarity, or the Waveshare 7.5-inch V2 driver:

1. Start a `refresh-expert` subagent using `.agents/skills/refresh-expert/SKILL.md` before implementation.
2. Give the subagent the refresh-specific investigation, design, or review work.
3. Keep final integration and user communication with the primary agent.

The primary agent may skip delegation only when the user explicitly asks not to use agents or when the task is a one-line status answer with no analysis or change.
