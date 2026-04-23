/**
 * u3v_basler.h - Basler daA1280-54um Camera Register Definitions
 *
 * Register addresses extracted from GenICam XML
 */
#ifndef U3V_BASLER_H
#define U3V_BASLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Basler daA1280-54um Register Map */
#define BASLER_REG_WIDTH                         0x1000026CULL
#define BASLER_REG_HEIGHT                        0x10000294ULL
#define BASLER_REG_OFFSET_X                      0x1000025CULL
#define BASLER_REG_OFFSET_Y                      0x10000284ULL
#define BASLER_REG_PIXEL_FORMAT                  0x10000224ULL
#define BASLER_REG_PAYLOAD_SIZE                  0x100002E0ULL
#define BASLER_REG_WIDTH_MAX                     0x100002E4ULL
#define BASLER_REG_HEIGHT_MAX                    0x100002E8ULL
#define BASLER_REG_SENSOR_WIDTH                  0x100002E4ULL  /* Same as WidthMax */
#define BASLER_REG_SENSOR_HEIGHT                 0x100002E8ULL  /* Same as HeightMax */
#define BASLER_REG_ACQ_MODE                      0x100000BCULL
#define BASLER_REG_ACQ_START                     0x100000B4ULL
#define BASLER_REG_ACQ_STOP                      0x100000B4ULL
#define BASLER_REG_ACQ_FRAME_RATE                0x10000000ULL  /* TBD */
#define BASLER_REG_EXPOSURE_TIME                 0x10000000ULL  /* TBD */
#define BASLER_REG_EXPOSURE_AUTO                 0x10000020ULL
#define BASLER_REG_GAIN                          0x100002A4ULL
#define BASLER_REG_TEMPERATURE                   0x00000000ULL  /* Not available */
#define BASLER_REG_DEVICE_RESET                  0x00000000ULL  /* Not available */
#define BASLER_REG_FW_VERSION                    0xF2F01014ULL

/* Basler Camera Info */
#define BASLER_VENDOR_ID                         0x2676
#define BASLER_PRODUCT_ID_DAA1280                0xBA03

#ifdef __cplusplus
}
#endif

#endif /* U3V_BASLER_H */
