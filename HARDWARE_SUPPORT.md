# USB3 Vision Camera Viewer - Hardware Support List

This document lists all camera modules that have been verified and tested with the USB3 Vision Camera Viewer.

---

## ✅ Verified & Tested Cameras

### InnoMaker Products

#### U3V-IMX296-GS Series

| Model | Sensor | Resolution | Frame Rate | Features | Status | Link |
|-------|--------|-----------|-----------|----------|--------|------|
| **U3V-IMX296Mono-GS** | Sony IMX296LLR | 1.58 MP (1456×1088) | 60 fps | Global Shutter, Monochrome, Trigger/Strobe | ✅ Verified | [View](https://www.inno-maker.com/product/u3v-imx296-gs/) |
| **U3V-IMX296Color-GS** | Sony IMX296LQR | 1.58 MP (1456×1088) | 60 fps | Global Shutter, Color (Bayer), Trigger/Strobe | ✅ Verified | [View](https://www.inno-maker.com/product/u3v-imx296-gs/) |

**Specifications:**
- **Sensor Type:** CMOS Global Shutter
- **Pixel Size:** 3.4 µm × 3.4 µm
- **Optical Format:** 1/1.3 inch
- **Interface:** USB3 Vision (U3V) compliant
- **Trigger Support:** External trigger input, strobe output
- **Timestamp Resolution:** 1 microsecond
- **Isolation:** 2.5 kV galvanic isolation

**Supported Features:**
- ✅ Live preview
- ✅ Exposure control (manual & auto)
- ✅ Gain control
- ✅ ROI (Region of Interest)
- ✅ Hardware trigger mode
- ✅ Software trigger
- ✅ Strobe output control
- ✅ Frame statistics
- ✅ Image capture (PGM, RAW formats)

**Tested Platforms:**
- ✅ Windows 10/11 x64
- ✅ Ubuntu 22.04 LTS x64
- ✅ Raspberry Pi 5 (Debian Bookworm ARM64)
- ✅ NVIDIA Jetson Orin Nano (Ubuntu 22.04 ARM64)

---

## 🔄 Adding New Hardware

To add your U3V camera to this list, please:

1. **Test the camera** with USB3 Vision Camera Viewer
2. **Document the specifications** (sensor, resolution, frame rate, etc.)
3. **Verify all features** work correctly (preview, exposure, gain, trigger, etc.)
4. **Submit a pull request** with the following information:
   - Camera model name
   - Sensor manufacturer and model
   - Resolution and frame rate
   - Key features
   - Tested platforms
   - Link to product page (if available)

### Pull Request Template

```markdown
## Add Hardware Support: [Camera Model Name]

### Camera Information
- **Model:** [e.g., U3V-IMX296-GS]
- **Sensor:** [e.g., Sony IMX296]
- **Resolution:** [e.g., 1456×1088]
- **Frame Rate:** [e.g., 60 fps]
- **Interface:** USB3 Vision (U3V)

### Features Verified
- [ ] Live preview
- [ ] Exposure control
- [ ] Gain control
- [ ] ROI support
- [ ] Trigger mode
- [ ] Strobe output
- [ ] Image capture

### Tested Platforms
- [ ] Windows 10/11 x64
- [ ] Linux x64
- [ ] Linux ARM64

### Additional Notes
[Any specific notes or quirks about this camera]

### Product Link
[Link to manufacturer's product page]
```

---

## 📋 Planned Support

The following cameras are planned for testing and verification:

| Model | Status | ETA |
|-------|--------|-----|
| U3V-IMX585-GS | 🔄 In Testing | Q2 2026 |
| U3V-IMX577-GS | 🔄 In Testing | Q2 2026 |
| U3V-OV9281-GS | 📋 Planned | Q3 2026 |

---

## 🔧 Troubleshooting by Camera

### U3V-IMX296-GS Series

**Issue:** Camera not detected in viewer
- **Solution:** Ensure WinUSB driver is installed on Windows. On Linux, check udev rules are configured.

**Issue:** Trigger mode not working
- **Solution:** Verify trigger pins are properly connected. Check camera firmware supports trigger mode.

**Issue:** Image appears dark or overexposed
- **Solution:** Adjust exposure and gain values. Try auto-exposure mode first to find baseline.

---

## 📞 Support

For camera-specific issues or to report compatibility problems:
- Open an issue on GitHub with the camera model and error details
- Contact: [support@inno-maker.com](mailto:support@inno-maker.com)

---

## 📚 Related Documentation

- [USB3 Vision Standard](https://www.visiononline.org/vision-standards-details.php/3705)
- [Main README](./README.md)
- [SDK Documentation](./DELIVERY_OVERVIEW.md)

---

**Last Updated:** May 8, 2026  
**Total Verified Cameras:** 2 models  
**Total Platforms Tested:** 4 (Windows x64, Linux x64, Raspberry Pi 5, Jetson Orin Nano)
