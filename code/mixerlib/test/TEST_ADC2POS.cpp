#include "gtest/gtest.h"
#include "maths/maths.h"
#include "eeprom/eeprom.h"
extern struct EEPROM_Header header;

/**
 * ===============================================================
 * FIRST : Correct conversion
 * ===============================================================
 */

TEST(ADC2POS, CorrectConversion)
{
	header.gain = 1.00f;
	header.chan_gain[0] = 1.0f;
	header.chan_gain[1] = 1.0f;
	header.chan_gain[2] = 1.0f;
	header.chan_gain[3] = 1.0f;
	header.chan_gain[4] = 1.0f;
	header.offset = 0.00f;
	header.chan_offsets[0] = 0.00f;
	header.chan_offsets[1] = 0.00f;
	header.chan_offsets[2] = 0.00f;
	header.chan_offsets[3] = 0.00f;
	header.chan_offsets[4] = 0.00f;

	uint16_t t = 0;

	for (int k = 0; k < 100; k++)
	{
		ASSERT_EQ(ADC2POS(t, 1), k);
		t += 41;
	}
}

/**
 * ===============================================================
 * FIRST : Tests that include corrections
 * ===============================================================
 */

TEST(ADC2POS, SmallGain)
{
	header.gain = 0.5f;
	header.chan_gain[0] = 1.0f;
	header.chan_gain[1] = 1.0f;
	header.chan_gain[2] = 1.0f;
	header.chan_gain[3] = 1.0f;
	header.chan_gain[4] = 1.0f;
	header.offset = 0.00f;
	header.chan_offsets[0] = 0.00f;
	header.chan_offsets[1] = 0.00f;
	header.chan_offsets[2] = 0.00f;
	header.chan_offsets[3] = 0.00f;
	header.chan_offsets[4] = 0.00f;

	uint16_t t = 0;

	for (int k = 0; k < 100; k+=2)
	{
		ASSERT_EQ(ADC2POS(t, 1), k/2);
		t += 82;
	}
}

TEST(ADC2POS, Offset)
{
	header.gain = 1.00f;
	header.chan_gain[0] = 1.0f;
	header.chan_gain[1] = 1.0f;
	header.chan_gain[2] = 1.0f;
	header.chan_gain[3] = 1.0f;
	header.chan_gain[4] = 1.0f;
	header.offset = 0.033040293f;
	header.chan_offsets[0] = 0.00f;
	header.chan_offsets[1] = 0.00f;
	header.chan_offsets[2] = 0.00f;
	header.chan_offsets[3] = 0.00f;
	header.chan_offsets[4] = 0.00f;

	uint16_t t = 0;

	for (int k = 0; k < 100; k++)
	{
		ASSERT_EQ(ADC2POS(t, 1), k + 1);
		t += 41;
	}
}
