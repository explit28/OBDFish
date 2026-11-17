# OBDFish

OBDFish is an open-source OBD-II diagnostic application for Sailfish OS. It connects to **classic Bluetooth (RFCOMM) ELM327** vehicle diagnostic adapters and displays vehicle data, dynamic parameters, and diagnostic trouble codes.

This repository is maintained at https://github.com/explit28/OBDFish. The original project was created by **Jens Drescher**: https://github.com/jdrescher2006/OBDFish.

## Sailfish OS 5.x modernization

The `compat/sailfishos-5x` branch updates application permissions, build dependencies, Bluetooth socket handling and diagnostic-log file paths for modern Sailfish OS packaging.

**Compatibility status:** source changes are prepared for Sailfish OS 5.1 and 5.2, but neither SDK compilation nor on-device operation has been verified yet. Do not treat this branch as a tested release.

The app requires a classic Bluetooth ELM327 adapter supporting serial RFCOMM (normally channel 1). BLE-only OBD adapters and Wi-Fi adapters are **not supported** by this transport implementation. Pair the adapter in Sailfish OS Bluetooth settings before using the app. Vehicle ignition may need to be enabled.

## Build

Open `harbour-obdfish.pro` in Sailfish SDK or use the Sailfish SDK command line to build an RPM for your installed target and architecture. The project uses Qt 5, Sailfish Silica, Qt Bluetooth and Qt Sensors.

Example with Sailfish SDK already installed and a suitable build target selected:

```sh
sfdk config
sfdk build
```

The RPM metadata is in `rpm/harbour-obdfish.spec` and `rpm/harbour-obdfish.yaml`. They must remain consistent. Build separately for each required architecture (such as aarch64 or armv7hl) and test on each target OS version.

### Permissions and logging

The app declares Sailjail `Bluetooth` and `Documents` permissions. Enabling diagnostic logging in the app writes `obd_log.txt` to the system Documents location; this may contain vehicle information. Logging is optional.

Migrating from an older installation may change the QSettings sandbox storage location. Back up settings before upgrading, and check saved adapter selections and custom parameter pages afterwards.

## Validation checklist

- Build the RPM in both Sailfish OS 5.1 and 5.2 SDK targets.
- Launch through Sailjail (not only from Sailfish IDE) and accept requested permissions.
- Confirm Bluetooth device discovery and paired-adapter reconnection.
- Connect an ELM327 RFCOMM adapter; verify `ATZ`, `ATE0`, `ATL0`, `ATH0`, `ATSP0` and supported PID discovery.
- Test dynamic parameters, DTC views, disconnection and reconnection.
- Enable diagnostic logging and verify the file under Documents.
- Verify saved adapter list and page settings across application restarts.

See [TODO.txt](TODO.txt) for further planned features.

## Credits

- Original OBDFish author: Jens Drescher
- Messagebox: Kimmo Lindholm, https://github.com/kimmoli/paint/blob/master/qml/components/Messagebox.qml
- Plotwidget: Thomas Kolb, https://github.com/Bytewerk/Messwerk/blob/master/src/plotwidget.cpp
- Historical Qt Connectivity backport: Richard Rondu, https://github.com/lainwir3d/qtconnectivity

## License

GNU General Public License v3 or later (see [LICENSE](LICENSE)).

**Safety:** This is experimental diagnostic software. Use it responsibly and do not operate the app while driving. Vehicle-specific compatibility has not been established for this branch.
