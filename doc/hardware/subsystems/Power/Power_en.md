# Power subsystem

**You don't build this subsystem**, just buy one.

> [!CAUTION]
> Pay close attention to the wiring of any power supply (including batteries).
> Reverse polarity connection may cause fire or irreparable damage to the circuitry.
> If you notice a strange odor or traces of smoke,
> immediately disconnect all power sources.
> If you want to add revese polarity protection look at this article:
> [https://www.circuits.dk/reverse-current-battery-protection-circuits/](https://www.circuits.dk/reverse-current-battery-protection-circuits/).
> This subject is not covered here.

## Purpose

The purpose of this subsystem is:

- To provide a regulated power source of 3.3V to the rest of the system,
  no matter if it is powered through cable, batteries or both.
- If the system is powered through batteries:
  - To charge batteries while the USB cable is plugged in,
    even if the system is in use.
  - To prevent over charge of the batteries.
  - To prevent over discharge of the batteries.
  - To **prevent your batteries from catching fire or blowing up**.
  - To maximize battery life.

## Power requirements

The system requires a current supply of 500 mA or more from the power source.

## Understanding power plugs

One or more power sources must be attached to your DevKit board, but there are several plugs:

- **USB cable**.
- **`5V0` (or `5V`) and `GND` pins**, called "5V plug" for short.
- **`3V3` and `GND` pins**, called "3V plug" for short.
- **Built-in connector for rechargeable batteries** (where available).

And there are restrictions to them as described below.

### USB cable or 5V plug

The USB cable and the 5V plug are electrically connected, so they are just the same.
For that reason you must **never** attach any power source to both plugs simultaneously.

If a power source is attached, it will feed a built-in
[voltage regulator](https://en.wikipedia.org/wiki/Voltage_regulator)
which is connected to the 3V plug, so that plug becomes a power source itself.
For that reason, you must **never** attach another power source to the 3V plug simultaneously.
Otherwise, the DevKit board will get damaged.

If your DevKit board features a built-in battery connector,
the power source will also feed a built-in battery charger.

Any power source capable of providing a minimum of 5 volts and **up to 12 volts** will work
(depends on the particular DevKit board),
even if the voltage is not steady.
However, the higher the voltage, the higher the waste of power and the higher the heat dissipated.
No more than 7 volts is recommended.

> [!CAUTION]
> Some boards designed by *Unexpected Maker* are limited to exactly 5 volts.
> Higher voltages will damage those boards.
> **Always check the manufacturer's data sheet**.

### 3V plug

As stated above, if you attach a power source to this plug,
you must not attach any other power source to other plugs.
That power source must comply with an extra requirement:
a steady and constant voltage between 3.0V and 3.3V, which is uncommon.
However, this is the case when using a powerboost module (see below).

It is unlikely that such a power source can make any built-in battery charger work.

### Battery conector

If your DevKit board features a battery connector,
an external battery will work as a secondary power source.
Do not assume any battery will work. Look for a data sheet.
If your DevKit board does not feature a battery connector,
you may still use batteries by purchasing a powerboost module (see below).

## Power from a powerboost module

A powerboost module/shield (also known as a "power bank")
combines a battery pack with a battery charger and a voltage regulator.
This device will act as a power source for the DevKit board,
either by attaching it to the 5V plug or the 3V plug.

### Buying guide

Ensure you buy nothing but a *powerboost module/shield* or a *power bank*.
A simple battery charger or any other circuit **is dangerous** even if it works.
The requirements are:

- Current supply of 500mA or more (1A is recommended).
- Capable of 3.0V to 3.3V output voltage.
  Some powerboost modules provide both 5.0V and 3.3V.

  If your powerboost module is 5.0V-capable,
  but not 3.3V-capable, you can still use it.
  Plug the 5.0V output to the 5V0 pin at the DevKit board.
  This works, but it is **a waste of power**
  (there are two voltage regulators in action),
  thus not recommended.

- Battery pack with a voltage of 3.2V to 4.3V,
  which is called "1S".
  This is, if there are two or more battery cells,
  they are connected in parallel, not in series.

It is highly recommended, but not mandatory,
for the powerboost module to come equipped with a
[power latch circuit](../PowerLatch/PowerLatch_en.md).
You now there is a power latch circuit
if there is a power on/off momentary push button,
but not a (non-momentary) switch.

Check which kind of battery is suitable for the chosen powerboost module and buy one.
Keep in mind how much space is available inside the wheel case to fit all the components,
so don't buy an oversized powerboost module.

Some ESP32-based DevKits already integrate a powerboost module,
such as the [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405).
Adafruit also offers two stand-alone powerboost modules (a bit overpriced, in my opinion),
but not 3.3V-capable.

This is the powerboost module used for testing in this project (note there is a latch circuit):
![module used for testing](./BatteryShieldSpecs.png)

#### Battery capacity

Choose your battery capacity according to the amount of continuous use you require.
Assuming no telemetry display hardware or LED lights are used,
the system will draw approximately
[150mA of current for BLE operation](https://www.luisllamas.es/en/esp32-power-consumption/)
(this is a very rough estimate).
The nominal voltage of a typical 1S lithium battery is 3.7 volts.
Using this data, it is possible to estimate the battery capacity required.
Use this [online calculator](https://whycalculator.com/battery-run-time-calculator/).

| 3.7V LiPo standard capacity | Roughly estimated run time |
| --------------------------- | -------------------------- |
| 1000 mAh                    | 6 hours and 40 minutes     |
| 1500 mAh                    | 10 hours                   |
| 2000 mAh                    | 13 hours and 20 minutes    |
| 2500 mAh                    | 16 hours and 40 minutes    |
| 3000 mAh                    | 20 hours                   |
| 4000 mAh                    | 26 hours and 40 minutes    |

## Firmware customization

Nothing is required.

## Brownout

If you are using batteries to power the system during boot-up, you can have a critical brownout.
The most likely culprits are **poor electrical connections or faulty welding**.

Brownout causes a never-ending boot-up loop.
Since there is no USB cable,
you cannot check for error messages.

However, you may look for the following symptoms thanks to on-board LED lights (if any):

- Dimmed power light.
- Flickering power light.
- Never-ending flashing TX light (UART transmission at all times).

## Further reading

For an explanation on how how to power the DevKit board, take a look at these articles:

- [How to power your ESP32 dev kit](https://techexplorations.com/guides/esp32/begin/power/)
- [What is the best battery for the ESP32?](https://diyi0t.com/best-battery-for-esp32/)
- [Power Supply for ESP32 with Battery Charger & Boost Converter](https://how2electronics.com/power-supply-for-esp32-with-boost-converter-battery-charger/)
