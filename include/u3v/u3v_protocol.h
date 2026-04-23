/**
 * u3v_protocol.h - GenCP / U3V protocol layer
 *
 * Implements READMEM / WRITEMEM commands over USB bulk endpoints.
 * Provides register read/write access to ABRM, SBRM, SIRM and device registers.
 */
#ifndef U3V_PROTOCOL_H
#define U3V_PROTOCOL_H

#include "u3v_types.h"
#include "u3v_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define U3V_DEFAULT_TIMEOUT_MS  1000
#define U3V_MAX_ACK_BUFFER      1024

/* Protocol context (wraps USB handle + state) */
typedef struct {
    u3v_usb_handle_t *usb;
    uint16_t          request_id;       /* auto-incrementing */
    uint32_t          max_cmd_transfer;
    uint32_t          max_ack_transfer;
    uint64_t          sbrm_address;
    uint64_t          sirm_address;
    uint32_t          sirm_length;
    uint32_t          timeout_ms;
} u3v_protocol_t;

/* Initialize protocol context after USB is opened */
u3v_status_t u3v_protocol_init(u3v_protocol_t *proto, u3v_usb_handle_t *usb);

/* Read memory from device (GenCP READMEM) */
u3v_status_t u3v_read_mem(u3v_protocol_t *proto,
                           uint64_t address, void *data, uint32_t size);

/* Write memory to device (GenCP WRITEMEM) */
u3v_status_t u3v_write_mem(u3v_protocol_t *proto,
                            uint64_t address, const void *data, uint32_t size);

/* Convenience: read a 32-bit register */
u3v_status_t u3v_read_reg32(u3v_protocol_t *proto, uint64_t address, uint32_t *value);

/* Convenience: write a 32-bit register */
u3v_status_t u3v_write_reg32(u3v_protocol_t *proto, uint64_t address, uint32_t value);

/* Convenience: read a 64-bit register */
u3v_status_t u3v_read_reg64(u3v_protocol_t *proto, uint64_t address, uint64_t *value);

/* Read a string register (null-terminated, up to max_len) */
u3v_status_t u3v_read_string(u3v_protocol_t *proto,
                              uint64_t address, char *buf, uint32_t max_len);

/* Bootstrap: read SBRM and SIRM addresses from ABRM */
u3v_status_t u3v_protocol_bootstrap(u3v_protocol_t *proto);

#ifdef __cplusplus
}
#endif

#endif /* U3V_PROTOCOL_H */
