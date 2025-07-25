# Integration test: Display of telemetry data received via HID output reports

## Purpose and summary

To test that telemetry data is received and parsed correctly by the HID implementation,
then properly displayed by the frame server.

## Hardware setup

We are using two display devices:

- A simple (LED) shift light
- A PCF8574-based LED driver with 8 LEDs.

Refer to their unit tests to see the wiring.

Output through USB serial port at 115200 bauds.

## Software setup

SimHub must be installed in the test machine.
The official plugin must be installed and enabled.
However, no game is required.
We are using pre-recorded telemetry data found in the
[TestTelemetryData](./TestTelemetryData/) folder.

1. Ensure the device is connected to the host computer through BLE.
2. Run SimHub. Enable the official plugin if asked to.
3. Choose **Automobilista 2** as active game (click on `change active game`).
   **This is a must**.
4. Click on `Replay` (top-right corner).
5. Click on `Open replay folder`.
6. Copy pre-recorded files to that folder.
7. Close the window and click on `Replay` again.
8. Choose the recording and hit the play button.

## Procedure and expected output

> [!IMPORTANT]
> This test departs a lot from others because there is no way to predict
> a "correct" behavior.

1. Open the serial monitor.
2. **From now on**, just check there are **no error messages**
   in the serial monitor,
   in particular, `Stack canary watchpoint triggered (UI)`.
   The firmware must be running for a while before this kind
   of error is triggered.
3. A never-ending loop of telemetry data should be displayed.
4. Check that shown telemetry data is coherent.
5. In doubt, open the `available properties` menu in SimHub.
   Search for a property to display the value sent.

The pre-recorded telemetry data must reflect the following car behavior:

1. Engine start.
2. Run through the pit lane (pit limiter on).
3. Run through a straight going into the rev limiter .
4. Hard brake (look for ABS engagement).
5. Hard on the gas pedal (look for TC engagement).
6. Run through some corners.
7. Engine stop.
