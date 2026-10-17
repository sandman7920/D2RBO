# D2RBO — Diablo II Resurrected Buff Overlay

> A lightweight, always-on-top buff timer overlay for **Diablo II: Resurrected**, built with C++ and Qt.

## Screenshots

| In-game overlay | Application UI |
|:---:|:---:|
| ![](resources/running.png) | ![](resources/preview.png) |

## Features

- **Transparent, click-through overlay** rendered *always on top* of the game window, so it never interferes with gameplay.
- **Multiple profiles** — create, switch, and delete character profiles (e.g. *Warlock*, *Amazon*, *Necromancer*).
- **Per-profile skill list** — add or remove any number of buff / skill entries.
- **Hot-key driven timers** — each skill is bound to a configurable hot key (e.g. `F5`, `F6`, `F8`). Pressing the key **resets the timer** for that buff.
- **Visual countdown** — a color-coded progress bar under each skill icon depletes over the configured duration.
- **Preview mode** — enable the **Preview** checkbox to show a movable preview window; drag the overlay with the mouse to reposition it to your preferred screen location.
- **Customisable layout** — progress-bar thickness, spacing, skill-icon size, and vertical/horizontal direction are all adjustable from the UI.
- **Backspace toggle** — a single keyboard shortcut (Backspace) toggles overlay visibility on/off.
- **Built-in skill library** — the `all_skills/` folder (shipped alongside the executable) contains icon images for **every skill across all D2R classes**. You can pick any of them and copy the icon into your active `images/` folder to activate it.

## Usage

1. **Start / Stop** — the *Start* button initialises the overlay and begins the global hot-key listener. *Stop* tears it down.
2. **Profiles** — select a profile from the dropdown, or create a new one with the ➕ button. The current profile's settings (skills, durations, hot keys, layout) are persisted automatically.
3. **Add a skill**
   - Click **Add Skill**.
   - Pick the skill from the dropdown (populated from `images/`), set the **Duration** in seconds, and assign a **Hot key**.
4. **Activate the Preview checkbox** to drag the overlay to your desired screen position. Uncheck it when done.
5. **In-game** — press the assigned hot key to reset the corresponding buff timer. The progress bar restarts and counts down; it flashes **red** during the last *Warning time* seconds.
6. **Backspace** toggles overlay visibility (if the checkbox is enabled).
7. **Adding new skill icons** — copy the desired `.png` from `all_skills/` into `images/`, then restart the app to load it.

### Configurable UI options

| Control | Default | Description |
|---------|:-------:|-------------|
| Progress bar | 3 px | Thickness of the countdown bar |
| Space | 4 px | Gap between skill icons |
| Skill Size | 32 px | Width / height of each icon |
| Warning time | 10 sec | Duration of the red warning phase |
| Vertical Direction | off | Stack icons vertically instead of horizontally |
| Backspace toggle | on | Allow Backspace to show / hide the overlay |

---

## License
See the LICENSE file for licensing information.

## Third-Party Libraries
[nlohmann/json](https://github.com/nlohmann/json)

## Disclaimer
This project is a community-made utility and is not affiliated with, endorsed by, or connected to Blizzard Entertainment. Diablo II: Resurrected and all related assets are trademarks of their respective owners.
