# U3V Camera SDK Build Environment Setup

This guide provides instructions for setting up your build environment to use the U3V Camera SDK across different platforms.

## Windows (MSVC)

### Prerequisites
- Windows 10 or later (64-bit)
- Visual Studio 2019/2022 (with "Desktop development with C++" workload installed)
- CMake (3.16 or later)

### Setup Instructions
1. Install the WinUSB driver for your camera using Zadig (refer to `WINUSB_DRIVER_INSTALL.md`).
2. Add the `include/u3v` directory to your project's include path.
3. Link your project against `lib/u3v_cam.lib`.
4. Ensure `u3v_cam.dll` and `libusb-1.0.dll` from the `bin` directory are placed next to your executable at runtime.

### CMake Example
```cmake
cmake_minimum_required(VERSION 3.16)
project(my_u3v_app)

add_executable(my_u3v_app main.c)
target_include_directories(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/include)
target_link_libraries(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/lib/u3v_cam.lib)
```

---

## Linux (Ubuntu/Debian)

### Prerequisites
- Ubuntu 20.04 or later (or compatible Debian-based distribution)
- GCC/G++ toolchain
- CMake (3.16 or later)

### Setup Instructions
1. Install required packages:
   ```bash
   sudo apt update
   sudo apt install build-essential cmake libusb-1.0-0-dev pkg-config
   ```
2. Configure udev rules to allow non-root access to the camera:
   Create a file `/etc/udev/rules.d/99-u3v-camera.rules` with the following content:
   ```
   SUBSYSTEM=="usb", ATTR{idVendor}=="04b4", ATTR{idProduct}=="1010", MODE="0666"
   ```
   Reload udev rules:
   ```bash
   sudo udevadm control --reload-rules && sudo udevadm trigger
   ```
3. Add the `include/u3v` directory to your project's include path.
4. Link your project against `libu3v_cam.so`.
5. Ensure `libu3v_cam.so` is accessible at runtime (e.g., set `LD_LIBRARY_PATH` or install to `/usr/local/lib`).

### CMake Example
```cmake
cmake_minimum_required(VERSION 3.16)
project(my_u3v_app)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(my_u3v_app main.c)
target_include_directories(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/include)
target_link_libraries(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/bin/libu3v_cam.so ${LIBUSB_LIBRARIES})
```

---

## macOS

### Prerequisites
- macOS 10.15 or later
- Xcode Command Line Tools
- Homebrew
- CMake (3.16 or later)

### Setup Instructions
1. Install Command Line Tools:
   ```bash
   xcode-select --install
   ```
2. Install dependencies via Homebrew:
   ```bash
   brew install cmake libusb pkg-config
   ```
3. Add the `include/u3v` directory to your project's include path.
4. Link your project against `libu3v_cam.dylib`.
5. Ensure `libu3v_cam.dylib` is accessible at runtime (e.g., set `DYLD_LIBRARY_PATH` or install to `/usr/local/lib`).

### CMake Example
```cmake
cmake_minimum_required(VERSION 3.16)
project(my_u3v_app)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(my_u3v_app main.c)
target_include_directories(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/include)
target_link_libraries(my_u3v_app PRIVATE path/to/V9-SDK-DLL-CUS/bin/libu3v_cam.dylib ${LIBUSB_LIBRARIES})
```
