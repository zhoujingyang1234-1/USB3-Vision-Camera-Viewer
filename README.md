# U3V Camera SDK User Guide

Welcome to the U3V Camera SDK. This SDK provides a C-based API for controlling and streaming from USB3 Vision (U3V) cameras on Windows.

## Directory Structure
- `include/u3v/` - C header files containing the public API.
- `lib/` - `u3v_cam.lib` (static import library for linking).
- `bin/` - `u3v_cam.dll` and its dependency `libusb-1.0.dll` (required at runtime).
- `example/` - Source code for a basic console application showing how to use the SDK.

## Requirements
- Windows 10 or later (64-bit)
- MSVC compiler (Visual Studio 2019/2022) or compatible toolchain
- WinUSB driver installed for the camera (Use Zadig to replace the default driver with WinUSB)

---

## 1. Project Configuration (Visual Studio / CMake)

### Include Directories
Add the `include` folder to your compiler's include path.
```cmake
target_include_directories(your_app PRIVATE path/to/V9-SDK-DLL-CUS/include)
```

### Link Libraries
Add `u3v_cam.lib` to your linker inputs.
```cmake
target_link_libraries(your_app PRIVATE path/to/V9-SDK-DLL-CUS/lib/u3v_cam.lib)
```

### Runtime Dependencies
Ensure that both `u3v_cam.dll` and `libusb-1.0.dll` from the `bin/` folder are copied to the directory containing your compiled executable (e.g., alongside `your_app.exe`).

---

## 2. Basic Workflow

To use the SDK in your application, include the main header:
```c
#include <u3v/u3v_sdk.h>
```

### Initialization & Discovery
Before calling any other SDK functions, you must initialize the library. Then, discover connected U3V cameras.

```c
/* 1. Initialize the SDK */
if (u3v_sdk_init() != U3V_OK) {
    printf("SDK Initialization failed.\n");
    return -1;
}

/* 2. Discover cameras */
u3v_device_info_t devices[8];
int count = u3v_discover(devices, 8);
if (count == 0) {
    printf("No cameras found.\n");
    u3v_sdk_shutdown();
    return 0;
}
printf("Found camera: %s %s\n", devices[0].manufacturer, devices[0].model);
```

### Opening the Camera
Use the index from the discovery phase to open the camera.

```c
/* 3. Open the first camera */
u3v_camera_t *cam = NULL;
if (u3v_camera_open(&cam, 0) != U3V_OK) {
    printf("Failed to open camera.\n");
    return -1;
}
```

### Configuring Camera Parameters
Once opened, you can access and modify camera parameters (e.g., Exposure, Gain, ROI, Trigger).

```c
/* Example: Set Exposure to 10ms (10000 us) */
u3v_camera_set_exposure(cam, 10000);

/* Example: Set ROI */
u3v_camera_set_offset_x(cam, 0);
u3v_camera_set_offset_y(cam, 0);
u3v_camera_set_width(cam, 1440);
u3v_camera_set_height(cam, 1080);
```

### Streaming Images (Continuous Mode)
To capture images, create a stream engine, start the camera, and grab frames.

```c
/* 4. Configure and create stream */
u3v_stream_t *stream = NULL;
u3v_stream_config_t cfg = {
    .num_buffers = 4,
    .timeout_ms = 1000,
    .transfer_size = 0  /* Auto */
};
u3v_stream_create(&stream, cam, &cfg);

/* 5. Start camera acquisition */
u3v_camera_start(cam);

/* 6. Allocate an image buffer */
uint32_t payload_size = 0;
u3v_camera_get_payload_size(cam, &payload_size);
u3v_buffer_t buf = {0};
u3v_buffer_alloc(&buf, payload_size);

/* 7. Grab a frame */
u3v_status_t status = u3v_stream_grab(stream, &buf);
if (status == U3V_OK && buf.status == 0) {
    printf("Successfully captured a frame: %d x %d\n", buf.width, buf.height);
    /* Save to file (optional) */
    u3v_buffer_save(&buf, "image.pgm");
}

/* 8. Cleanup */
u3v_buffer_free(&buf);
u3v_camera_stop(cam);
u3v_stream_destroy(stream);
```

### Using Trigger Mode
To use hardware or software triggers, configure the trigger parameters *before* calling `u3v_camera_start()`.

```c
/* Enable Software Trigger (Trigger Source 4) */
u3v_camera_set_trigger_selector(cam, 2); /* FRAME_START */
u3v_camera_set_trigger_mode(cam, 1);     /* ON */
u3v_camera_set_trigger_source(cam, 4);   /* SOFTWARE */

/* Start acquisition (Camera is now waiting for a trigger) */
u3v_camera_start(cam);

/* Send software trigger and grab */
u3v_camera_send_software_trigger(cam);
u3v_status_t st = u3v_stream_grab(stream, &buf);
```

### Closing and Shutdown
Always close the camera and shut down the SDK before exiting.

```c
/* 9. Final Cleanup */
u3v_camera_close(cam);
u3v_sdk_shutdown();
```

---

## 3. Important API Functions

### Initialization & Discovery
- `U3V_API u3v_status_t u3v_sdk_init(void);`
- `U3V_API void u3v_sdk_shutdown(void);`
- `U3V_API int u3v_discover(u3v_device_info_t *info, int max_devices);`

### Camera Control
- `U3V_API u3v_status_t u3v_camera_open(u3v_camera_t **cam, int device_index);`
- `U3V_API void u3v_camera_close(u3v_camera_t *cam);`
- `U3V_API u3v_status_t u3v_camera_start(u3v_camera_t *cam);`
- `U3V_API u3v_status_t u3v_camera_stop(u3v_camera_t *cam);`

### Image Acquisition
- `U3V_API u3v_status_t u3v_stream_create(u3v_stream_t **stream, u3v_camera_t *cam, const u3v_stream_config_t *config);`
- `U3V_API void u3v_stream_destroy(u3v_stream_t *stream);`
- `U3V_API u3v_status_t u3v_buffer_alloc(u3v_buffer_t *buf, uint32_t size);`
- `U3V_API u3v_status_t u3v_stream_grab(u3v_stream_t *stream, u3v_buffer_t *buf);`

### Parameters (Examples)
- `U3V_API u3v_status_t u3v_camera_set_exposure(u3v_camera_t *cam, uint32_t time_us);`
- `U3V_API u3v_status_t u3v_camera_set_gain(u3v_camera_t *cam, uint32_t gain);`
- `U3V_API u3v_status_t u3v_camera_set_width(u3v_camera_t *cam, uint32_t width);`
- `U3V_API u3v_status_t u3v_camera_set_trigger_mode(u3v_camera_t *cam, uint32_t mode);`

*(For the complete list of parameter control functions, please refer to `include/u3v/u3v_camera.h`)*
