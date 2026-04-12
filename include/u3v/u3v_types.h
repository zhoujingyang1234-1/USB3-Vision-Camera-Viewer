/**
 * u3v_types.h - USB3 Vision / GenCP protocol types and constants
 *
 * Based on USB3 Vision specification v1.1 and GenCP v1.0
 */
#ifndef U3V_TYPES_H
#define U3V_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Error codes
 * ----------------------------------------------------------------------- */
typedef enum {
    U3V_OK                    =  0,
    U3V_ERR_INVALID_PARAM     = -1,
    U3V_ERR_NO_DEVICE         = -2,
    U3V_ERR_USB_OPEN          = -3,
    U3V_ERR_USB_IO            = -4,
    U3V_ERR_TIMEOUT           = -5,
    U3V_ERR_PROTOCOL          = -6,
    U3V_ERR_NO_MEMORY         = -7,
    U3V_ERR_NOT_CONNECTED     = -8,
    U3V_ERR_BUSY              = -9,
    U3V_ERR_ABORTED           = -10,
    U3V_ERR_BUFFER_TOO_SMALL  = -11,
    U3V_ERR_STREAM            = -12,
} u3v_status_t;

static inline const char* u3v_status_str(u3v_status_t s) {
    switch (s) {
    case U3V_OK:                   return "OK";
    case U3V_ERR_INVALID_PARAM:    return "Invalid parameter";
    case U3V_ERR_NO_DEVICE:        return "No device found";
    case U3V_ERR_USB_OPEN:         return "USB open failed";
    case U3V_ERR_USB_IO:           return "USB I/O error";
    case U3V_ERR_TIMEOUT:          return "Timeout";
    case U3V_ERR_PROTOCOL:         return "Protocol error";
    case U3V_ERR_NO_MEMORY:        return "Out of memory";
    case U3V_ERR_NOT_CONNECTED:    return "Not connected";
    case U3V_ERR_BUSY:             return "Device busy";
    case U3V_ERR_ABORTED:          return "Aborted";
    case U3V_ERR_BUFFER_TOO_SMALL: return "Buffer too small";
    case U3V_ERR_STREAM:           return "Stream error";
    default:                       return "Unknown error";
    }
}

/* --------------------------------------------------------------------------
 * USB3 Vision class identifiers
 * ----------------------------------------------------------------------- */
#define U3V_INTERFACE_CLASS     0xEF
#define U3V_INTERFACE_SUBCLASS  0x05
#define U3V_INTERFACE_PROTOCOL  0x00

/* USB3 Vision interface IDs (by convention) */
#define U3V_CONTROL_INTERFACE   0
#define U3V_EVENT_INTERFACE     1
#define U3V_STREAM_INTERFACE    2

/* --------------------------------------------------------------------------
 * GenCP (Generic Control Protocol) command IDs
 * ----------------------------------------------------------------------- */
#define GENCP_READMEM_CMD       0x0800
#define GENCP_READMEM_ACK       0x0801
#define GENCP_WRITEMEM_CMD      0x0802
#define GENCP_WRITEMEM_ACK      0x0803
#define GENCP_PENDING_ACK       0x0805
#define GENCP_EVENT_CMD         0x0C00

/* GenCP status codes */
#define GENCP_STATUS_SUCCESS            0x0000
#define GENCP_STATUS_NOT_IMPLEMENTED    0x8001
#define GENCP_STATUS_INVALID_PARAM      0x8002
#define GENCP_STATUS_INVALID_ADDRESS    0x8003
#define GENCP_STATUS_WRITE_PROTECT      0x8004
#define GENCP_STATUS_BAD_ALIGNMENT      0x8005
#define GENCP_STATUS_ACCESS_DENIED      0x8006
#define GENCP_STATUS_BUSY               0x8007
#define GENCP_STATUS_MSG_TIMEOUT        0x800B

/* GenCP magic + header sizes */
#define GENCP_PREFIX_MAGIC      0x43563355  /* "U3VC" little-endian */
#define GENCP_CCD_SIZE          8   /* Command/Ack prefix: flag(2) + cmd_id(2) + length(2) + req_id(2) */
#define GENCP_SCD_READMEM_SIZE  8   /* address(8) for 64-bit, but GenCP v1 uses 8 bytes: addr(8)+ reserved(2)+count(2) => actually 12 */

/* --------------------------------------------------------------------------
 * GenCP Packet Structures
 * ----------------------------------------------------------------------- */
#pragma pack(push, 1)

/* Command/Acknowledge header prefix */
typedef struct {
    uint32_t prefix;          /* GENCP_PREFIX_MAGIC */
    uint16_t flag;
    uint16_t command_id;
    uint16_t length;          /* SCD length (bytes after this header) */
    uint16_t request_id;
} gencp_header_t;

