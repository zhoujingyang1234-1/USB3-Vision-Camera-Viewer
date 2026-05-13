# U3V Camera SDK — Delivery Overview

USB3 Vision (U3V) compliant camera SDK with cross-platform binaries,
GUI viewer, and reference example.

- **SDK version:** 2.1.1
- **Last updated:** 2026-05-12

---

## 1. What's in the Box

| File | Platform | Size | Purpose |
|---|---|---|---|
| `V9-SDK-DLL-CUS.zip` | Windows x64 | 26 MB | Compressed Windows C/C++ package — distribute by email or cloud share |
| `V9-SDK-DLL-CUS/` | Windows x64 | 56 MB | Extracted folder, ready to use |
| `V9-SDK-SO-CUS.tar.gz` | Linux x64 / ARM64 | 520 KB | Compressed Linux C/C++ package |
| `V9-SDK-SO-CUS/` | Linux x64 / ARM64 | 1.4 MB | Extracted folder, ready to use |
| `V9-SDK-PYTHON-CUS.zip` | Windows + Linux (x64 / ARM64) | 540 KB | Compressed Python package — bundles native libraries for all 3 platforms |
| `V9-SDK-PYTHON-CUS/` | Windows + Linux (x64 / ARM64) | 1.2 MB | Extracted folder, ready to use |

All three packages share the same SDK source tree and the same underlying
shared library. Code written against the C API on Windows will compile and
run unchanged on Linux; Python code written with `u3v_cam` runs unchanged
on every supported platform.

---

## 2. Supported Sensors

The SDK is a single shared codebase that supports the sensors listed
below. The C / Python API is identical across sensors — only the
sensor-specific values differ. Code samples in §3–§5 and §7 use IMX296
values; for OV9281 / IMX585, substitute the corresponding values from
the matrix below.

| Parameter | Sony IMX296 | OmniVision OV9281 | Sony IMX585 |
|---|---|---|---|
| Status | ✅ Production (SDK 2.1.1) | ✅ Production (SDK 2.1.1) | 🟡 Reserved |
| Native resolution | 1456 × 1088 | TBD | TBD |
| Pixel size | 3.45 µm | TBD | TBD |
| Shutter type | Global | TBD | TBD |
| Max frame rate (full ROI) | 60 fps | TBD | TBD |
| Pixel formats | Mono8 / Mono10 / Mono12 / Mono16 | TBD | TBD |
| Gain register range | 0 – 480 | TBD | TBD |
| Hardware trigger | ✅ Verified (line0–3 / software, rising / falling) | TBD | TBD |
| Sensor-specific header | `u3v_imx296.h` | `u3v_ov9281.h` *(planned)* | `u3v_imx585.h` *(planned)* |
| Special modes | — | TBD | TBD |
| Verified hardware | Pi 5, Jetson Orin Nano, Win 10/11 x64 | TBD | TBD |

Empty cells (`TBD`) will be filled when the corresponding sensor lands.
The package layout, install steps, and 74-function C / Python API are
shared and **do not** change per sensor.

---

## 3. Windows Package (`V9-SDK-DLL-CUS`)

### 3.1 Target Audience

- **Windows 10 / 11 x64** users
- No prerequisites required — Qt, Visual Studio runtime, libusb, and the
  USB driver installer are all bundled
- Truly out-of-the-box

### 3.2 Folder Layout

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
├── include/u3v/                    Public API headers
│   ├── u3v_sdk.h                   Core SDK init / shutdown
│   ├── u3v_camera.h                Camera open / control
│   ├── u3v_stream.h                Streaming
│   ├── u3v_types.h                 Common types & status codes
│   ├── u3v_protocol.h              GenCP protocol primitives
│   ├── u3v_usb.h                   USB transport
│   ├── u3v_xml.h                   XML descriptor parsing
│   ├── u3v_basler.h                Vendor-specific helpers (Basler U3V)
│   ├── u3v_imx296.h                Sensor-specific helpers (see §2)
│   ├── u3v_ov9281.h                (planned — reserved for OV9281)
│   └── u3v_imx585.h                (planned — reserved for IMX585)
├── examples/
│   └── basic_capture.c             CLI sample source (compilable reference)
└── tools/                          Bundled third-party utilities
    ├── zadig-2.9.exe               WinUSB driver installer
    ├── README-driver.txt           Driver installation notes
    └── LICENSE-zadig.txt           Zadig license (GPLv3 attribution)
