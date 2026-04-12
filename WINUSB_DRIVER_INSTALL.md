# WinUSB Driver Installation Guide for U3V Cameras (Windows)

## Prerequisites

- Windows 10 or later
- **Zadig** utility — download from [https://zadig.akeo.ie/](https://zadig.akeo.ie/)
- USB3 Vision camera connected to a USB 3.0 port

---

## Step-by-Step Installation

### 1. Connect the Camera

Plug the U3V camera into a **USB 3.0** port. Wait a few seconds for Windows to detect the device.

### 2. Launch Zadig

Run `zadig-2.x.exe` as **Administrator** (right-click → Run as administrator).

### 3. Configure Zadig Options

Before selecting the device, you **must** change the following settings:

1. Go to **Options** menu (top menu bar)
2. Check **☑ List All Devices**
3. Uncheck **☐ Ignore Hubs or Composite Parents**

> **Important:** If you skip step 3, the correct device entry will not appear in the device list.

### 4. Select the Camera Device

In the device dropdown list, look for one of the following:

| First-time Install | Subsequent Re-install |
|---|---|
| Select the entry showing **your camera name with "(Composite Parent)"** suffix | Select the entry showing your camera's VID/PID or device name |

**For first-time installation**, you will typically see an entry like:

```
USB device (Composite Parent)
```
or
```
FX3 (Composite Parent)
```

Select this **Composite Parent** entry. Do **NOT** select individual interface entries (Interface 0, Interface 1, etc.).

> **Why Composite Parent?**
> U3V cameras are USB composite devices with multiple interfaces (Control, Event, Stream). The WinUSB driver must be installed on the **parent device** to allow libusb to access all interfaces.

### 5. Install the WinUSB Driver

1. In the **Driver** field (right side), make sure **WinUSB** is selected
   - If another driver is shown, use the up/down arrows to select **WinUSB (vX.X.X.X)**
2. Click **Replace Driver** (or **Install Driver** if no driver is currently installed)
3. Wait for the installation to complete — you should see a **"The driver was installed successfully"** message

### 6. Verify Installation

1. Open **Device Manager** (Win+X → Device Manager)
2. Look under **Universal Serial Bus devices**
3. Your camera should appear with no warning icons (no yellow triangle)
4. Right-click → Properties → Driver tab should show **WinUSB** as the driver

---

## Troubleshooting

### Camera not appearing in Zadig device list

- Ensure **Options → List All Devices** is checked
- Ensure **Options → Ignore Hubs or Composite Parents** is **unchecked**
- Try a different USB 3.0 port
- Unplug and re-plug the camera

### "Access Denied" or LIBUSB_ERROR_ACCESS when opening camera

- The WinUSB driver was likely installed on the wrong device entry
- Re-run Zadig and install on the **Composite Parent** entry (see Step 4)

### Camera works once but requires USB re-plug between sessions

- This is normal behavior during initial setup
- After proper WinUSB driver installation, the camera should work without re-plugging

### Need to revert to the original driver

1. Open **Device Manager**
2. Right-click the camera device → **Update driver**
3. Select **Browse my computer for drivers** → **Let me pick from a list**
4. Choose the original driver (e.g., Cypress FX3 driver)

---

## Quick Reference

| Step | Action |
|---|---|
| 1 | Connect camera to USB 3.0 port |
| 2 | Run Zadig as Administrator |
| 3 | Options → ☑ List All Devices |
| 4 | Options → ☐ Ignore Hubs or Composite Parents (uncheck) |
| 5 | Select **"USB device (Composite Parent)"** from dropdown |
| 6 | Set driver to **WinUSB** → Click **Replace Driver** |
| 7 | Verify in Device Manager — no warning icons |

---

*This guide is for the U3V Camera SDK (libusb backend). The WinUSB driver is required for libusb to communicate with the camera on Windows.*
