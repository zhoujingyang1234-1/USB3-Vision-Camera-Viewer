/**
 * u3v_export.h - DLL export/import macros
 */
#ifndef U3V_EXPORT_H
#define U3V_EXPORT_H

#ifdef _WIN32
    #ifdef U3V_DLL_EXPORTS
        #define U3V_API __declspec(dllexport)
    #else
        #define U3V_API __declspec(dllimport)
    #endif
#else
    #define U3V_API __attribute__((visibility("default")))
#endif

#endif /* U3V_EXPORT_H */
