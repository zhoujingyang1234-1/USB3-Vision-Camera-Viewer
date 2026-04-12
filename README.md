# USB3 Vision Camera Viewer

Welcome to the **USB3 Vision Camera Viewer** repository. This project provides a dedicated camera viewer and SDK designed specifically for USB3 Vision (U3V) industrial cameras. 

It is built to operate with free and open-source USB drivers, eliminating the need for expensive proprietary software licenses while maintaining high performance and reliability.

## Key Features

- **Dedicated U3V Viewer**: A streamlined, easy-to-use viewer application for USB3 Vision cameras.
- **Free Driver Support**: Fully compatible with free `WINUSB` and `LIBUSB` drivers.
- **Cross-Platform Potential**: Designed with standard USB protocols, offering support for Windows, Linux, and macOS environments.
- **C-based SDK**: Includes a lightweight, high-performance C API (`u3v_cam.dll` / `u3v_cam.lib`) for custom application development.
- **Free to Use**: The software and SDK are provided free of charge for compatible hardware.

## Supported Hardware List

This viewer and SDK have been tested and verified with the following hardware:

- **InnoMaker U3V-CAM-IMX296**
  - High-speed global shutter USB3 Vision camera based on the Sony IMX296 sensor.
  - Product Link: [https://www.inno-maker.com/product/u3v-imx296-gs/](https://www.inno-maker.com/product/u3v-imx296-gs/)

*(More compatible models will be added to this list as they are verified.)*

## Documentation

For detailed instructions on how to install drivers, configure your development environment, and use the SDK API, please refer to the following documents included in this repository:

- [**User Manual**](UserManual.md): Comprehensive guide on SDK directory structure, project configuration (CMake/MSVC), and basic API workflow.
- [**WinUSB Driver Installation**](WINUSB_DRIVER_INSTALL.md): Step-by-step guide on how to replace default camera drivers with the required WinUSB driver using Zadig.
- [**Build Environment**](BUILD_ENVIRONMENT.md): Instructions for setting up the build environment.

## Quick Start (SDK)

The repository includes a pre-compiled SDK and a basic example. 

1. Install the WinUSB driver for your camera (see `WINUSB_DRIVER_INSTALL.md`).
2. Navigate to the `example/` directory to view `basic_capture.c`.
3. Link against `lib/u3v_cam.lib` and ensure `bin/u3v_cam.dll` and `bin/libusb-1.0.dll` are in your executable path.
4. Refer to `UserManual.md` for detailed API usage.
