# OBDFish 0.95 — community test instructions

This is a **test build**, not a verified release. It is intended for Sailfish OS 5.x testers who have access to a car and a **classic Bluetooth ELM327** OBD-II adapter.

## Getting the RPM

1. Open [GitHub Actions](https://github.com/explit28/OBDFish/actions/workflows/sailfish-rpm.yml).
2. Choose a successful *Sailfish OS RPM test builds* run for the `compat/sailfishos-5x` branch.
3. Download the `obdfish-aarch64-rpm` or `obdfish-armv7hl-rpm` artifact as appropriate for your phone. Extract the ZIP and install the RPM.
4. If there is no successful run, **no installable package is available yet**. Report the build failure rather than downloading an unverified file.

Check your device architecture using `uname -m`. Use an RPM that matches your device architecture. Installing an incompatible RPM will not work.

## Safety precautions

- Park the car safely and turn on the ignition when required; **never operate the app while driving**.
- Use a known-working classic Bluetooth ELM327 adapter (Bluetooth Low Energy / BLE and Wi-Fi ELM327 adapters are not supported).
- Do not clear diagnostic trouble codes unless the vehicle owner authorizes it and understands the effects.
- Do not share unredacted VINs, Bluetooth MAC addresses, location details or diagnostic logs publicly.

## Suggested test

1. Note the phone model, exact Sailfish OS version, CPU architecture and ELM327 adapter model.
2. Pair the adapter using Sailfish OS Bluetooth settings (common pairing PINs are 1234 or 0000).
3. Launch OBDFish and grant Bluetooth and Documents permissions if prompted.
4. Scan for devices and connect to the ELM327 adapter.
5. Verify adapter initialization and live values (RPM, temperature, speed where available).
6. Open the diagnostic trouble code page and confirm that it displays without crashing. **Do not erase codes**.
7. Disconnect, reconnect, close the app and reopen it. Verify that saved adapter and page settings survive.
8. Optionally enable logging and confirm `obd_log.txt` in Documents; remove private information before sharing it.

## Report test results

Open a [GitHub issue](https://github.com/explit28/OBDFish/issues/new) with:

- Device:
- Sailfish OS exact version:
- Architecture (`uname -m`):
- ELM327 adapter type/model:
- Bluetooth pairing: pass / fail
- Discovery: pass / fail
- Connection and initialization: pass / fail
- Live PIDs: pass / fail
- DTC page: pass / fail
- Restart/reconnection: pass / fail
- Optional debug log (redacted):
- Steps to reproduce any failure:

Please include the exact RPM filename and the GitHub Actions run URL so results can be tied to a build.
