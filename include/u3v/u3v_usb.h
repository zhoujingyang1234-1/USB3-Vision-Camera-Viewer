/**
 * u3v_usb.h - USB transport layer for USB3 Vision cameras
 *
 * Windows: WinUSB + SetupAPI
 * Linux:   libusb-1.0
 */
#ifndef U3V_USB_H
#define U3V_USB_H

#include "u3v_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize / shutdown the USB subsystem */
u3v_status_t u3v_usb_init(void);
void         u3v_usb_shutdown(void);

/* Enumerate USB3 Vision devices. Returns count found, fills info[] up to max_devices. */
int u3v_usb_enumerate(u3v_device_info_t *info, int max_devices);

/* Open / close a device by index (from enumeration) */
u3v_status_t u3v_usb_open(u3v_usb_handle_t **handle, int device_index);
void         u3v_usb_close(u3v_usb_handle_t *handle);

/* Bulk transfer on control endpoint */
u3v_status_t u3v_usb_control_write(u3v_usb_handle_t *handle,
                                    const uint8_t *data, uint32_t length,
                                    uint32_t timeout_ms);
u3v_status_t u3v_usb_control_read(u3v_usb_handle_t *handle,
                                   uint8_t *data, uint32_t length,
                                   uint32_t *actual_length,
                                   uint32_t timeout_ms);

/* Bulk transfer on stream endpoint */
u3v_status_t u3v_usb_stream_read(u3v_usb_handle_t *handle,
                                  uint8_t *data, uint32_t length,
                                  uint32_t *actual_length,
                                  uint32_t timeout_ms);

/* Bulk transfer on event endpoint (optional) */
u3v_status_t u3v_usb_event_read(u3v_usb_handle_t *handle,
                                 uint8_t *data, uint32_t length,
                                 uint32_t *actual_length,
                                 uint32_t timeout_ms);

/* Get max packet sizes for stream endpoint */
uint32_t u3v_usb_get_stream_max_transfer(u3v_usb_handle_t *handle);

/* Flush (discard) any pending data on the stream endpoint */
void u3v_usb_stream_flush(u3v_usb_handle_t *handle);

/* Disable async streaming (for trigger mode) */
void u3v_usb_stream_disable_async(u3v_usb_handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* U3V_USB_H */
