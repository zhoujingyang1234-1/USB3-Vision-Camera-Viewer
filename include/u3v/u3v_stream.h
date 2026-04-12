/**
 * u3v_stream.h - Image streaming engine
 *
 * Handles U3V stream protocol: Leader → Payload → Trailer
 * Provides synchronous single-frame grab and async callback-based streaming.
 */
#ifndef U3V_STREAM_H
#define U3V_STREAM_H

#include "u3v_types.h"
#include "u3v_camera.h"
#include "u3v_sdk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Stream configuration */
typedef struct {
    uint32_t num_buffers;       /* number of image buffers to allocate (default: 4) */
    uint32_t timeout_ms;        /* per-frame timeout (default: 5000) */
    uint32_t transfer_size;     /* USB bulk transfer chunk size (0 = auto) */
} u3v_stream_config_t;

/* Create stream engine for a connected camera */
U3V_API u3v_status_t u3v_stream_create(u3v_stream_t **stream, u3v_camera_t *cam,
                                const u3v_stream_config_t *config);

/* Destroy stream engine and free all buffers */
U3V_API void u3v_stream_destroy(u3v_stream_t *stream);

/* Allocate an image buffer (caller must free with u3v_buffer_free) */
U3V_API u3v_status_t u3v_buffer_alloc(u3v_buffer_t *buf, uint32_t size);

/* Free image buffer data */
U3V_API void u3v_buffer_free(u3v_buffer_t *buf);

/* Grab a single frame (synchronous, blocking).
 * Camera must be in streaming state (u3v_camera_start called).
 * Buffer must be pre-allocated with u3v_buffer_alloc. */
U3V_API u3v_status_t u3v_stream_grab(u3v_stream_t *stream, u3v_buffer_t *buf);

/* Save raw image data to file (PGM for mono8, PPM for RGB8, raw binary otherwise) */
U3V_API u3v_status_t u3v_buffer_save(const u3v_buffer_t *buf, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* U3V_STREAM_H */
