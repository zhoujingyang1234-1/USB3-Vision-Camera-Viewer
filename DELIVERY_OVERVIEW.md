# U3V Camera SDK — Delivery Overview

USB3 Vision (U3V) compliant camera SDK with cross-platform binaries,
GUI viewer, and reference example.

- **SDK version:** 2.0.0
- **Last updated:** 2026-04-28

---

## 1. What's in the Box

| File | Platform | Size | Purpose |
|---|---|---|---|
| `V9-SDK-DLL-CUS.zip` | Windows x64 | 26 MB | Compressed package — distribute by email or cloud share |
| `V9-SDK-DLL-CUS/` | Windows x64 | 56 MB | Extracted folder, ready to use |
| `V9-SDK-SO-CUS.tar.gz` | Linux x64 / ARM64 | 520 KB | Compressed package for Linux delivery |
| `V9-SDK-SO-CUS/` | Linux x64 / ARM64 | 1.4 MB | Extracted folder, ready to use |

Both packages share the same SDK source tree and the same public API — built
once for each target platform. Code written against one will compile and run
unchanged on the other.

---

## 2. Windows Package (`V9-SDK-DLL-CUS`)

### 2.1 Target Audience

- **Windows 10 / 11 x64** users
- No prerequisites required — Qt, Visual Studio runtime, libusb, and the
  USB driver installer are all bundled
- Truly out-of-the-box

### 2.2 Folder Layout

```
V9-SDK-DLL-CUS/
├── README.md                       SDK overview
├── BUILD_ENVIRONMENT.md            Build environment notes (developer reference)
├── WINUSB_DRIVER_INSTALL.md        WinUSB driver installation guide
├── run_viewer.bat                  Launch the GUI viewer (double-click)
├── bin/                            All runtime files (~50 MB, self-contained)
│   ├── u3v_viewer.exe              GUI viewer
│   ├── basic_capture.exe           CLI capture demo
│   ├── u3v_cam.dll                 SDK library
│   ├── libusb-1.0.dll              USB transport
│   ├── vcruntime140.dll            VC++ runtime
│   ├── vcruntime140_1.dll
│   ├── msvcp140.dll
│   ├── Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll      Qt6 framework
│   ├── Qt6Network.dll, Qt6Svg.dll
│   ├── opengl32sw.dll, D3Dcompiler_47.dll           Graphics stack
│   ├── platforms/qwindows.dll                       Qt platform plugin
│   ├── styles/, imageformats/, iconengines/         UI assets
│   └── networkinformation/, tls/, generic/          Other Qt plugins
├── lib/u3v_cam.lib                 Import library (link your application)
├── include/u3v/                    Public API headers (9 files)
│   ├── u3v_sdk.h
│   ├── u3v_camera.h
│   ├── u3v_stream.h
│   ├── u3v_types.h
│   ├── u3v_imx296.h
│   ├── u3v_basler.h
│   ├── u3v_protocol.h
│   ├── u3v_usb.h
│   └── u3v_xml.h
├── examples/
│   └── basic_capture.c             CLI sample source (compilable reference)
└── tools/                          Bundled third-party utilities
    ├── zadig-2.9.exe               WinUSB driver installer
    ├── README-driver.txt           Driver installation notes
    └── LICENSE-zadig.txt           Zadig license (GPLv3 attribution)
```

### 2.3 Quick Start (End User)

```
1. Extract V9-SDK-DLL-CUS.zip to any directory (e.g., D:\u3v\)
2. Plug in the U3V camera
3. Right-click D:\u3v\V9-SDK-DLL-CUS\tools\zadig-2.9.exe
   → Run as administrator
   → Follow WINUSB_DRIVER_INSTALL.md to install the WinUSB driver
4. Double-click D:\u3v\V9-SDK-DLL-CUS\run_viewer.bat
   → GUI launches and your camera should appear in the device list
```

### 2.4 Application Development

Linking the SDK into your own program:

```cpp
// Header
#include <u3v/u3v_sdk.h>

// MSVC project settings:
//   Additional Include Directories:  V9-SDK-DLL-CUS\include
//   Additional Library Directories:  V9-SDK-DLL-CUS\lib
//   Additional Dependencies:         u3v_cam.lib
// At runtime, ensure V9-SDK-DLL-CUS\bin is on PATH, or copy the
// contents of bin\ to your executable's directory.
```

---

## 3. Linux Package (`V9-SDK-SO-CUS`)

### 3.1 Target Audience

- **Ubuntu 22.04 LTS or newer** (Debian 12+, Raspberry Pi OS Bookworm)
- **Both x86_64 and ARM64** (Raspberry Pi 5, NVIDIA Jetson, Rockchip SBCs, etc.)
- System packages required: `libusb-1.0-0`, `libqt6widgets6` (one apt command)

**Verified ARM64 platforms** (same `ubuntu22.04-arm64/` binary, no recompile):
| OS | Hardware | Status |
|---|---|---|
| Ubuntu 22.04 LTS | NVIDIA Jetson Orin Nano | ✅ |
| Debian Trixie (13) | Raspberry Pi 5 | ✅ |

### 3.2 Folder Layout

```
V9-SDK-SO-CUS/
├── README_LINUX.md                 SDK overview and deployment guide
├── ubuntu22.04-x64/                Intel/AMD 64-bit
│   ├── lib/
│   │   ├── libu3v_cam.so           ← link target
│   │   ├── libu3v_cam.so.2         ← SONAME
│   │   └── libu3v_cam.so.2.0.0     ← actual library
│   ├── bin/
│   │   ├── u3v_viewer              GUI viewer
│   │   └── basic_capture           CLI capture demo
│   ├── include/u3v/*.h             Public API headers (same 9 as Windows)
│   ├── examples/basic_capture.c    Sample source
│   └── run_viewer.sh               Launch script
└── ubuntu22.04-arm64/              ARM64 (Pi 5 / Jetson / Rockchip)
    └── (same layout, ARM aarch64 binaries)
```

