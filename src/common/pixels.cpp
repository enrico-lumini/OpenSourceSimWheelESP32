/**
 * @file Pixels.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain.
 * @date 2024-12-13
 * @brief Everything related to pixel control.
 *
 * @copyright Licensed under the EUPL
 *
 */

//---------------------------------------------------------------
// Imports
//---------------------------------------------------------------

#include "SimWheel.hpp"
#include "SimWheelInternals.hpp"
#include "InternalServices.hpp"
#include "OutputHardware.hpp"

#include <mutex>
#include <chrono>
#include <thread>

//---------------------------------------------------------------
// Globals
//---------------------------------------------------------------

static LEDStrip *pixelData[3] = {nullptr};
static std::recursive_timed_mutex pixelMutex;
#define DELAY_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define WAIT_MS std::chrono::milliseconds(80)
#define INT(N) ((uint8_t)N)
#define CEIL_DIV(dividend, divisor) (dividend + divisor - 1) / divisor

//---------------------------------------------------------------
//---------------------------------------------------------------
// Public
//---------------------------------------------------------------
//---------------------------------------------------------------

void pixels::configure(
    PixelGroup group,
    OutputGPIO dataPin,
    uint8_t pixelCount,
    bool useLevelShift,
    PixelDriver pixelType,
    PixelFormat pixelFormat,
    uint8_t globalBrightness)
{
    if (pixelData[INT(group)] != nullptr)
        throw std::runtime_error("A pixel group was configured twice");
    pixelData[INT(group)] = new LEDStrip(
        dataPin,
        pixelCount,
        useLevelShift,
        pixelType,
        pixelFormat);
    pixelData[INT(group)]->brightness(globalBrightness);
}

//---------------------------------------------------------------
//---------------------------------------------------------------
// Internal
//---------------------------------------------------------------
//---------------------------------------------------------------

void pixelsShutdown()
{
    // NOTE: no timeouts here.
    // Shutdown is mandatory.
    pixelMutex.lock();
    for (int i = 0; i < 3; i++)
        if (pixelData[i])
        {
            pixelData[i]->clear();
            pixelData[i]->show();
            delete pixelData[i];
            pixelData[i] = nullptr;
        }
    pixelMutex.unlock();
}

//---------------------------------------------------------------

void internals::pixels::getReady()
{
    OnShutdown::subscribe(pixelsShutdown);
}

//---------------------------------------------------------------

uint8_t internals::pixels::getCount(PixelGroup group)
{
    if (pixelData[INT(group)] != nullptr)
        return pixelData[INT(group)]->getPixelCount();
    else
        return 0;
}

//---------------------------------------------------------------
// Pixel control
//---------------------------------------------------------------

void internals::pixels::set(
    PixelGroup group,
    uint8_t pixelIndex,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    if (pixelData[INT(group)] && pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelData[INT(group)]->pixelRGB(pixelIndex, red, green, blue);
        pixelMutex.unlock();
    }
}

void internals::pixels::setAll(
    PixelGroup group,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    if (pixelData[INT(group)] && pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelData[INT(group)]->pixelRangeRGB(0, 255, red, green, blue);
        pixelMutex.unlock();
    }
}

void internals::pixels::shiftToNext(PixelGroup group)
{
    if (pixelData[INT(group)] && pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelData[INT(group)]->shiftToNext();
        pixelMutex.unlock();
    }
}

void internals::pixels::shiftToPrevious(PixelGroup group)
{
    if (pixelData[INT(group)] && pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelData[INT(group)]->shiftToPrevious();
        pixelMutex.unlock();
    }
}

void internals::pixels::reset()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        for (int i = 0; i < 3; i++)
            if (pixelData[i])
            {
                pixelData[i]->pixelRangeRGB(0, 255, 0, 0, 0);
                pixelData[i]->show();
            }
        pixelMutex.unlock();
    }
}

void internals::pixels::show()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        for (int i = 0; i < 3; i++)
            if (pixelData[i])
            {
                pixelData[i]->show();
            }
        pixelMutex.unlock();
    }
}

//---------------------------------------------------------------
//---------------------------------------------------------------
// Notifications
//---------------------------------------------------------------
//---------------------------------------------------------------

bool PixelControlNotification::renderBatteryLevel(
    PixelGroup group,
    bool colorGradientOrPercentage,
    uint32_t barColor)
{
    if (BatteryService::call::hasBattery())
    {
        int soc = BatteryService::call::getLastBatteryLevel();
        if (colorGradientOrPercentage)
        {
            // Color gradient
            uint8_t green = (255 * soc) / 100;
            internals::pixels::setAll(group, 255 - green, green, 0);
        }
        else
        {
            // Percentage bar
            uint8_t pixelCount = internals::pixels::getCount(group);
            uint8_t litCount = (soc * pixelCount) / 100;
            uint8_t blue = barColor;
            uint8_t green = (barColor >> 8);
            uint8_t red = (barColor >> 16);
            for (uint8_t pixelIndex = 0; pixelIndex < litCount; pixelIndex++)
                internals::pixels::set(group, pixelIndex, red, green, blue);
        }
        return true;
    }
    return false;
}

