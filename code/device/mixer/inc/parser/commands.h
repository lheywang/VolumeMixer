/** ================================================================
 * @file    mixer/parser/commands.h
 *
 * @brief   This file define the commands structs to make easier the
 *          transfers
 *
 * @date 	31-05-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */

#include <stdint.h>

/* -----------------------------------------------------------------
 * COMMANDS
 * -----------------------------------------------------------------
 */

/**
 *  ASYNC COMMAND RX
 */
struct CMD_ASYNC_RX
{
    uint32_t appSlider1;
    uint32_t appSlider2;
    uint32_t appSlider3;
    uint32_t appSlider4;
    uint32_t appSlider5;
};

/**
 *  ASYNC COMMAND TX (Added field is the match one)
 */

struct CMD_ASYNC_TX
{
    struct
    {
        uint32_t appSlider;
        uint32_t appMatch;
    } slider1;

    struct
    {
        uint32_t appSlider;
        uint32_t appMatch;
    } slider2;

    struct
    {
        uint32_t appSlider;
        uint32_t appMatch;
    } slider3;

    struct
    {
        uint32_t appSlider;
        uint32_t appMatch;
    } slider4;

    struct
    {
        uint32_t appSlider;
        uint32_t appMatch;
    } slider5;
};

/**
 *  UICON struct
 */
struct CMD_UICON_RX
{
    uint32_t posSlider;
    uint32_t appSlider;
    uint8_t icon[128];
    uint32_t appStore;
};

/**
 *  SLPOS command
 */

struct CMD_SLPOS_TX
{
    struct
    {
        uint32_t Pos;
        uint32_t Mute;
    } slider1;

    struct
    {
        uint32_t Pos;
        uint32_t Mute;
    } slider2;

    struct
    {
        uint32_t Pos;
        uint32_t Mute;
    } slider3;

    struct
    {
        uint32_t Pos;
        uint32_t Mute;
    } slider4;

    struct
    {
        uint32_t Pos;
        uint32_t Mute;
    } slider5;
};

/**
 *  DCONF RX
 */
struct CMD_DCONF_RX
{
    uint8_t SN[8];
    double adcGain;
    uint16_t adcOffset;

    struct
    {
        uint16_t offset;
        double gain;
    } slider1;

    struct
    {
        uint16_t offset;
        double gain;
    } slider2;

    struct
    {
        uint16_t offset;
        double gain;
    } slider3;

    struct
    {
        uint16_t offset;
        double gain;
    } slider4;

    struct
    {
        uint16_t offset;
        double gain;
    } slider5;
};