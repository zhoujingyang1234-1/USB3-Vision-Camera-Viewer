# USB3 Vision Camera Viewer

**A free, open-source viewer and SDK for USB3 Vision (U3V) cameras using WinUSB/libusb drivers**

![License](https://img.shields.io/badge/License-Free-green) ![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-blue) ![Architecture](https://img.shields.io/badge/Architecture-x64%20%7C%20ARM64-blue)

---

## 🎯 Overview

**USB3 Vision Camera Viewer** is a professional-grade, completely free application designed for controlling and viewing USB3 Vision (U3V) compliant cameras. It provides both a graphical user interface (GUI) and a comprehensive C SDK for developers.

### Key Features

- **✅ Completely Free** — No licensing fees, no restrictions
- **🖥️ Cross-Platform** — Windows 10/11 x64 and Linux (x64/ARM64)
- **📷 Live Preview** — Real-time camera stream visualization
- **⚙️ Full Camera Control** — Exposure, gain, ROI, trigger mode, and more
- **🛠️ Developer-Friendly** — Complete C SDK with examples
- **🔌 Universal Driver Support** — Works with WinUSB and libusb
- **📊 Professional Tools** — Histogram, frame statistics, image capture
- **🎯 Hardware Trigger Support** — External trigger and strobe synchronization

---

## 📦 What's Included

### Windows Package (V9-SDK-DLL-CUS)

| Component | Details |
|-----------|---------|
| **GUI Viewer** | `u3v_viewer.exe` — Full-featured camera control interface |
| **CLI Tool** | `basic_capture.exe` — Command-line image capture |
| **SDK Library** | `u3v_cam.dll` + `u3v_cam.lib` — C API for custom applications |
| **Headers** | 9 public API headers for development |
| **Examples** | `basic_capture.c` — Reference implementation |
| **Tools** | Zadig WinUSB driver installer (bundled) |
| **Size** | 56 MB (extracted), 26 MB (compressed) |

### Linux Package (V9-SDK-SO-CUS)

| Component | Details |
|-----------|---------|
| **GUI Viewer** | `u3v_viewer` — Full-featured camera control interface |
| **CLI Tool** | `basic_capture` — Command-line image capture |
| **SDK Library** | `libu3v_cam.so*` — Shared library for custom applications |
| **Headers** | 9 public API headers (identical to Windows) |
| **Examples** | `basic_capture.c` — Reference implementation |
| **Architectures** | x64 (Intel/AMD) + ARM64 (Raspberry Pi 5, Jetson, etc.) |
| **Size** | 1.4 MB (extracted), 520 KB (compressed) |

---

## 🚀 Quick Start

### Windows

#### 1. Install WinUSB Driver

```
1. Extract V9-SDK-DLL-CUS.zip
2. Run V9-SDK-DLL-CUS\tools\zadig-2.9.exe as Administrator
3. Select your U3V camera from the device list
4. Click "Replace Driver" to install WinUSB
5. Restart the computer (recommended)
```

#### 2. Launch the Viewer

```
Double-click: V9-SDK-DLL-CUS\run_viewer.bat
```

The GUI should launch and automatically detect connected U3V cameras.

### Linux

#### 1. Install Dependencies

```bash
sudo apt update
sudo apt install -y libusb-1.0-0 libqt6widgets6
```

#### 2. Install udev Rule (Optional, for non-root access)

```bash
sudo tee /etc/udev/rules.d/99-u3v.rules > /dev/null <<'EOF'
SUBSYSTEM=="usb", ATTRS{bDeviceClass}=="ef", ATTRS{bDeviceSubClass}=="02", ATTRS{bDeviceProtocol}=="01", MODE="0666", GROUP="plugdev"
EOF
sudo udevadm control --reload-rules && sudo udevadm trigger
sudo usermod -aG plugdev $USER
# Log out and back in for changes to take effect
```

#### 3. Launch the Viewer

```bash
cd V9-SDK-SO-CUS

# For x64 systems
cd ubuntu22.04-x64
./run_viewer.sh

# For ARM64 systems (Raspberry Pi 5, Jetson, etc.)
cd ubuntu22.04-arm64
./run_viewer.sh
```

---

## 📋 Supported Hardware

The USB3 Vision Camera Viewer supports any USB3 Vision (U3V) compliant camera using WinUSB or libusb drivers. Below is a list of verified and tested camera modules:

### Verified Camera Modules

| Product | Sensor | Resolution | Features | Link |
|---------|--------|-----------|----------|------|
| **U3V-CAM-GS** | Sony IMX296 | 1.58 MP (1456×1088) | Global Shutter, 60fps, Monochrome/Color, Trigger/Strobe | [View Product](https://www.inno-maker.com/product/u3v-imx296-gs/) |

### Planned Camera Modules

| Product | Sensor | Status |
|---------|--------|--------|
| **U3V-CAM-OV9281** | OmniVision OV9281 | 📋 Coming Soon |
| **U3V-CAM-IMX585** | Sony IMX585 | 📋 Coming Soon |

### Adding New Hardware

We welcome contributions! If you have tested this viewer with additional U3V cameras, please submit a pull request or contact us to add your hardware to the supported list.

---

## 💻 SDK Documentation

### Windows Development

#### Project Setup (Visual Studio / CMake)

```cmake
# Include directories
target_include_directories(your_app PRIVATE V9-SDK-DLL-CUS/include)

# Link libraries
target_link_libraries(your_app PRIVATE V9-SDK-DLL-CUS/lib/u3v_cam.lib)

# Runtime: Copy V9-SDK-DLL-CUS/bin/*.dll to your executable directory
```

#### Basic Usage Example

```c
#include <u3v/u3v_sdk.h>
#include <stdio.h>

int main() {
    /* Initialize SDK */
    if (u3v_sdk_init() != U3V_OK) {
        printf("SDK initialization failed.\n");
        return -1;
    }

    /* Discover cameras */
    u3v_device_info_t devices[8];
    int count = u3v_discover(devices, 8);
    if (count == 0) {
        printf("No cameras found.\n");
        u3v_sdk_shutdown();
        return 0;
    }

    printf("Found camera: %s %s\n", devices[0].manufacturer, devices[0].model);

    /* Open first camera */
    u3v_camera_t *cam = NULL;
    if (u3v_camera_open(&cam, 0) != U3V_OK) {
        printf("Failed to open camera.\n");
        u3v_sdk_shutdown();
        return -1;
    }

    /* Set exposure to 10ms */
    u3v_camera_set_exposure(cam, 10000);

    /* Create stream */
    u3v_stream_t *stream = NULL;
    u3v_stream_config_t cfg = {
        .num_buffers = 4,
        .timeout_ms = 1000,
        .transfer_size = 0
    };
    u3v_stream_create(&stream, cam, &cfg);

    /* Start acquisition */
    u3v_camera_start(cam);

    /* Grab a frame */
    uint32_t payload_size = 0;
    u3v_camera_get_payload_size(cam, &payload_size);
    u3v_buffer_t buf = {0};
    u3v_buffer_alloc(&buf, payload_size);

    u3v_status_t status = u3v_stream_grab(stream, &buf);
    if (status == U3V_OK && buf.status == 0) {
        printf("Successfully captured: %d x %d\n", buf.width, buf.height);
        u3v_buffer_save(&buf, "image.pgm");
    }

    /* Cleanup */
    u3v_buffer_free(&buf);
    u3v_camera_stop(cam);
    u3v_stream_destroy(stream);
    u3v_camera_close(cam);
    u3v_sdk_shutdown();

    return 0;
}
```

See `V9-SDK-DLL-CUS/examples/basic_capture.c` for the complete example.

### Linux Development

#### Compilation

```bash
ARCH_DIR=V9-SDK-SO-CUS/ubuntu22.04-x64    # or ubuntu22.04-arm64

gcc my_app.c \
    -I$ARCH_DIR/include \
    -L$ARCH_DIR/lib -lu3v_cam \
    -Wl,-rpath,'$ORIGIN/lib' \
    -o my_app
```

The `-Wl,-rpath,'$ORIGIN/lib'` flag allows the binary to find the SDK library at runtime.

#### Running Your Application

```bash
# Copy the lib folder next to your executable
cp -r $ARCH_DIR/lib ./
./my_app
```

See `V9-SDK-SO-CUS/ubuntu22.04-x64/examples/basic_capture.c` for the complete example.

---

## 📚 API Reference

### Core Functions

#### Initialization & Discovery
```c
u3v_status_t u3v_sdk_init(void);
void         u3v_sdk_shutdown(void);
int          u3v_discover(u3v_device_info_t *info, int max_devices);
```

#### Camera Control
```c
u3v_status_t u3v_camera_open(u3v_camera_t **cam, int device_index);
void         u3v_camera_close(u3v_camera_t *cam);
u3v_status_t u3v_camera_start(u3v_camera_t *cam);
u3v_status_t u3v_camera_stop(u3v_camera_t *cam);
```

#### Image Acquisition
```c
u3v_status_t u3v_stream_create(u3v_stream_t **stream, u3v_camera_t *cam, const u3v_stream_config_t *config);
void         u3v_stream_destroy(u3v_stream_t *stream);
u3v_status_t u3v_stream_grab(u3v_stream_t *stream, u3v_buffer_t *buf);
```

#### Camera Parameters
```c
u3v_status_t u3v_camera_set_exposure(u3v_camera_t *cam, uint32_t time_us);
u3v_status_t u3v_camera_set_gain(u3v_camera_t *cam, uint32_t gain);
u3v_status_t u3v_camera_set_width(u3v_camera_t *cam, uint32_t width);
u3v_status_t u3v_camera_set_height(u3v_camera_t *cam, uint32_t height);
u3v_status_t u3v_camera_set_trigger_mode(u3v_camera_t *cam, uint32_t mode);
u3v_status_t u3v_camera_send_software_trigger(u3v_camera_t *cam);
```

For the complete API, see the header files in `include/u3v/`.

---

## 🔧 Troubleshooting

### Windows

**Issue:** Camera not detected
- **Solution:** Ensure WinUSB driver is installed using Zadig. Check Device Manager to confirm the camera appears as a USB device.

**Issue:** "Failed to open camera"
- **Solution:** Close any other applications using the camera. Try a different USB port or USB 3.0 port specifically.

**Issue:** DLL not found
- **Solution:** Ensure all `.dll` files from `V9-SDK-DLL-CUS/bin/` are in the same directory as your executable.

### Linux

**Issue:** Permission denied when accessing camera
- **Solution:** Install the udev rule (see Quick Start section) and log out/back in.

**Issue:** libusb-1.0.so.0 not found
- **Solution:** Install libusb: `sudo apt install libusb-1.0-0`

**Issue:** Qt6 libraries not found
- **Solution:** Install Qt6: `sudo apt install libqt6widgets6`

---

## 📋 System Requirements

### Windows
- **OS:** Windows 10 (build 1809+) or Windows 11, x64
- **USB:** USB 3.0 or higher
- **Driver:** WinUSB (installed via Zadig)
- **Runtime:** Bundled (VC++ Redist, Qt6)

### Linux
- **OS:** Ubuntu 22.04 LTS or newer, Debian 12+, Raspberry Pi OS Bookworm
- **Architecture:** x64 (Intel/AMD) or ARM64 (Raspberry Pi 5, Jetson, Rockchip, etc.)
- **USB:** USB 3.0 or higher
- **Libraries:** libusb-1.0-0, libqt6widgets6
- **Compiler (for development):** GCC 11+ or Clang 14+

---

## 📄 Technical Specifications

| Specification | Value |
|---------------|-------|
| **SDK Version** | 2.0.0 |
| **USB Standard** | USB3 Vision (U3V) v1.x compliant |
| **Protocol** | USB 3.0 SuperSpeed |
| **Max Bandwidth** | Up to 400 MB/s (theoretical USB 3.0 limit) |
| **Supported Pixel Formats** | Mono8, Mono16, RGB8, Bayer8, Bayer16 |
| **Timestamp Resolution** | 1 microsecond |
| **Trigger Support** | Hardware trigger, software trigger, strobe output |
| **Qt Version** | 6.8.3 (Windows bundled) / 6.2+ (Linux system) |
| **libusb Version** | 1.0.x |

---

## 📝 License

This software is **completely free to use** with no licensing restrictions. See LICENSE file for details.

---

## 🤝 Contributing

We welcome contributions! If you:
- Have tested this viewer with additional U3V cameras
- Found bugs or have feature suggestions
- Want to improve documentation
- Have optimizations or enhancements

Please submit a pull request or open an issue.

---

## 📞 Support

For technical support, bug reports, or feature requests, please:
- Open an issue on GitHub
- Contact: [support@inno-maker.com](mailto:support@inno-maker.com)

---

## 🔗 Related Resources

- [InnoMaker Official Website](https://www.inno-maker.com/)
- [USB3 Vision Standard](https://www.visiononline.org/vision-standards-details.php/3705)
- [Zadig USB Driver Installer](https://zadig.akeo.ie/)
- [libusb Documentation](https://libusb.info/)

---

**Last Updated:** May 8, 2026  
**SDK Version:** 2.0.0  
**Status:** ✅ Production Ready