void PixelControlNotification::onStart()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnStart();
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnStart()
{
    if (renderBatteryLevel(PixelGroup::GRP_TELEMETRY, false))
    {
        // Show battery level
        renderBatteryLevel(PixelGroup::GRP_BUTTONS, true);
        renderBatteryLevel(PixelGroup::GRP_INDIVIDUAL, true);
    }
    else
    {
        // There is no battery
        // All white
        internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 85, 85, 85);
        internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 85, 85, 85);
        internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 85, 85, 85);
    }
    internals::pixels::show();
    DELAY_MS(1500);
}

//---------------------------------------------------------------

void PixelControlNotification::set(
    PixelGroup group,
    uint8_t pixelIndex,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    internals::pixels::set(group, pixelIndex, red, green, blue);
}

void PixelControlNotification::setAll(
    PixelGroup group,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    internals::pixels::setAll(group, red, green, blue);
}

void PixelControlNotification::shiftToNext(PixelGroup group)
{
    internals::pixels::shiftToNext(group);
}

void PixelControlNotification::shiftToPrevious(PixelGroup group)
{
    internals::pixels::shiftToPrevious(group);
}

//---------------------------------------------------------------

void PixelControlNotification::onBitePoint(uint8_t bitePoint)
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnBitePoint(bitePoint);
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnBitePoint(uint8_t bitePoint)
{
    uint8_t pixelCount = internals::pixels::getCount(PixelGroup::GRP_TELEMETRY);
    uint8_t litCount = CEIL_DIV(bitePoint * pixelCount, CLUTCH_FULL_VALUE);
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 0, 0, 0);
    internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 0, 0, 0);
    internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 0, 0, 0);
    for (int i = 0; i < litCount; i++)
        internals::pixels::set(PixelGroup::GRP_TELEMETRY, i, 85, 85, 0);
    internals::pixels::show();
    DELAY_MS(250);
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 0, 0, 0);
    internals::pixels::show();
}
//---------------------------------------------------------------

void PixelControlNotification::onConnected()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnConnected();
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnConnected()
{
    internals::pixels::reset();
}

//---------------------------------------------------------------

void PixelControlNotification::onBLEdiscovering()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnBLEdiscovering();
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnBLEdiscovering()
{
    // All purple
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 85, 0, 85);
    internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 85, 0, 85);
    internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 85, 0, 85);
    internals::pixels::show();
    DELAY_MS(250);
}

//---------------------------------------------------------------

void PixelControlNotification::onLowBattery()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnLowBattery();
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnLowBattery()
{
    // Alternate pixels in blue and red
    for (int group = 0; group < 3; group++)
    {
        uint8_t pixelCount = internals::pixels::getCount((PixelGroup)group);
        for (int pixelIndex = 0; pixelIndex < pixelCount; pixelIndex = pixelIndex + 2)
            internals::pixels::set((PixelGroup)group, pixelIndex, 127, 0, 0); // red
        for (int pixelIndex = 1; pixelIndex < pixelCount; pixelIndex = pixelIndex + 2)
            internals::pixels::set((PixelGroup)group, pixelIndex, 0, 0, 127); // blue
    }
    internals::pixels::show();
    // Cool animation
    for (int animCount = 0; animCount < 5; animCount++)
    {
        DELAY_MS(200);
        internals::pixels::shiftToNext(PixelGroup::GRP_TELEMETRY);
        internals::pixels::shiftToNext(PixelGroup::GRP_BUTTONS);
        internals::pixels::shiftToNext(PixelGroup::GRP_INDIVIDUAL);
        internals::pixels::show();
    }
    DELAY_MS(200);
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 0, 0, 0);
    internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 0, 0, 0);
    internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 0, 0, 0);
    internals::pixels::show();
}

//---------------------------------------------------------------

void PixelControlNotification::onSaveSettings()
{
    if (pixelMutex.try_lock_for(WAIT_MS))
    {
        pixelControl_OnSaveSettings();
        pixelMutex.unlock();
    }
}

void PixelControlNotification::pixelControl_OnSaveSettings()
{
    // All green
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 0, 85, 0);
    internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 0, 85, 0);
    internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 0, 85, 0);
    internals::pixels::show();
    DELAY_MS(150);
    // All off
    internals::pixels::reset();
    DELAY_MS(150);
    // All green
    internals::pixels::setAll(PixelGroup::GRP_TELEMETRY, 0, 85, 0);
    internals::pixels::setAll(PixelGroup::GRP_BUTTONS, 0, 85, 0);
    internals::pixels::setAll(PixelGroup::GRP_INDIVIDUAL, 0, 85, 0);
    internals::pixels::show();
    DELAY_MS(150);
    // All off
    internals::pixels::reset();
}