```

### 3.3 Quick Start (End User)

```
1. Extract V9-SDK-DLL-CUS.zip to any directory (e.g., D:\u3v\)
2. Plug in the U3V camera
3. Right-click D:\u3v\V9-SDK-DLL-CUS\tools\zadig-2.9.exe
   → Run as administrator
   → Follow WINUSB_DRIVER_INSTALL.md to install the WinUSB driver
4. Double-click D:\u3v\V9-SDK-DLL-CUS\run_viewer.bat
   → GUI launches and your camera should appear in the device list
```

### 3.4 Application Development

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

## 4. Linux Package (`V9-SDK-SO-CUS`)

### 4.1 Target Audience

- **Ubuntu 22.04 LTS or newer** (Debian 12+, Raspberry Pi OS Bookworm)
- **Both x86_64 and ARM64** (Raspberry Pi 5, NVIDIA Jetson, Rockchip SBCs, etc.)
- System packages required: `libusb-1.0-0`, `libqt6widgets6` (one apt command)

**Verified ARM64 platforms** (same `ubuntu22.04-arm64/` binary, no recompile):
| OS | Hardware | Status |
|---|---|---|
| Ubuntu 22.04 LTS | NVIDIA Jetson Orin Nano | ✅ |
| Debian Trixie (13) | Raspberry Pi 5 | ✅ |

### 4.2 Folder Layout

```
V9-SDK-SO-CUS/
├── README_LINUX.md                 SDK overview and deployment guide
├── ubuntu22.04-x64/                Intel/AMD 64-bit
│   ├── lib/
│   │   ├── libu3v_cam.so           ← link target
│   │   ├── libu3v_cam.so.2         ← SONAME
│   │   └── libu3v_cam.so.2.1.1     ← actual library
│   ├── bin/
│   │   ├── u3v_viewer              GUI viewer
│   │   └── basic_capture           CLI capture demo
│   ├── include/u3v/*.h             Public API headers (same set as Windows)
│   ├── examples/basic_capture.c    Sample source
│   └── run_viewer.sh               Launch script
└── ubuntu22.04-arm64/              ARM64 (Pi 5 / Jetson / Rockchip)
    └── (same layout, ARM aarch64 binaries)
```

### 4.3 Quick Start (End User)

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

### 4.4 Application Development

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

## 5. Python Package (`V9-SDK-PYTHON-CUS`)

### 5.1 Target Audience

- **Python 3.8+** users on Windows x64, Linux x64, or Linux ARM64
- ML / computer-vision / data-science workflows that need direct NumPy access
- Rapid prototyping, headless capture scripts, and Jupyter notebooks
- Developers who want a PyQt6 viewer they can copy and modify

This package wraps the **same** `u3v_cam` shared library that the C/C++
packages use. There is no separate codepath: a fix to the C SDK reaches
Python users by re-bundling the same binary.

### 5.2 Folder Layout

```
V9-SDK-PYTHON-CUS/
├── README.md                       Detailed Python usage guide
├── pyproject.toml                  PEP 517 metadata (`pip install -e .` works)
├── install_deps.bat / .sh          One-click dependency installer
├── run_basic_capture.bat / .sh     Capture 5 frames, save first as PGM
├── run_live_capture.bat / .sh      Continuous capture with FPS / drop stats
├── run_viewer.bat / .sh            Launch the PyQt6 + pyqtgraph live viewer
├── u3v_cam/                        Python package
│   ├── __init__.py                 High-level `Camera` class
│   ├── _raw.py                     1:1 ctypes binding (74 functions)
│   ├── _loader.py                  Cross-platform native-library locator
│   └── _libs/                      Native libraries (auto-selected by OS+arch)
│       ├── windows-x64/            u3v_cam.dll + libusb-1.0.dll
│       ├── linux-x64/              libu3v_cam.so / .so.2 / .so.2.0.0
│       └── linux-arm64/            libu3v_cam.so / .so.2 / .so.2.0.0
├── examples/
│   ├── basic_capture.py            Counterpart to C basic_capture.c
│   ├── live_capture.py             Headless live FPS measurement
│   ├── trigger_mode.py             Software-trigger demo + latency timing
│   ├── roi_exposure.py             ROI / exposure / gain sweep
│   └── viewer_pyqt.py              Full PyQt6 viewer (~290 lines, copy-friendly)
└── tests/test_smoke.py             No-hardware load + symbol-resolution test
```

### 5.3 Quick Start (End User)

**Windows:**
```bat
:: 1. Extract V9-SDK-PYTHON-CUS.zip to any directory
:: 2. Plug in the U3V camera
:: 3. Install the WinUSB driver (use Zadig from V9-SDK-DLL-CUS\tools\)
:: 4. Open a command prompt in V9-SDK-PYTHON-CUS\
install_deps.bat                    :: installs numpy + optional viewer/cv2
run_basic_capture.bat               :: smoke test
run_viewer.bat                      :: live viewer
```

**Linux (Ubuntu 22.04, Debian 12+, Raspberry Pi OS Bookworm):**
```bash
# Same prerequisites as the C/C++ Linux package: libusb-1.0-0 and the
# u3v udev rule. See Section 4.3.

unzip V9-SDK-PYTHON-CUS.zip
cd V9-SDK-PYTHON-CUS
chmod +x *.sh
./install_deps.sh
./run_basic_capture.sh
./run_viewer.sh
```

### 5.4 Application Development

The example below uses IMX296 values; for OV9281 / IMX585 substitute the
resolution, frame rate, and gain from §2.

```python
import u3v_cam

# Discover cameras
print(u3v_cam.list_cameras())
# -> [{'index': 0, 'manufacturer': '...', 'serial': '...', ...}]

# Open, configure, capture
with u3v_cam.Camera() as cam:
    cam.set_roi(1456, 1088)
    cam.pixel_format = u3v_cam.PFNC_MONO8
    cam.exposure_us  = 5000
    cam.gain         = 0
    cam.frame_rate   = 60
    cam.start()
    for _ in range(60):
        frame = cam.read_frame()        # numpy.ndarray (H, W) uint8
        # ... process frame
    cam.stop()
```

Hardware trigger:

```python
cam.configure_trigger(on=True, source="software")
cam.start()
cam.software_trigger()
frame = cam.read_frame()
```

For low-level needs, `u3v_cam._raw` exposes the full 74-function C API
exactly as declared in `include/u3v/*.h`.

### 5.5 Optional Dependencies

| Feature | Package | Install |
|---|---|---|
| Core (always required) | `numpy>=1.20` | `pip install numpy` |
| `viewer_pyqt.py` | `PyQt6` + `pyqtgraph` | `pip install PyQt6 pyqtgraph` |
| `live_capture.py --show` | `opencv-python` | `pip install opencv-python` |

Or use `install_deps.bat` / `install_deps.sh` for an interactive prompt.

---

## 6. Cross-Platform Quick Reference

| Aspect | Windows C/C++ | Linux C/C++ | Python |
|---|---|---|---|
| Package | `V9-SDK-DLL-CUS` | `V9-SDK-SO-CUS` | `V9-SDK-PYTHON-CUS` |
| Main library | `bin\u3v_cam.dll` | `lib/libu3v_cam.so*` | Same `.dll` / `.so` (bundled in `_libs/<platform>/`) |
| Link / import | `lib\u3v_cam.lib` | Link against `.so` | `import u3v_cam` (ctypes, no link step) |
| GUI viewer | `bin\u3v_viewer.exe` | `bin/u3v_viewer` | `examples\viewer_pyqt.py` |
| CLI sample | `bin\basic_capture.exe` | `bin/basic_capture` | `examples\basic_capture.py` |
| Sample source | `examples\basic_capture.c` | Same | `examples\*.py` (5 scripts) |
| Launch script | `run_viewer.bat` | `run_viewer.sh` | `run_viewer.bat` / `.sh` |
| Qt6 runtime | Bundled in `bin\` | System `libqt6widgets6` | Optional pip `PyQt6` (only for viewer) |
| C runtime | Bundled in `bin\` | System `libc` | System Python interpreter |
| USB library | Bundled `bin\libusb-1.0.dll` | System `libusb-1.0-0` | Bundled in `_libs/windows-x64/`, system on Linux |
| Driver setup | `tools\zadig-2.9.exe` (bundled) | udev rule (Section 4.3) | Same as the matching C/C++ package |
| Architectures | x64 only | x64 + ARM64 | x64 + ARM64 (single package) |
| Package size | 56 MB (zip 26 MB) | 1.4 MB (tar.gz 520 KB) | 1.2 MB (zip 540 KB) |
| Supported OS | Windows 10 / 11 x64 | Ubuntu 22.04+ / Debian 12+ / Raspberry Pi OS Bookworm | All of the above + Python 3.8 – 3.12 |

---

## 7. Source Compatibility Across Platforms

The `include/u3v/*.h` headers are **identical** in the Windows and Linux
C/C++ packages. Application code written on Windows can be recompiled on
Linux without changes, and vice versa. The Python package binds the same
74 public functions one-to-one through `u3v_cam._raw`, so behaviour is
consistent across all three packages.

### Public API Highlights — C/C++

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

### Public API Highlights — Python

```python
import u3v_cam

# Discovery
u3v_cam.list_cameras()                        # list of dicts

# High-level Camera class
with u3v_cam.Camera() as cam:
    cam.exposure_us = 5000                    # property, calls u3v_camera_set_exposure
    cam.gain        = 0
    cam.set_roi(1456, 1088)
    cam.start()
    frame = cam.read_frame()                  # numpy.ndarray
    cam.stop()

# Low-level 1:1 binding (escape hatch — same names as the C API)
from u3v_cam import _raw
_raw.camera_set_exposure(cam._handle, 5000)
```

---

## 8. Compatibility Matrix

| Item | Value |
|---|---|
| SDK version | 2.1.1 |
| USB protocol | USB3 Vision (U3V) v1.x compliant |
| Linux glibc requirement | 2.35 or newer (Ubuntu 22.04+) |
| Windows requirement | Windows 10 build 1809 or newer, all Windows 11 versions |
| Qt | 6.8.3 (bundled on Windows) / 6.2+ (system on Linux) |
| libusb | 1.0.x |
| Recommended C compiler for customers | MSVC 2022 (Windows) / GCC 11+ (Linux) |
| Recommended C++ compiler for customers | MSVC 2022 (Windows) / G++ 11+ (Linux) |
| Python version (V9-SDK-PYTHON-CUS) | 3.8 – 3.12, CPython |
| Required Python packages | `numpy>=1.20` |
| Optional Python packages | `PyQt6 + pyqtgraph` (viewer), `opencv-python` (live preview) |

---

## 9. Delivery Notes

1. **Cloud share or email** — send the `.zip` / `.tar.gz` archive directly
2. **USB stick or air-gapped sites** — copy the entire folder
3. **Custom enterprise deployment** — the package layout is suitable as a
   base for re-branding (logo, signing, license-key gating)
4. **Choosing a package**
   - System integrators / desktop apps → `V9-SDK-DLL-CUS` (Windows) +
     `V9-SDK-SO-CUS` (Linux)
   - Python / ML / CV teams, headless capture, scripting → `V9-SDK-PYTHON-CUS`
     (single archive covers Win + Linux, x64 + ARM64)
   - The three packages are **non-exclusive** — same library, different
     interface. A customer can deploy any combination.

For technical support, contact the SDK provider.
