/**
 * basic_capture.c - USB3 Vision Camera SDK 示例程序
 *
 * 演示: 发现相机 → 连接 → 配置 → 采集 → 保存图像
 *
 * 编译 (MSVC):
 *   cl basic_capture.c /I ..\include /link u3v_cam.lib winusb.lib setupapi.lib
 *
 * 编译 (CMake):
 *   cmake --build build --target basic_capture
 */
#include <stdio.h>
#include <stdlib.h>
#include <u3v/u3v_sdk.h>

int main(int argc, char *argv[]) {
    u3v_status_t st;

    printf("=== U3V Camera SDK - IMX296 Basic Capture ===\n\n");

    /* 1. 初始化SDK */
    st = u3v_sdk_init();
    if (st != U3V_OK) {
        fprintf(stderr, "SDK init failed: %s\n", u3v_status_str(st));
        return 1;
    }

    /* 2. 发现相机 */
    u3v_device_info_t devices[4];
    int count = u3v_discover(devices, 4);
    printf("Found %d U3V camera(s)\n", count);

    if (count == 0) {
        printf("No camera found. Check:\n");
        printf("  - Camera is connected via USB3\n");
        printf("  - WinUSB or eBUS driver is installed\n");
        printf("  - Device appears in Device Manager\n");
        u3v_sdk_shutdown();
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("  [%d] VID=%04X PID=%04X %s %s\n",
               i, devices[i].vendor_id, devices[i].product_id,
               devices[i].manufacturer, devices[i].model);
    }

    /* 3. 打开第一个相机 */
    u3v_camera_t *cam = NULL;
    st = u3v_camera_open(&cam, 0);
    if (st != U3V_OK) {
        fprintf(stderr, "Camera open failed: %s\n", u3v_status_str(st));
        u3v_sdk_shutdown();
        return 1;
    }

    const u3v_cam_info_t *info = u3v_camera_get_info(cam);
    printf("\nConnected: %s %s (S/N: %s)\n", info->manufacturer, info->model, info->serial);
    printf("Sensor: %u x %u\n", info->sensor_width, info->sensor_height);

    /* 4. 配置图像参数 */
    u3v_camera_set_width(cam, info->width_max);
    u3v_camera_set_height(cam, info->height_max);
    u3v_camera_set_pixel_format(cam, PFNC_MONO8);
    u3v_camera_set_exposure(cam, 10000);   /* 10ms */
    u3v_camera_set_gain(cam, 0);
    u3v_camera_set_acq_mode(cam, IMX296_ACQ_MODE_CONTINUOUS);

    /* 读回确认 */
    uint32_t w, h, fmt, exp_us, payload;
    u3v_camera_get_width(cam, &w);
    u3v_camera_get_height(cam, &h);
    u3v_camera_get_pixel_format(cam, &fmt);
    u3v_camera_get_exposure(cam, &exp_us);
    u3v_camera_get_payload_size(cam, &payload);
    printf("\nConfig: %ux%u, PixFmt=0x%08X, Exposure=%uus, Payload=%u bytes\n",
           w, h, fmt, exp_us, payload);

    /* 5. 创建流引擎 */
    u3v_stream_t *stream = NULL;
    u3v_stream_config_t scfg = { .num_buffers = 4, .timeout_ms = 5000, .transfer_size = 0 };
    st = u3v_stream_create(&stream, cam, &scfg);
    if (st != U3V_OK) {
        fprintf(stderr, "Stream create failed: %s\n", u3v_status_str(st));
        u3v_camera_close(cam);
        u3v_sdk_shutdown();
        return 1;
    }

    /* 6. 分配图像缓冲区 */
    u3v_buffer_t buf;
    uint32_t buf_size = payload > 0 ? payload : (w * h);
    st = u3v_buffer_alloc(&buf, buf_size);
    if (st != U3V_OK) {
        fprintf(stderr, "Buffer alloc failed: %s\n", u3v_status_str(st));
        u3v_stream_destroy(stream);
        u3v_camera_close(cam);
        u3v_sdk_shutdown();
        return 1;
    }

    /* 7. 开始采集 */
    printf("\nStarting acquisition...\n");
    st = u3v_camera_start(cam);
    if (st != U3V_OK) {
        fprintf(stderr, "Acquisition start failed: %s\n", u3v_status_str(st));
        u3v_buffer_free(&buf);
        u3v_stream_destroy(stream);
        u3v_camera_close(cam);
        u3v_sdk_shutdown();
        return 1;
    }

    /* 8. 采集5帧 */
    for (int i = 0; i < 5; i++) {
        st = u3v_stream_grab(stream, &buf);
        if (st != U3V_OK) {
            fprintf(stderr, "Frame %d grab failed: %s\n", i, u3v_status_str(st));
            continue;
        }

        printf("Frame %d: %ux%u, %u bytes, ts=%llu, status=%u\n",
               i, buf.width, buf.height, buf.received_size,
               (unsigned long long)buf.timestamp, buf.status);

        /* 保存第一帧 */
        if (i == 0) {
            const char *outfile = "capture_0.pgm";
            st = u3v_buffer_save(&buf, outfile);
            if (st == U3V_OK)
                printf("  -> Saved to %s\n", outfile);
        }
    }

    /* 9. 停止采集并清理 */
    printf("\nStopping...\n");
    u3v_camera_stop(cam);
    u3v_buffer_free(&buf);
    u3v_stream_destroy(stream);
    u3v_camera_close(cam);
    u3v_sdk_shutdown();

    printf("Done.\n");
    return 0;
}
