/**
 * @file CustomSetup.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain.
 * @date 2022-03-01
 * @brief Custom setup. Please, read
 *        [How to customize](../../../doc/hardware/CustomizeHowto_en.md)
 *
 * @copyright Licensed under the EUPL
 *
 */

#include "SimWheel.hpp"
#include "SimWheelUI.hpp"

#include "SimWheelTypes.hpp"

// #include <Arduino.h> // For debugging

//------------------------------------------------------------------
// Global customization
//------------------------------------------------------------------

/* -----------------------------------------------------------------
 >>>> [EN] DEVICE IDENTIFICATION
 >>>> [ES] IDENTIFICACIÓN DEL DISPOSITIVO
------------------------------------------------------------------ */

// [EN] Set a name for this device between double quotes
// [ES] Indique un nombre para este dispositivo entre comillas

std::string DEVICE_NAME = "KLC PRO 1";

// [EN] Set a manufacturer's name for this device between double quotes
// [ES] Indique un nombre para el fabricante de este dispositivo entre comillas

std::string DEVICE_MANUFACTURER = "EL";

//------------------------------------------------------------------
// Globals
//------------------------------------------------------------------
#define ROT1_CW 33
#define ROT1_CCW ROT1_CW + 1

//------------------------------------------------------------------
// Setup
//------------------------------------------------------------------

void simWheelSetup()
{
    ButtonMatrix mtx;
    
    // Right buttons
    mtx[GPIO_NUM_2][GPIO_NUM_7] = 1;
    mtx[GPIO_NUM_3][GPIO_NUM_7] = 2;
    mtx[GPIO_NUM_4][GPIO_NUM_7] = 3;
    mtx[GPIO_NUM_5][GPIO_NUM_7] = 4;
    mtx[GPIO_NUM_6][GPIO_NUM_7] = 5;

    // Left buttons
    mtx[GPIO_NUM_2][GPIO_NUM_8] = 6;
    mtx[GPIO_NUM_3][GPIO_NUM_8] = 7;
    mtx[GPIO_NUM_4][GPIO_NUM_8] = 8;
    mtx[GPIO_NUM_5][GPIO_NUM_8] = 9;
    mtx[GPIO_NUM_6][GPIO_NUM_8] = 10;

    // Encoder buttons
    mtx[GPIO_NUM_2][GPIO_NUM_9] = 11;
    mtx[GPIO_NUM_3][GPIO_NUM_9] = 12;
    mtx[GPIO_NUM_4][GPIO_NUM_9] = 13;
    mtx[GPIO_NUM_5][GPIO_NUM_9] = 14;
    mtx[GPIO_NUM_6][GPIO_NUM_9] = 15;

    inputs::addButtonMatrix(mtx);

    inputs::addRotaryEncoder(GPIO_NUM_10, GPIO_NUM_11, ROT1_CW, ROT1_CCW);
    inputs::addRotaryEncoder(GPIO_NUM_21, GPIO_NUM_47, ROT1_CW + 2, ROT1_CCW + 2);
    inputs::addRotaryEncoder(GPIO_NUM_12, GPIO_NUM_13, ROT1_CW + 4, ROT1_CCW + 4);
    inputs::addRotaryEncoder(GPIO_NUM_14, GPIO_NUM_15, ROT1_CW + 6, ROT1_CCW + 6);
    inputs::addRotaryEncoder(GPIO_NUM_16, GPIO_NUM_17, ROT1_CW + 8, ROT1_CCW + 8);

    // inputs::addButton(GPIO_NUM_34, 33);
    // inputs::addButton(GPIO_NUM_33, 34);
    // inputs::addButton(GPIO_NUM_27, 35);

    // inputHub::clutch::inputs(33, 34);
    // inputHub::clutch::bitePointInputs(31, 32); // Rotary 4
    // inputHub::dpad::inputs(25, 26, 27, 28);
    // inputHub::altButtons::inputs({35});
}

//------------------------------------------------------------------

void customFirmware()
{
    simWheelSetup();
    hid::configure(
        DEVICE_NAME,
        DEVICE_MANUFACTURER,
        false);
}

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    
    Serial.println("Test setup");

    firmware::run(customFirmware);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}