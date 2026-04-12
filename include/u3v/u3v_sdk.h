/**
 * u3v_sdk.h - USB3 Vision Camera SDK
 *
 * Single-include header for the U3V camera SDK.
 * Target: IMX296 sensor on Cypress FX3 USB3 controller.
 *
 * Usage:
 *   #include <u3v/u3v_sdk.h>
 */
#ifndef U3V_SDK_H
#define U3V_SDK_H

/* Platform-specific DLL export/import definitions */
#if defined(_WIN32)
  #if defined(U3V_DLL_EXPORTS)
    #define U3V_API __declspec(dllexport)
  #else
    #define U3V_API __declspec(dllimport)
  #endif
#else
  #if defined(U3V_DLL_EXPORTS)
    #define U3V_API __attribute__((visibility("default")))
  #else
    #define U3V_API
  #endif
#endif

/* Exported Core headers */
#include "u3v_types.h"
#include "u3v_camera.h"
#include "u3v_stream.h"

/* Note: Internal protocol, xml, and usb headers are NOT exposed in the DLL */

#endif /* U3V_SDK_H */
