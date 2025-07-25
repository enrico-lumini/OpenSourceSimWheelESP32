# Switches subsystem

## Purpose

To provide a number of **switches** to the system,
including (when required):

- Push buttons.
- Push levers and roller levers (suitable for shift paddles and "digital" clutch paddles).
- Built in push buttons of rotary encoders (both KY-040 and bare bone).
- Directional pads, directional joysticks and funky switches
  (except for rotation, which behaves like any other
  [rotary encoder](../RelativeRotaryEncoder/RelativeRotaryEncoder_en.md)).
- Rotary coded switches.
  Now they can be attached to **any** supported input hardware.
  Instructions below.

And also **potentiometers** as digital clutch paddles, in case you are short of GPIO pins.
If not, potentiometers as
[analog clutch paddles](../AnalogClutchPaddles/AnalogClutchPaddles_en.md)
are a better option.

There are several, non-exclusive, **implementation** choices:

- Button matrix.
- Analog Multiplexers.
- PISO shift registers. Not recommended since it takes too much space.
- GPIO expanders.
  **The best option and recommended way to go** thanks to its minimal cost,
  size, pin expenditure, and overall simplicity.
- Single switch (or button) attached to a single pin.

Take a look at the article on [input hardware](../../InputHW_en.md) for an introduction.

> [!CAUTION]
> It has been reported that some ALPS funky switches shows erroneously swapped
> `Push` and `COM` tags, at least, in their datasheet.
> You can check this with the help of a basic multimeter.

![Misleading ALPS datasheet](./ALPS_sheet_mistake.png)

## Button Matrix implementation

Two hardware designs are given below.
You should be able to extrapolate those designs to your needs.
Button matrices in this project work in *positive logic*
(inputs are internally pulled down).

> [!CAUTION]
> Do not remove the diodes from these designs.
> Otherwise this firmware will cause a **short circuit**.

### Button Matrix (25 inputs)

Needed parts (not counting input hardware like push buttons nor a perfboard):

