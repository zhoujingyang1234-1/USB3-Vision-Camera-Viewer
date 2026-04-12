/**
 * u3v_camera.h - High-level camera control API
 *
 * Provides device discovery, connection management, and feature access
 * for USB3 Vision cameras with dynamic register discovery via GenICam XML.
 */
#ifndef U3V_CAMERA_H
#define U3V_CAMERA_H

#include "u3v_types.h"
#include "u3v_sdk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Camera state */
typedef enum {
    U3V_CAM_STATE_DISCONNECTED = 0,
    U3V_CAM_STATE_CONNECTED,
    U3V_CAM_STATE_STREAMING,
} u3v_cam_state_t;

/* Camera device info (read from device) */
typedef struct {
    char     manufacturer[64];
    char     model[64];
    char     serial[64];
    char     firmware_version[64];
    uint32_t sensor_width;
    uint32_t sensor_height;
    uint32_t width_max;
    uint32_t height_max;
} u3v_cam_info_t;

/* Opaque camera handle */
typedef struct u3v_camera u3v_camera_t;

/* ---- Lifecycle ---- */

/* Initialize SDK (call once at startup) */
U3V_API u3v_status_t u3v_sdk_init(void);

/* Shutdown SDK (call once at exit) */
U3V_API void u3v_sdk_shutdown(void);

/* Discover cameras. Returns number found, fills info[] */
U3V_API int u3v_discover(u3v_device_info_t *info, int max_devices);

/* Open camera by device index */
U3V_API u3v_status_t u3v_camera_open(u3v_camera_t **cam, int device_index);

/* Close camera and free resources */
U3V_API void u3v_camera_close(u3v_camera_t *cam);

/* Get device info (populated after open) */
U3V_API const u3v_cam_info_t* u3v_camera_get_info(u3v_camera_t *cam);

/* ---- Image Format ---- */

U3V_API u3v_status_t u3v_camera_get_width(u3v_camera_t *cam, uint32_t *width);
U3V_API u3v_status_t u3v_camera_set_width(u3v_camera_t *cam, uint32_t width);
U3V_API u3v_status_t u3v_camera_get_height(u3v_camera_t *cam, uint32_t *height);
U3V_API u3v_status_t u3v_camera_set_height(u3v_camera_t *cam, uint32_t height);
U3V_API u3v_status_t u3v_camera_get_offset_x(u3v_camera_t *cam, uint32_t *offset_x);
U3V_API u3v_status_t u3v_camera_set_offset_x(u3v_camera_t *cam, uint32_t offset_x);
U3V_API u3v_status_t u3v_camera_get_offset_y(u3v_camera_t *cam, uint32_t *offset_y);
U3V_API u3v_status_t u3v_camera_set_offset_y(u3v_camera_t *cam, uint32_t offset_y);
U3V_API u3v_status_t u3v_camera_get_pixel_format(u3v_camera_t *cam, uint32_t *fmt);
U3V_API u3v_status_t u3v_camera_set_pixel_format(u3v_camera_t *cam, uint32_t fmt);
U3V_API u3v_status_t u3v_camera_get_payload_size(u3v_camera_t *cam, uint32_t *size);

/* ---- Acquisition Control ---- */

U3V_API u3v_status_t u3v_camera_set_acq_mode(u3v_camera_t *cam, uint32_t mode);
U3V_API u3v_status_t u3v_camera_start(u3v_camera_t *cam);
U3V_API u3v_status_t u3v_camera_stop(u3v_camera_t *cam);
U3V_API u3v_status_t u3v_camera_get_frame_rate(u3v_camera_t *cam, uint32_t *fps);
U3V_API u3v_status_t u3v_camera_set_frame_rate(u3v_camera_t *cam, uint32_t fps);

/* ---- Exposure ---- */

U3V_API u3v_status_t u3v_camera_get_exposure(u3v_camera_t *cam, uint32_t *time_us);
U3V_API u3v_status_t u3v_camera_set_exposure(u3v_camera_t *cam, uint32_t time_us);
U3V_API u3v_status_t u3v_camera_set_exposure_auto(u3v_camera_t *cam, uint32_t enable);

/* ---- Gain ---- */

U3V_API u3v_status_t u3v_camera_get_gain(u3v_camera_t *cam, uint32_t *gain);
U3V_API u3v_status_t u3v_camera_set_gain(u3v_camera_t *cam, uint32_t gain);

/* ---- Trigger ---- */

U3V_API u3v_status_t u3v_camera_set_trigger_selector(u3v_camera_t *cam, uint32_t selector);
U3V_API u3v_status_t u3v_camera_get_trigger_mode(u3v_camera_t *cam, uint32_t *mode);
U3V_API u3v_status_t u3v_camera_set_trigger_mode(u3v_camera_t *cam, uint32_t mode);
U3V_API u3v_status_t u3v_camera_get_trigger_source(u3v_camera_t *cam, uint32_t *source);
U3V_API u3v_status_t u3v_camera_set_trigger_source(u3v_camera_t *cam, uint32_t source);
U3V_API u3v_status_t u3v_camera_set_trigger_activation(u3v_camera_t *cam, uint32_t activation);
U3V_API u3v_status_t u3v_camera_send_software_trigger(u3v_camera_t *cam);
U3V_API u3v_status_t u3v_camera_set_line_debounce(u3v_camera_t *cam, uint32_t time_us);
U3V_API u3v_status_t u3v_camera_set_strobe(u3v_camera_t *cam, uint32_t duration,
                                    uint32_t delay, uint32_t pre_delay);

/* ---- Misc ---- */

U3V_API u3v_status_t u3v_camera_get_temperature(u3v_camera_t *cam, uint32_t *temp);
U3V_API u3v_status_t u3v_camera_reset(u3v_camera_t *cam);
U3V_API u3v_status_t u3v_camera_find_me(u3v_camera_t *cam);
U3V_API void u3v_camera_flush_stream(u3v_camera_t *cam);

/* ---- Raw register access ---- */

U3V_API u3v_status_t u3v_camera_read_reg(u3v_camera_t *cam, uint64_t addr, uint32_t *val);
U3V_API u3v_status_t u3v_camera_write_reg(u3v_camera_t *cam, uint64_t addr, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* U3V_CAMERA_H */