/* READMEM command SCD */
typedef struct {
    uint64_t address;
    uint16_t reserved;
    uint16_t count;           /* bytes to read */
} gencp_readmem_cmd_t;

/* READMEM acknowledge SCD */
typedef struct {
    /* data follows directly, length bytes */
    uint8_t  data[];
} gencp_readmem_ack_t;

/* WRITEMEM command SCD */
typedef struct {
    uint64_t address;
    /* data follows directly */
    uint8_t  data[];
} gencp_writemem_cmd_t;

/* WRITEMEM acknowledge SCD */
typedef struct {
    uint16_t reserved;
    uint16_t bytes_written;
} gencp_writemem_ack_t;

/* PENDING acknowledge SCD */
typedef struct {
    uint16_t reserved;
    uint16_t timeout_ms;      /* additional time to wait */
} gencp_pending_ack_t;

#pragma pack(pop)

/* --------------------------------------------------------------------------
 * ABRM (Application Bootstrap Register Map) - Offsets from device base
 *   The ABRM is at address 0x0000 in the Technology Specific Register Map
 * ----------------------------------------------------------------------- */
#define ABRM_GENCP_VERSION              0x0000  /* 4 bytes */
#define ABRM_MANUFACTURER_NAME          0x0048  /* 64 bytes string */
#define ABRM_MODEL_NAME                 0x0088  /* 64 bytes string */
#define ABRM_FAMILY_NAME                0x00C8  /* 64 bytes string */
#define ABRM_DEVICE_VERSION             0x0108  /* 64 bytes string */
#define ABRM_MANUFACTURER_INFO          0x0148  /* 64 bytes string */
#define ABRM_SERIAL_NUMBER              0x0188  /* 64 bytes string */
#define ABRM_USER_DEFINED_NAME          0x01C8  /* 64 bytes string */
#define ABRM_DEVICE_CAPABILITY          0x0208  /* 8 bytes */
#define ABRM_MAX_CMD_TRANSFER           0x0214  /* 4 bytes - from TSRM */
#define ABRM_MAX_ACK_TRANSFER           0x0218  /* 4 bytes - from TSRM */
#define ABRM_NUM_STREAM_CHANNELS        0x021C  /* 4 bytes */
#define ABRM_SBRM_ADDRESS               0x0220  /* 8 bytes */
#define ABRM_SBRM_LENGTH                0x0228  /* 4 bytes */
#define ABRM_MANIFEST_TABLE_ADDR        0x0230  /* 8 bytes */
#define ABRM_FIRST_URL                  0x0400  /* 512 bytes string - GenICam XML URL */
#define ABRM_SECOND_URL                 0x0600  /* 512 bytes string - GenICam XML URL */
#define ABRM_SIRM_ADDRESS               0x0020  /* 8 bytes (from U3V TSRM) */
#define ABRM_SIRM_LENGTH                0x0028  /* 4 bytes */
#define ABRM_EIRM_ADDRESS               0x002C  /* 8 bytes */
#define ABRM_EIRM_LENGTH                0x0034  /* 4 bytes */

/* --------------------------------------------------------------------------
 * SBRM (Serial Bootstrap Register Map) - Base = SBRM address from ABRM
 *   For this camera: 0x00050000
 * ----------------------------------------------------------------------- */
#define SBRM_U3V_VERSION                0x0000  /* 4 bytes */
#define SBRM_U3VCP_CAPABILITY           0x0004  /* 8 bytes */
#define SBRM_U3VCP_CONFIGURATION        0x000C  /* 8 bytes */
#define SBRM_MAX_CMD_TRANSFER           0x0014  /* 4 bytes */
#define SBRM_MAX_ACK_TRANSFER           0x0018  /* 4 bytes */
#define SBRM_NUM_STREAM_CHANNELS        0x001C  /* 4 bytes */
#define SBRM_SIRM_ADDRESS               0x0020  /* 8 bytes */
#define SBRM_SIRM_LENGTH                0x0028  /* 4 bytes */
#define SBRM_EIRM_ADDRESS               0x002C  /* 8 bytes */
#define SBRM_EIRM_LENGTH                0x0034  /* 4 bytes */
#define SBRM_IIDC2_ADDRESS              0x0038  /* 8 bytes */
#define SBRM_CURRENT_SPEED              0x0040  /* 4 bytes */

/* --------------------------------------------------------------------------
 * SIRM (Streaming Interface Register Map) - Base = SIRM address from SBRM
 *   For this camera: 0x00062000
 * ----------------------------------------------------------------------- */
