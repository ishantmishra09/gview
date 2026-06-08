# gview

A lightweight image viewer built with SDL2. Opens images via a native file dialog, displays them with zoom and pan support, and lets you browse all images in the same directory using the arrow keys.

---

## Features

- Pan and zoom with mouse or keyboard
- Browse images in the same directory without reopening the dialog
- Bilinear filtering for smooth scaling
- Automatic fit-to-window on open and window resize
- Native file dialog via NFD

## Supported Formats

JPEG, PNG, BMP, WebP, TIFF, PPM

---

## Dependencies

| Library                                                                        | Purpose                      |
| ------------------------------------------------------------------------------ | ---------------------------- |
| [SDL2](https://www.libsdl.org/)                                                | Window, renderer, input      |
| [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended) | Native OS file dialog        |
| [stb_image](https://github.com/nothings/stb)                                   | Image decoding               |
| [stb_image_resize2](https://github.com/nothings/stb)                           | High-quality CPU downscaling |

---

## Building

### Prerequisites

Install SDL2 and nativefiledialog-extended for your platform:

**Linux**

```bash
sudo apt install libsdl2-dev    # Debian/Ubuntu
sudo pacman -S sdl2             # Arch
```

NFD must be built and installed separately — see the [NFD repository](https://github.com/btzy/nativefiledialog-extended).

**macOS**

```bash
brew install sdl2 nativefiledialog
```

**Windows**

Install SDL2 and NFD via vcpkg or provide the headers and `.lib` files manually, then ensure they are visible to your compiler toolchain.

### Compile

```bash
meson setup build
meson compile -C build
```

The binary is placed in `build/gview` (or `build/gview.exe` on Windows).

> The `vendor/` directory is on the include path — place `stb_image.h` and
> `stb_image_resize2.h` there before building.

---

## Usage

Launch the binary and press `O` to open an image. Once an image is open, all
images in the same directory are available for browsing with the arrow keys.

### Keybindings

| Key                    | Action                      |
| ---------------------- | --------------------------- |
| `O`                    | Open file dialog            |
| `F`                    | Fit image to window         |
| `Right`                | Next image in directory     |
| `Left`                 | Previous image in directory |
| `+` / `=` / numpad `+` | Zoom in                     |
| `-` / numpad `-`       | Zoom out                    |
| `Q` / `Escape`         | Quit                        |

### Mouse

| Action              | Result                    |
| ------------------- | ------------------------- |
| Scroll wheel        | Zoom in / out             |
| Left click and drag | Pan (only when zoomed in) |

---

## Project Structure

```
gview/
├── meson.build
├── src/
│   ├── main.c          # Entry point — event loop
│   ├── app.c / .h      # Application state, init, shutdown, image loading
│   ├── image.c / .h    # Image decoding (stb_image) and GPU texture upload
│   ├── viewer.c / .h   # Zoom, pan, clamp, and render logic
│   ├── gallery.c / .h  # Directory scanning and sequential navigation
│   ├── events.c / .h   # SDL event handling (keyboard, mouse, window)
│   └── nfd_g.c / .h    # Thin wrapper around the NFD file dialog
└── vendor/
    ├── stb_image.h
    └── stb_image_resize2.h
```

---

## Notes

- Images larger than 4096px on either dimension are downscaled on the CPU at
  load time using stb_image_resize2. This reduces VRAM usage and improves
  display quality at fit-to-window scale. Zooming in beyond that resolution
  will show the downscaled version rather than original pixels.
- The gallery is capped at 1024 files per directory (`MAX_GALLERY_FILES` in `app.h`).
- Zoom range is 5% to 2300%.