- *Schottky diodes*: x25. Any kind should work but choose low forward voltage ( $V_F$ ).
  Suggested: [1N4148](https://www.alldatasheet.com/datasheet-pdf/pdf/15021/PHILIPS/1N4148.html)
  ( $V_F=0.6V$ ).
- *Dupond pin headers* (male or female):
  - 25x2 (=50) for external inputs.
  - 5x1 (=5) for input pins.
  - 5x1 (=5) for selector pins.
- Thin wire.

Open the [circuit design](./BtnMatrix25Inputs.diy) using
[DIY Layout Creator](https://github.com/bancika/diy-layout-creator).
Inputs are externally wired to the light-blue pin header in columns.

![Circuit design 25 inputs](./BtnMatrix25Inputs.png)

### Button Matrix (16 inputs)

Needed parts (not counting input hardware like push buttons nor a perfboard):

- *Schottky diodes*: x16. Same as above.
- *Dupond pin headers* (male or female): x40.

This [circuit design](./BtnMatrix16Inputs.diy) requires no wiring but takes more space:

![Circuit design 16 inputs](./BtnMatrix16Inputs.png)

### Satellite circuit for potentiometers attached to a button matrix

The purpose of this circuit is to transform analog potentiometers
into "on/off" switches that can be wired to a button matrix.
**Useful for clutch paddles if no ADC-capable pins are available**.
Another soldered potentiometer (called "trimmer")
will calibrate the position where on/off switching happens.
Please, **follow this calibration procedure in order to minimize battery drainage**:

1. Move the trimmer to an end where the switch is always on, no matter which position the clutch paddle is.
2. Move and hold the clutch paddle to the desired position where it should switch on/off.
3. Slowly move the trimmer towards the other end until switch happens, but no more. If switch does not happen, swap left and right terminals of the clutch's potentiometer, then start again.
4. Forget. No further adjustment is required.

Since clutch paddles come in pairs, the following
[electrical circuit (falstad.com)](https://falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWEBmAHAJmgdgGzoRmACzICcpkORICkNNdCApgLRhgBQYW1CR1dPNVR0IYeHRg5UqCAGEANgFcALgGMAFpwDuIEunA466GYdEcA5iBOoz102FIHJHXUPAmQ022E-oOALJeDp5YCAa+tsbQRFw8esgRRnqkOB4+4BKS0GA4yERZhZAGACoaAE5MAM4aAPYKACYcADI0-NYIaXzUyJACUCAAZgCGClVMKNDIUBwAHjRYpCAsZCBYkNMradTiIADKTApMaiq15QA6VQAOAJYAdpfDKpcARqqnD1UAtk-lN7NzGhUZboAxYUHLRx6TIgAAK90u4yOJzOlwaTEuCmGlx+Kj+AC80Riqi9ah9qhx8kNtA1yrUrucAI5ScDgbDEZDhWROWTwdCsLADMAxVAUSCpBDEPr8BDTcwNLzoahEWzuSLWam0+lMqRxXjtXKCJXWAqs8TwKDQdDg3pFCTURSqTT+VrdAZulgqoVDUbjSZyjgKt2GlJpdUGQY0ukM5nQHCBtrUVKh9IayNamO6hXudCQVXG8Oa6M6uOuPTA9VEZPqyCAzoQXq2HgN9jQ3YAeTxtUut0+SOOpwuVXRmOx31+N0Jw+JpPJVTr4OsTjWiSXSB2dAAgncyRomEP+yih73Hs8Se9ap9cX8AfN8EgwBhrFhBY-1zCAOIAOQAIoCTBEYDTEQ6BiLkbZ0CUfwWBY+6XCoFTVHUjQ9mSTB3CoNy1F8TAqPu-64B4gokK+vQQXCaF3GoNwAM84V2RKXEoXwvOUoyMcONzXMMdKAusES0CAsjTGAsrkSU3bnioHw4hOsyXPcVyqD29yAjgWBiDgSCoGJuS2BuICbgoKmfCO4wYWxDRjmZWKyXik4cbOl4UvMZD6Ug4TAfpMKOuoGg9sMDQNEcqF4RhWH0fhrk4GkOAQOEgrAgZsKUdRdG4XSHFHAAbtZxJMCxbEWEoTAcC8KCCUBXh5kJAhlSgVbgE+RiCvgAaues1icjQyAQOgaDkSlYVpfRmXTpiTC5RxBWscMxWlfMBQGKBSwJdYEIGb5mgBUFIXXJRmHYbh+HlUqSzkAwr6dbWQA)
is suitable as long as both clutch's potentiometers **are identical**.
It is designed for 10K-ohms linear potentiometers,
but it will work with anything up to 50K-ohms.

![Dual clutch satellite circuit](./BtnMatrix2ClutchPaddles.png)

Needed parts (not counting the potentiometer) for one clutch paddle:

- A "vertically operated" 100K-ohms trimmer linear potentiometer.
  Any impedance, higher than 3 times that of the clutch's potentiometer, should work.
  These are some examples of trimmer potentiometers:
  [https://www.bourns.com/docs/technical-documents/technical-library/trimmers/publications/trimpot_handout.pdf](https://www.bourns.com/docs/technical-documents/technical-library/trimmers/publications/trimpot_handout.pdf)
  ![Trimmer potentiometers](./TrimmerPotentiometers.png)
- A *Schottky diode*: the same as for the button matrix.

Needed parts (not counting the potentiometers) for two clutch paddles:

- A "vertically operated" 100K-ohms trimmer linear potentiometer. Any impedance, higher than 3 times that of the clutch's potentiometer, should work.
- Four *Schottky diodes* : the same as for the button matrix.

Open this [circuit layout](./PotentiometerAsSwitch.diy) using [DIY Layout Creator](https://github.com/bancika/diy-layout-creator).

![Potentiometer as a switch](./PotentiometerAsSwitch_diy.png)

Note that, in case of need,
a momentary switch can substitute the potentiometer
using the two top pins (single clutch)
or the two right-most pins (dual clutch) at the blue header.

This hack is an alternative to this circuit: [https://www.instructables.com/Super-simple-potentiometer-switch-hack/](https://www.instructables.com/Super-simple-potentiometer-switch-hack/)

### Satellite circuit for a KY-040 (rotary encoder) built in push button

>[!NOTE]
> A bare bone rotary encoder is a better option since no satellite circuit is required.

The purpose of this circuit is to avoid the expenditure of a single GPIO pin for a single rotary encoder. Such an input is connected to the button matrix instead. This is the [electrical circuit at falstad.com](https://falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWcMBMcUHYMGZIA4UA2ATmIxAUgoqoQFMBaMMAKACdxiUQUUAWTtzB8BteJBYBnQT34zco8CAAubAK50WAc3mQBYLiGzZCUKCwBKM4QMxCRZqnyrZo2R1GgIWAdxm8BQls5FHYzAJAg2UUwcW0QBj48IxMEpKM8RQkAI3A+CGZTbBRk7BxzAA9wAxA8CGwEAUyQfWSLAHtlAEM2AE8AHUk6ADsAY3aAEzo2Qey1AEsAG2VB+eHZtWVlduGWKoxbBHcwYp4EU1aQAGUAdUGABzWWABk05LAShOYhT6oIADMuoshkY3OYALJfBz8KgMNDJGHhLx7BLYMDvNFfU4nAT6JBXOiLOijbYzSSPdaSLorSRzLY7QYAW2pbHmFRRDDRxHAlASdR57jxIAACmtBkMiST2mSRoNFl0mSz5gAvQZTWadHZ0SQc4rckpIBgYbglZJC0WU9VDYaqLoTBXWuUO5mqFVquga7bDbUclBkHhRJiQchEchCgCSw3umweYqpNOUAAsPbTNl7Fa72X5EgjnG8eHh3iiPgVIEVCH9INzLgAhToAZ8thMG4wm83+81GdulgzYnWp0vm7V8UNseZzPDzoQ4E9N+eMpkr8BYynCclnIS+oL9pFIxrw4hQ7jhsGwu7I-AwkGYfAwjHIsRAU0BamWI4nH2Ss-w4BYQA), which is a simple logic inverter, not counting the rectangle at the right side.

![Satellite circuit for a KY-040](./LogicInverter_falstad.png)

Needed parts (not counting the rotary encoder itself):

- Two 10K-ohms resistors. An higher impedance up to 100K-ohms should work.
- A bipolar junction transistor (x1), PNP type:
  any kind should work,
  for example: [BC640](https://www.onsemi.com/pdf/datasheet/bc640-d.pdf).
  Pay attention to the pinout.
  It *may not match* the one shown here.

Open this [circuit layout](./LogicInverter.diy) using
[DIY Layout Creator](https://github.com/bancika/diy-layout-creator).

![Logic inverter](./LogicInverter_diy.png)

### External wiring for the button matrix

- **Push buttons, push levers or roller levers**. They have two interchangeable terminals. If there are three terminals, choose the two terminals for the NO (normally open) switch. There is no common pole.

  - One terminal wired to a selector pin header at the button matrix.
  - Other terminal wired to an input pin header at the button matrix.

- **Built-in push button of a rotary encoder**:
  - **Barebone**: They are wired to the button matrix as any other push button, being `SW` and `SW GND` the involved terminals.
  - **KY-040**. Use the satellite circuit shown above.

- **Directional pads (DPADs) and funky switches (except for rotation)**. Terminals are **not** interchangeable:

  - The common terminal (`COM`) must be wired to a single input pin header at the button matrix.
  - Each other terminal (`A`, `B`, `C`, `D` and `Push`)
    wired to a different selector pin header at the button matrix.
    Be careful with the misleading tags in the datasheet.

- **Potentiometers**: Use the satellite circuit shown above.
  Terminals are **not** interchangeable.

## Analog multiplexer implementation

This implementation works with *any* analog multiplexer in the market (8, 16 or 32 channels).
If you have two or more chips in the circuit, all of them must be identical.

All switches work in *negative logic*, so their common pole must be attached to `GND`. See below.

### Circuit design using 16-channel multiplexers

The following circuit design is based on the widely available
[CD74HC4067E](../../esp32reference/CD74HC4067_datasheet.pdf)
analog multiplexer: an *16 to 1* multiplexer (16 channels).
Provides 32 inputs using 6 pins, which should be enough for most steering wheels.

![Multiplexed switches design (16-channel)](./MultiplexedSwitchesX32.png)

Open this [circuit layout](./MultiplexedSwitchesX32.diy) using [DIY Layout Creator](https://github.com/bancika/diy-layout-creator).

Needed parts (not counting input hardware like push buttons):

- Standard-sized perfboard 28x6 holes.
- CD74HC4067E analog multiplexer: x2.
- Dupond pin headers (male or female): x48.
- Thin wire.

### Circuit design using 8-channel multiplexers

The following circuit design is based on the widely available
[74HC4051N](../../esp32reference/75HC4051_datasheet.pdf)
analog multiplexer: an *8 to 1* multiplexer (8 channels).
Provides 24 inputs using 6 pins, which should be enough for most steering wheels.
However, this design can be extended easily:

- Add another analog multiplexer.
- Wire `S0`, `S1` and `S2` (selector pins) to the same pin tags at another multiplexer.
- Add a new input pin wired to `(A)`.

> [!NOTE]
> If  more than three 74HC4051N chips are needed,
> 16 or 32-channel multiplexers are a better choice.

![Multiplexed switches design (8-channel)](./MultiplexedSwitchesX24.png)

Open this [circuit layout](./MultiplexedSwitchesX24.diy) using [DIY Layout Creator](https://github.com/bancika/diy-layout-creator).

Needed parts (not counting input hardware like push buttons):

- Standard-sized perfboard 28x6 holes.
- 74HC4051N analog multiplexer: x3.
- Dupond pin headers (male or female): x42.
- Thin wire.

### External wiring for the analog multiplexers

- There are many redundant `3V3` and `GND` pin headers.
  Attach one of each to the power source.
  Use the others as you wish, or leave unattached.
- Use at least one of the `GND` pin headers as a common pole for all switches *in a chain*
  (as shown in the picture above).
- Wire the other terminal of each push button (or switch) to one of the light-blue pins.
- Bare bone rotary encoders: their built-in push button must be wired like any other push button,
  being `SW` and `SW GND` the involved terminals.
- KY-040 rotary encoders: wire `SW` to any light-blue pin.
- Funky switches (except for rotation): wire `COM` to `GND`.
  Wire `A`, `B`, `C`, `D` and `Push` to a light-blue pin header.
  Be careful with the misleading tags in the datasheet.

- Potentiometers as digital clutch paddles:
  build this [satellite circuit](https://falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWEBmAHAJmgdgGzoRmACzICcpkORICkNNdCApgLRhgBQA5jeqiOkj8EfAUToSOAJRq4QRYXIlR5dZNGQqJ0BBwCy81HVI4UkaiZWZdBnFnQhLqBY9N1rHMFmp5qLuw5gonRg8NoI5GFR8BAAwgA2AK4ALgDGABacAO68-C4Ici6QHDm+hrKmRZ7eueUKIUTUIWFQ0HzoDgkpGejV1CL8gvxlxNQQofCtyOhgRtFRmgAq6QBOTADO6QD28QAmJSAjjRXgx8U5BaajtdfFAB4gWLTgXiCoCJpBdGN0i1sAOut4gBLLjpZKAgBGSSYgIADsCAHaA9JMACGuyYKw4D3QpCwLwJ4hCgnk4F+AKBoPBUJh8KRKPRmOxDwQYFM6CoNDQAjsZNCIAA4gA5AAiOJoBAEeMO+IE035vxWoK4WMByVWG22e3hW2STERyWBWwAtkx9SzDshNDhSI8pfZFSAuml0vCMbt4rD1nC9QajabzViJWQCRFeA48E6XRl3btPd7ffrDcazRaOEA)
  which is quite simillar to that of the button matrix.
  All three potentiometers must have the same impedance.
  The higher, the better.
  The trigger threshold potentiometer is a trimmer one.
  Note that this satellite circuit will drain current at all times.

  ![Satellite circuit for clutch paddles](./Multiplexer2ClutchPaddles.png)

## Shift registers implementation

This implementation is based on the widely available
[74HC165N](../../esp32reference/SN74HC165_datasheet.pdf)
*parallel-in-serial-out* shift register of 8 bits.
The following circuit design provides **25 inputs using just 3 pins**,
which should be enough for most steering wheels.

![Shift register based design](./ShiftRegister.png)

Open this [circuit layout](./ShiftRegister.diy) using
[DIY Layout Creator](https://github.com/bancika/diy-layout-creator).
Hit `CTRL+5` to obtain a clear view of the back traces.

This is a "recursive" design.
Use it as a template to add more shift registers and more switches if you want.

Needed parts (not counting input hardware like push buttons):

- Standard-sized perfboard 24x10 holes.
- 74HC165N shift registers: x3.
- Dupond pin headers (male or female): x37.
- Resistors (any impedance from 1K-ohms to 10K-ohms): x25.
- Thin wire.

### External wiring for the shift registers

Just the same as for analog multiplexers.
See above.

## GPIO expanders implementation

This implementation is based on any of the following widely available GPIO expanders
(*I2C* interface):

- [PCF8574](../../esp32reference/PCF8574_datasheet.pdf):
  8 additional switches per chip.
- [MCP23017](../../esp32reference/MCP23017_datasheet.pdf):
  16 additional switches per chip (**recommended**).
  Do not confuse *MCP23017* with *MCP23S17*, which exposes a different interface.

All chips must share the `SCL` and `SDA` pins with the DevKit board.
Note that sometimes `SCL` is tagged as `SCK` (they are equivalent).
Additional **external** pull-up resistors may be needed at those pins
depending on wire capacitance, but that is unusual.
Follow [Expressif's advice](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html)
on the matter.

The `RESET` pin at the *MCP23017* must be wired to `3V3`, otherwise operation is unreliable.

All switches must work in negative logic.
Thus, one terminal must be wired to `GND` and the other terminal to one GPIO pin at each chip.
Those GPIO pins are tagged `Pn`, `GPAn` or `GPBn` on the chip's datasheet.

Make sure to configure a unique I2C address for each chip (more on this below).

All those considerations are met in the following circuit design.
It provides **32 inputs using just two pins**,
which should be enough for most steering wheels.

![MCP23017 switches design](./MCP23017Switches.png)

Open this [circuit layout](./MCP23017Switches.diy)
using [DIY Layout Creator](https://github.com/bancika/diy-layout-creator).

Needed parts (not counting input hardware like push buttons):

- Standard-sized perfboard 28x6 holes.
- MCP23017 GPIO expander: x2.
- Dupond pin headers (male or female): x50.
- Thin wire.

### External wiring for the GPIO expanders

- Attach `SCL` and `SCA` to the corresponding pins at the DevKit board (same tag).
- There are many redundant `GND` (ground) and `3V3` pin headers.
  Attach one of each to the DevKit board (same tag).
  Use the others as you wish, or leave unattached. A suggestion:
  - Redundant `GND` pins may be attached to switches (one terminal),
    rotary encoders (`COM` terminal) or potentiometers (right or left terminal).
  - Redundant `3V3` pins may be attached to potentiomers
    (right or left terminal, the opposite to `GND`).
- For components (switches, funky switches, etc.),
  follow the same rules as for analog multiplexers (see above).
- The `wake-up` pin is related to the
  [power latch subsystem](../PowerLatch/PowerLatch_en.md).
  For simplification,
  just the buttons attached to the left GPIO expander will wake up the system from deep sleep.
  This pin can be left unwired.

### I2C Addressing

Each chip must have a 7-bit-long **unique** address for the *I2C* interface.
Those bits are called a "**full** address".

Three of those bits are user-defined thanks to "address pins"
(tagged `A0`, `A1` and `A2` on the chip's datasheet).
They are called a "**hardware address**" and
match the three least-significant bits of the full address.

The other four bits are factory-defined and fixed.
The chip's datasheet shows the value of those bits.
**However, the actual factory-defined address bits may not match the datasheet**
(I have proof of that).
This could be caused by after-market changes or cloned (non-genuine) chips.
The firmware will **automatically** detect those 4 bits,
but **you must ensure** that all hardware addresses on the I2C bus are also **unique**.
Otherwise, you have to manually provide a full I2C address to the firmware (see below).

```mermaid
---
title: "I2C address"
---
block-beta
   columns 7
   A6 A5 A4 A3 A2 A1 A0
   FA["Factory address"]:4
   HW["Hardware address"]:3
   FL["Full I2C address (7 bits)"]:7
```

## Implementation of a single switch attached to a single pin

There is no circuit involved here, just wiring.
Attach one terminal to an input-capable GPIO pin.
Attach the other terminal to `GND`.
An internal pull-up resistor is required since we are using negative logic.
If not available, add an external pull-up resistor.

## Firmware customization

Customization takes place at the body of `simWheelSetup()` inside
[CustomSetup.ino](../../../../src/Firmware/CustomSetup/CustomSetup.ino).

**Important notes:**

- You should assign an "**input number**" to each switch.
- Valid input numbers are in the range of 0 to 63 (inclusive).
  The **firmware will not boot** if an invalid input number is detected.
- Any switch with no assigned input number **will be ignored** by the firmware without notice.
- Input numbers should be unique for each switch.
  You are allowed to assign the same input number to two or more different switches,
  but **there is no point in that**.
  By default, the firmware does not enforce unique input numbers.

### Button matrix

Input pins must be wired to valid input capable GPIO pins with internal pull-down resistors.
Otherwise, external pull-down resistors must be added to the circuit design.
Selector pins must be wired to valid output capable GPIO pins.

1. Create a `ButtonMatrix` object, let's say `mtx`.
2. This object has two dimensions (obvious, as it is a matrix).
   The first dimension is a **selector** pin.
   The second dimension is an **input** pin.
3. Assign an input number to both dimensions using this syntax:
   `mtx[<selector>][<input>] = <input number>`,
   where `<selector>` and `<input>` are pin numbers or aliases,
   and `<input number>` is an input number or alias.
4. Repeat as needed.
5. Call `inputs::addButtonMatrix()` and pass the object (`mtx`)
   as the first parameter.
6. Optionally, set the second parameter to `true` if your button matrix uses negative logic.
   Such a hardware design is not in this project,
   but this option allows you to use a recycled button matrix from another project.

> [!CAUTION]
> If, despite my advice, your button matrix does not have diodes,
> configure your custom firmware to work with negative logic
> by setting the second parameter to `true`.
> Otherwise, you will have a **short circuit**.
> Either way, you will have ghost inputs.

For example:

```c++
void simWheelSetup()
{
    ...
    ButtonMatrix mtx;
    mtx[GPIO_NUM_10][GPIO_NUM_12] = 1;
    mtx[GPIO_NUM_10][GPIO_NUM_13] = 2;
    mtx[GPIO_NUM_11][GPIO_NUM_12] = 3;
    mtx[GPIO_NUM_11][GPIO_NUM_13] = 4;
    inputs::addButtonMatrix(mtx);
    ...
}
```

For convenience, you can use `populateButtonMatrix()`
to assign the input numbers all at once with these parameters:

1. The `ButtonMatrix` object to populate.
2. The involved **selector** pins (or aliases) between braces.
3. The involved **input** pins (or aliases) between braces.
4. The first input number to be assigned.

For example (equivalent to the previous example):

```c++
void simWheelSetup()
{
    ...
    ButtonMatrix mtx;
    populateButtonMatrix(mtx, {GPIO_NUM_10, GPIO_NUM_10}, {GPIO_NUM_12, GPIO_NUM_13}, 1);
    inputs::addButtonMatrix(mtx);
    ...
}
```

### Analog multiplexers

Input pins must be wired to valid input capable GPIO pins with internal pull-up resistors.
Otherwise, external pull-up resistors must be added to the circuit design.
Selector pins must be wired to valid output capable GPIO pins.
Don't be confused by the "analog" label.
It is not necessary to use ADC-capable input pins.

1. Declare each multiplexer chip,
   passing the **input pin** as a constructor parameter.
   Use the classes
   `AnalogMultiplexerChip8`, `AnalogMultiplexerChip16` or `AnalogMultiplexerChip32`
   for 8, 16 and 32 channel chips respectively.
   For example: `AnalogMultiplexerChip8 chip1(GPIO_NUM_10);`
2. Assign input numbers to pin tags on each chip using the array syntax.
   For example: `chip1[Mux8Pin::A0] = 1;`.
   Qualify each pin tag with `Mux8Pin::`, `Mux16Pin::` or `Mux32Pin::`
   for 8, 16 and 32 channel chips respectively.
3. Place a call to `inputs::addAnalogMultiplexerGroup()`:
   - The first 3, 4 or 5 parameters,
     depending on the number of channels (8, 16 or 32 channels),
     are **selector pins**.
   - You can not mix 8, 16 and 32 channel chips.
     There is no room for error.
   - In the last parameter, group the chip instances
     between braces.

For example:

```c++
void simWheelSetup()
{
    AnalogMultiplexerChip8 chip1(13);
    AnalogMultiplexerChip8 chip2(14);
    chip1[Mux8Pin::A0] = 1;
    ...
    chip1[Mux8Pin::A7] = 7;
    chip2[Mux8Pin::A0] = 8;
    ...
    chip2[Mux8Pin::A8] = 16;
    inputs::addAnalogMultiplexerGroup(10,11,12,{chip1,chip2});
}
```

In the previous example:

- `10`, `11` and `12` are selector pins.
- `13` and `14` are input pins.

#### A note on digital multiplexers

Digital multiplexers are not recommended as
they require an external pull-up resistor for each switch.
However, the firmware will work with them.
This project does not provide a hardware design using digital multiplexers,
but you can reuse an existing circuit or design one yourself.
Just configure your custom firmware as you would for analogue multiplexers.

### Shift registers

`SERIAL` must be wired to a valid input capable GPIO,
but it does not require any pull resistor.
`LOAD` and `NEXT` must be wired to valid output capable GPIOs.

1. Declare each shift register chip with the class `ShiftRegisterChip`,
   for example: `ShiftRegisterChip chip1;`.
2. Assign an input number to each pin tag using the array syntax.
   For example: `chip1[SR8Pin::A] = 1;`.
   Qualify each pin tag with `SR8Pin::`.
3. Group these chips between braces.
   The position of each chip in the group is its position in the chain.
   The leftmost instance is the first.
4. Call `inputs::add74HC165NChain()` with the following parameters:
   - `Load` pin.
   - `Next` pin.
   - `Input` pin.
   - Group of chip instances.
   - Optionally, the input number assigned to the `SER`
     tag in the last chip in the chain.
     Pass `UNSPECIFIED::VALUE` (the default) if there is no switch attached to `SER`.
   - Optionally, `false` if your switches uses positive logic (pulled up).
     Such a hardware design is not in this project,
     but this option allows you to recycle hardware from another project.
     This parameter is `true` by default (pulled down).

For example:

```c++
void simWheelSetup()
{
    ShiftRegisterChip chip1, chip2;
    chip1[SR8Pin::A] = 1;
    chip1[SR8Pin::B] = 2;
    ...
    chip2[SR8Pin::A] = 8;
    ...
    chip2[SR8Pin::H] = 16;
    inputs::add74HC165NChain(12,13,14, {chip1,chip2}, 17);
}
```

### GPIO expanders

`SCL/SCK` and `SDA` pins must be wired to the corresponding pins at the DevKit board.

1. Declare each GPIO expander chip using the classes
   `MCP23017Expander` or `PCF8574Expander`.
   For example: `PCF8574Expander chip1;`.
2. Assign an input number to each pin tag using the array syntax.
   For example: `chip1[PCF8574Pin::P0] = 1;`.
   Qualify each pin tag with `PCF8574Pin::` or `MCP23017Pin::`.
3. Place a call to `inputs::addPCF8574Expander()` or `inputs::addMCP23017Expander()`
   depending on the chip.
   Make as many calls as needed.
   The parameters are as follows:
   - A chip instance,
   - An I2C (full or hardware) address.
   - Optionally, `true` for a full address, or `false` for a hardware address (default).
   - Optionally, an I2C bus (the default is the primary bus).

The following code will enable the example circuit above:

```c++
void simWheelSetup()
{
    ...
    MCP23017Expander chip1, chip2;
    chip1[MCP23017Pin::GPA0]=0;
    chip1[MCP23017Pin::GPA1]=1;
    ...
    chip1[MCP23017Pin::GPA7]=7;
    chip1[MCP23017Pin::GPB0]=8;
    chip1[MCP23017Pin::GPB1]=9;
    ...
    chip1[MCP23017Pin::GPB7]=15;
    chip2[MCP23017Pin::GPA0]=16;
    chip2[MCP23017Pin::GPA1]=17;
    ...
    chip2[MCP23017Pin::GPA7]=23;
    chip2[MCP23017Pin::GPB0]=24;
    chip2[MCP23017Pin::GPB1]=25;
    ...
    chip2[MCP23017Pin::GPB7]=31;
    // Note: hardware addresses are used
    inputs::addMCP23017Expander(chip1,7);
    inputs::addMCP23017Expander(chip2,0);
    ...
}
```

Be aware that **the firmware won't boot up** in the following cases:

- A GPIO expander is not powered or found on the I2C bus.
  Check your wiring first, then your code.
  Ensure you wrote the correct I2C address.
- The hardware address is not unique.
  Use a full address instead.

In case you have no clue about what the full address could be,
upload and run the
[I2C probe](../../../../src/Firmware/I2C_probe/I2C_probe.ino)
firmware provided in this project.
It will tell how many chips were found on the I2C bus and their full addresses.

Note that *hardware addresses* are in the range from 0 to 7 (inclusive),
while *full addresses* are in the range from 0 to 127 (inclusive).

#### I2C bus customization

The firmware will use the default `SDA` and `SCL` pins on your DevKit board.
If you prefer to use other pins,
you must **explicitly** place a call to
`inputs::initializeI2C()` with two parameters:

- The first parameter is the desired `SDA` pin.
- The second parameter is the desired `SCL` pin.

Both pins **must** support input, output and pull-up resistors.
This API function must be called **before**
`inputs::addPCF8574Expander()` or `inputs::addMCP23017Expander()`.

### Single switch

Place a call to `inputs::addButton()`:

- The first parameter is the GPIO where the switch is attached to.
- Second parameter: assigned input number for this button.

For example:

```c++
void simWheelSetup()
{
   ...
   inputs::addButton(GPIO_NUM_26, 1);
   ...
}
```

### Rotary coded switches

There is a *common pole* pin in your rotary coded switch,
which is usually tagged as `C`.
Sometimes this pin is duplicated, but the two pins are electrically connected.
You can therefore use either pin.

When you attach `C` to `GND`, you are said to be using *complementary code*,
which is most common and required for input hardware working in negative logic.
In a button matrix, attach `C` to a single input pin.

The position of the rotary switch is encoded in binary ("BCD" output code).
Each bit is assigned to a pin, typically labelled `1`, `2`, `4`, `8` or `16`,
depending on how many bits are used for encoding.
`1` is always the least significant bit.
Attach those pins to any input hardware in the *switches subsystem*.
In a button matrix, attach each bit pin to a selector pin.

1. Declare the input hardware where the bit pins are attached.
   You have to assign valid input numbers to those pins and
   all of them **must be different**
   (taking into account other coded switches as well).

2. Declare each coded switch instance using one of these classes:

   - `CodedSwitch8` for switches having 3 bit pins (up to 8 positions).
   - `CodedSwitch16` for switches having 4 bit pins (up to 16 positions).
   - `CodedSwitch32` for switches having 5 bit pins (up to 32 positions).

3. Assign an input number to each switch position using the array syntax.
   Each position is indexed starting with zero.
   To detect and prevent out of boundary indexes, use the operator `at()` instead of `[]`.
   You **can reuse** the input numbers assigned to the bit pins.

4. Place a call to `inputHub::codedSwitch::add()`
   and pass the following parameters from left to right:

   1. Input number assigned to `1`.
   2. Input number assigned to `2`.
   3. Input number assigned to `4`.
   4. Input number assigned to `8` (omit this parameter if the bit pin does not exist).
   5. Input number assigned to `16` (omit this parameter if the bit pin does not exist).
   6. The `CodedSwitch8`/`CodedSwitch16`/`CodedSwitch32` instance.

For example,
let's say you have a 12-position binary-coded rotary switch
having `C` attached to `GND`, and `1`, `2`, `4` and `8`
directly attached to the GPIO pins 11, 12, 13 and 14 respectively.

```c++
void simWheelSetup()
{
   ...
   // Create the input hardware for the rotary coded switch
   inputs::addButton(GPIO_NUM_11, 10);
   inputs::addButton(GPIO_NUM_12, 11);
   inputs::addButton(GPIO_NUM_13, 12);
   inputs::addButton(GPIO_NUM_14, 13);

   // Create the input numbers for the decoded positions
   // Note that we can reuse the input numbers 10 to 14
   CodedSwitch16 sw;
   sw[0] = 10; // array syntax
   sw.at(1) = 11; // Preferred syntax to detect out of bound indexes
   sw.at(2) = 12;
   sw.at(3) = 13;
   sw.at(4) = 14;
   sw.at(5) = 15;
   sw.at(6) = 16;
   sw.at(7) = 17;
   sw.at(8) = 18;
   sw.at(9) = 19;
   sw.at(10) = 20;
   sw.at(11) = 21;

   // Create the coded switch
   // as the input numbers 10, 11, 12 and 13 are already assigned to the bit pins
   inputHub::codedSwitch::add(10,11,12,13,sw);
   ...
}
```

Where `sw.at(7) = 17;` means the input number `17` is assigned
to the position index `7` in the rotary switch.
