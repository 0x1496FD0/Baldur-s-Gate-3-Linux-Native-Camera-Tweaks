# Linux Native Camera Tweaks

A native Linux mod for **Baldur's Gate 3** that reworks the game's camera system entirely: full rotation around your character on any axis (including roll), and no more zoom min/max limit.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Platform](https://img.shields.io/badge/platform-Linux-green)
![Game Build](https://img.shields.io/badge/BG3%20build-4.1.1.7209685-orange)

---

## Features

- Free 360° camera rotation around your character, on all axes
- Full **roll** control (camera tilt), not available in the base game
- Zoom limit (min/max) completely removed

---

## Requirements

- Baldur's Gate 3 — build `v4.1.1.7209685`
- **SDL2** installed on your system (required to compile)

---

## Building

Make sure SDL2 is installed before compiling:

```bash
# Debian / Ubuntu
sudo apt install libsdl2-dev

# Arch Linux
sudo pacman -S sdl2

# Fedora
sudo dnf install SDL2-devel
```

Then build the project to get `linux_native_camera_tweaks.so`.

---

## Installation

Download (or build) the `.so` file and put it wherever you want on your system.

> ⚠️ In the commands below, replace the paths with your own.

### Steam version

Add the following line to the game's launch options
(right-click the game → **Properties** → **Launch Options**):

```bash
LD_PRELOAD=PATH_TO_THE_FILE/linux_native_camera_tweaks.so %command%
```

### "Standalone" version

Add this alias to your `~/.bashrc`:

```bash
alias bg3="LD_PRELOAD=PATH_TO_THE_FILE/linux_native_camera_tweaks.so PATH_TO_THE_GAME/Baldur's\ Gate\ 3/bin/bg3"
```

Then launch the game through this alias, or create an equivalent shortcut.

---

## Known issues

- Some special keys that require a modifier depending on your keyboard layout (`ö`, `$`, `"`, `ü`, etc.) don't work. Unlikely to be an issue for camera rotation, but feel free to open an issue if it affects you.

---

## Credits and permissions

- Mod created by **Biiinks78**
- Modification and release of bug fixes/improvements allowed, as long as the original creator is credited
- Asset reuse allowed with credit
- Uploading to other sites: **not allowed**
- Conversion for other games: **not allowed**
- Use in paid mods or mods earning donation points: **not allowed**

---

## Nexus Mods link

[Linux Native Camera Tweaks — Nexus Mods](https://www.nexusmods.com/baldursgate3/mods/23896)