### 3.3 Quick Start (End User)

```bash
# 1. Extract
tar xzf V9-SDK-SO-CUS.tar.gz
cd V9-SDK-SO-CUS

# 2. Install runtime dependencies (one-time)
sudo apt update
sudo apt install -y libusb-1.0-0 libqt6widgets6

# 3. Install udev rule (allow non-root USB camera access)
sudo tee /etc/udev/rules.d/99-u3v.rules > /dev/null <<'EOF'
SUBSYSTEM=="usb", ATTRS{bDeviceClass}=="ef", ATTRS{bDeviceSubClass}=="02", ATTRS{bDeviceProtocol}=="01", MODE="0666", GROUP="plugdev"
EOF
sudo udevadm control --reload-rules && sudo udevadm trigger
sudo usermod -aG plugdev $USER
# Log out and back in for the group change to take effect

# 4. Pick the right architecture and launch the GUI
case "$(uname -m)" in
    x86_64)  cd ubuntu22.04-x64 ;;
    aarch64) cd ubuntu22.04-arm64 ;;
esac
./run_viewer.sh
```

### 3.4 Application Development

```bash
ARCH_DIR=ubuntu22.04-x64    # or ubuntu22.04-arm64

gcc my_app.c \
    -I$ARCH_DIR/include \
    -L$ARCH_DIR/lib -lu3v_cam \
    -Wl,-rpath,'$ORIGIN/lib' \
    -o my_app
```

The `-Wl,-rpath,'$ORIGIN/lib'` flag lets the resulting binary find
`libu3v_cam.so` next to itself at runtime, so you can ship `my_app`
together with the `lib/` folder.

---

## 4. Cross-Platform Quick Reference

| Aspect | Windows (V9-SDK-DLL-CUS) | Linux (V9-SDK-SO-CUS) |
|---|---|---|
| Main library | `bin\u3v_cam.dll` | `lib/libu3v_cam.so*` |
| Link library | `lib\u3v_cam.lib` (import lib) | Link directly against `.so` |
| GUI viewer | `bin\u3v_viewer.exe` | `bin/u3v_viewer` |
| CLI sample | `bin\basic_capture.exe` | `bin/basic_capture` |
| Public API headers | `include\u3v\*.h` × 9 | Same × 9 |
| Sample source | `examples\basic_capture.c` | Same |
| Launch script | `run_viewer.bat` | `run_viewer.sh` |
| Qt6 runtime | Bundled in `bin\` | System (`apt install libqt6widgets6`) |
| C runtime | Bundled in `bin\` (VC++ Redist) | System `libc` |
| USB library | Bundled `bin\libusb-1.0.dll` | System (`apt install libusb-1.0-0`) |
| Driver setup | `tools\zadig-2.9.exe` (bundled) | udev rule (see Section 3.3) |
| Architectures | x64 only | x64 + ARM64 |
| Package size | 56 MB (zip 26 MB) | 1.4 MB (tar.gz 520 KB) |
| Supported OS | Windows 10 / 11 x64 | Ubuntu 22.04+ / Debian 12+ / Raspberry Pi OS Bookworm |

---

## 5. Source Compatibility Across Platforms

The `include/u3v/*.h` headers are **identical** in both packages. Application
code written on Windows can be recompiled on Linux without changes, and vice
versa.

### Public API Highlights

```c
/* Initialization */
u3v_status_t u3v_sdk_init(void);
void         u3v_sdk_shutdown(void);

/* Device discovery and connection */
int          u3v_discover(u3v_device_info_t *info, int max_devices);
u3v_status_t u3v_camera_open(u3v_camera_t **cam, int device_index);
void         u3v_camera_close(u3v_camera_t *cam);

/* Camera control */
u3v_status_t u3v_camera_get_temperature(u3v_camera_t *cam, uint32_t *temp);
u3v_status_t u3v_camera_set_exposure(u3v_camera_t *cam, uint32_t microseconds);
u3v_status_t u3v_camera_find_me(u3v_camera_t *cam);
/* ... additional setters / getters for gain, ROI, trigger, etc. */

/* Image streaming */
u3v_status_t u3v_stream_start(u3v_camera_t *cam);
u3v_status_t u3v_stream_grab(u3v_camera_t *cam, u3v_frame_t *frame, uint32_t timeout_ms);
u3v_status_t u3v_stream_stop(u3v_camera_t *cam);
```

For the full API surface, see the headers under `include/u3v/`.

---

## 6. Compatibility Matrix

| Item | Value |
|---|---|
| SDK version | 2.0.0 |
| USB protocol | USB3 Vision (U3V) v1.x compliant |
| Linux glibc requirement | 2.35 or newer (Ubuntu 22.04+) |
| Windows requirement | Windows 10 build 1809 or newer, all Windows 11 versions |
| Qt | 6.8.3 (bundled on Windows) / 6.2+ (system on Linux) |
| libusb | 1.0.x |
| Recommended C compiler for customers | MSVC 2022 (Windows) / GCC 11+ (Linux) |
| Recommended C++ compiler for customers | MSVC 2022 (Windows) / G++ 11+ (Linux) |

---

## 7. Delivery Notes

1. **Cloud share or email** — send the `.zip` / `.tar.gz` archive directly
2. **USB stick or air-gapped sites** — copy the entire folder
3. **Custom enterprise deployment** — the package layout is suitable as a
   base for re-branding (logo, signing, license-key gating)

For technical support, contact the SDK provider.
