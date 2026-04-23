/**
 * u3v_xml.h - GenICam XML download and register map discovery
 *
 * Downloads the GenICam XML descriptor from a U3V camera and
 * extracts register addresses for standard features.
 */
#ifndef U3V_XML_H
#define U3V_XML_H

#include "u3v_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize regmap with IMX296 defaults */
void u3v_regmap_init_defaults(u3v_regmap_t *regmap);

/* Initialize regmap with Basler defaults */
void u3v_regmap_init_basler(u3v_regmap_t *regmap);

/* Download GenICam XML from camera and populate regmap.
 * Falls back to defaults on failure. */
u3v_status_t u3v_xml_discover_registers(u3v_camera_t *cam);

/* Download raw GenICam XML from camera (caller must free *xml_out).
 * Returns XML size in *xml_size. Handles ZIP decompression. */
u3v_status_t u3v_xml_download(u3v_protocol_t *proto,
                               char **xml_out, uint32_t *xml_size);

#ifdef __cplusplus
}
#endif

#endif /* U3V_XML_H */
