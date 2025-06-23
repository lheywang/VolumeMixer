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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
struct CMD_ASYNC_TX
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

struct CMD_ASYNC_RX
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
struct CMD_UICON_TX
{
    uint32_t posSlider;
    uint32_t appSlider;
    uint8_t icon[128];
    uint32_t appStore;
};

/**
 *  SLPOS command
 */

struct CMD_SLPOS_RX
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
struct CMD_DCONF_TX
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

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */
/**
 * All of theses function exploit return values that does not interfer with the parser function.
 * This facilitate debugging the parsing module.
 */

/**
 *	ASYNC Command functions
 */
/**
 * @brief 	Parse the payload of an async command, and fill a struct with the rights elements.
 *
 * @param 	buf 	The input buffer to be parsed.
 * @param 	cmd 	The struct to be filled.
 *
 * @return 	int
 * @retval	  0 	Everything went fine
 */
int parse_async_payload(const char * buf, struct CMD_ASYNC_TX * const cmd);
/**
 * @brief 	Build the buffer output for the async command, with the updated values.
 *
 * @param 	cmd 	The command to be built.
 * @param 	buf 	The output buffer.
 * @param 	len 	The lenght of the buffer.
 */
int build_async_payload(const struct CMD_ASYNC_RX * const cmd, const char *buf, const int * len);
/**
 * UICON Command functions
 */
/**
 * @brief 	Parse the payload of an async command, and fill a struct with the rights elements.
 *
 * @param 	buf 	The input buffer to be parsed.
 * @param 	cmd 	The struct to be filled.
 *
 * @return 	int
 * @retval	  0 	Everything went fine
 * @retval 	- 1 	Invalid pointer passed
 * @retval 	-20		JSON Header not found
 * @retval  -21		Unable to cast the slider value
 * @retval  -22		Invalid slider position
 * @retval  -23		Unable to find the AppName JSON Header
 * @retval  -24		Unable to find the AppName hash
 * @retval  -25		Unable to find the Icon JSON Header
 * @retval  -26		Unable to read the icon values
 * @retval  -27		Unable to find the store JSON Header
 * @retval  -28		Unable to cast the store value to an integer
 */
int parse_uicon_payload(const char * buf, struct CMD_UICON_TX * const cmd);

/**
 * SLPOS Command functions
 */
/**
 * @brief 	Build the buffer output for the slpos command, with the updated values.
 *
 * @param 	cmd 	The command to be built.
 * @param 	buf 	The output buffer.
 * @param 	len 	The lenght of the buffer.
 */
int build_slpos_payload(const struct CMD_SLPOS_RX * const cmd, const char *buf, const int * len);

/**
 * DCONF Command functions
 */
/**
 * @brief 	Parse the payload of an async command, and fill a struct with the rights elements.
 *
 * @param 	buf 	The input buffer to be parsed.
 * @param 	cmd 	The struct to be filled.
 *
 * @return 	int
 * @retval	  0 	Everything went fine
 */
int parse_dconf_payload(const char * buf, struct CMD_DCONF_TX * const cmd);

#ifdef __cplusplus
}
#endif