#define SIRM_INFO                       0x0000  /* 4 bytes */
#define SIRM_CONTROL                    0x0004  /* 4 bytes */
#define SIRM_REQ_PAYLOAD_SIZE           0x0008  /* 8 bytes */
#define SIRM_REQ_LEADER_SIZE            0x0010  /* 4 bytes */
#define SIRM_REQ_TRAILER_SIZE           0x0014  /* 4 bytes */
#define SIRM_MAX_LEADER_SIZE            0x0018  /* 4 bytes */
#define SIRM_PAYLOAD_TRANSFER_SIZE      0x001C  /* 4 bytes */
#define SIRM_PAYLOAD_TRANSFER_COUNT     0x0020  /* 4 bytes */
#define SIRM_PAYLOAD_FINAL1_SIZE        0x0024  /* 4 bytes */
#define SIRM_PAYLOAD_FINAL2_SIZE        0x0028  /* 4 bytes */
#define SIRM_MAX_TRAILER_SIZE           0x002C  /* 4 bytes */

/* SIRM control bits */
#define SIRM_CTRL_STREAM_ENABLE         (1 << 0)

/* --------------------------------------------------------------------------
 * USB3 Vision Stream Packet types (Leader/Trailer prefix)
 * ----------------------------------------------------------------------- */
#define U3V_LEADER_MAGIC                0x4C563355  /* "U3VL" */
#define U3V_TRAILER_MAGIC               0x54563355  /* "U3VT" */

#pragma pack(push, 1)

typedef struct {
    uint32_t magic;           /* U3V_LEADER_MAGIC */
    uint16_t reserved;
    uint16_t leader_size;
    uint64_t block_id;
    uint16_t reserved2;
    uint16_t payload_type;    /* 0x0001 = Image */
    uint64_t timestamp;
    /* Image-specific leader fields: */
    uint32_t pixel_format;
    uint32_t width;
    uint32_t height;
    uint32_t offset_x;
    uint32_t offset_y;
    uint16_t padding_x;
    uint16_t padding_y;
} u3v_image_leader_t;

typedef struct {
    uint32_t magic;           /* U3V_TRAILER_MAGIC */
    uint16_t reserved;
    uint16_t trailer_size;
    uint64_t block_id;
    uint16_t status;
    uint16_t reserved2;
    uint64_t valid_payload_size;
    /* Image-specific trailer fields: */
    uint32_t size_y;          /* actual number of lines received */
} u3v_image_trailer_t;

#pragma pack(pop)

/* Payload type values */
#define U3V_PAYLOAD_TYPE_IMAGE          0x0001
#define U3V_PAYLOAD_TYPE_IMAGE_EXT      0x4001
#define U3V_PAYLOAD_TYPE_CHUNK          0x4000

/* --------------------------------------------------------------------------
 * GenICam Pixel Format Naming Convention (PFNC) - common formats
 * ----------------------------------------------------------------------- */
#define PFNC_MONO8              0x01080001
#define PFNC_MONO10             0x01100003
#define PFNC_MONO10_PACKED      0x010C0004
#define PFNC_MONO12             0x01100005
#define PFNC_MONO12_PACKED      0x010C0006
#define PFNC_MONO16             0x01100007
#define PFNC_BAYERGR8           0x01080008
#define PFNC_BAYERRG8           0x01080009
#define PFNC_BAYERGB8           0x0108000A
#define PFNC_BAYERBG8           0x0108000B
#define PFNC_BAYERGR10          0x0110000C
#define PFNC_BAYERRG10          0x0110000D
#define PFNC_BAYERGB10          0x0110000E
#define PFNC_BAYERBG10          0x0110000F
#define PFNC_BAYERGR12          0x01100010
#define PFNC_BAYERRG12          0x01100011
#define PFNC_BAYERGB12          0x01100012
#define PFNC_BAYERBG12          0x01100013
#define PFNC_YUV422_8           0x02100032
#define PFNC_RGB8               0x02180014

/* --------------------------------------------------------------------------
 * Utility types
 * ----------------------------------------------------------------------- */
typedef struct {
    uint16_t vendor_id;
    uint16_t product_id;
    char     serial[64];
    char     model[64];
    char     manufacturer[64];
    int      device_index;       /* internal enumeration index */
} u3v_device_info_t;

/* Forward declarations */
typedef struct u3v_usb_handle   u3v_usb_handle_t;
typedef struct u3v_camera       u3v_camera_t;
typedef struct u3v_stream       u3v_stream_t;

/* Image buffer */
typedef struct {
    uint8_t  *data;
    uint32_t  size;             /* allocated buffer size */
    uint32_t  received_size;    /* actual data received */
    uint32_t  width;
    uint32_t  height;
    uint32_t  pixel_format;
    uint64_t  timestamp;
    uint64_t  block_id;
    uint16_t  status;           /* trailer status: 0 = success */
} u3v_buffer_t;

/* Callback for asynchronous image delivery */
typedef void (*u3v_frame_callback_t)(u3v_buffer_t *buffer, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* U3V_TYPES_H */
