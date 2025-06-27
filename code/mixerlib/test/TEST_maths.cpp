#include "gtest/gtest.h"
#include "maths/maths.h"

/**
 * ===============================================================
 * FIRST : Invalids values
 * ===============================================================
 */

TEST(MATHS, ADC2DoubleInvalidValue)
{
	// Invalid buffer test
	uint16_t code = 5000;
	float val = 0.0f;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, -1);
}

TEST(MATHS, DoubleToADCInvalidValue)
{
	// Invalid buffer test
	uint16_t code = 0;
	float val = -0.1f;

	int retval = Double2ADC(val, &code);
	ASSERT_EQ(retval, -1);
}

TEST(MATHS, DoubleToADCInvalidValue2)
{
	// Invalid buffer test
	uint16_t code = 0;
	float val = 3.4f;

	int retval = Double2ADC(val, &code);
	ASSERT_EQ(retval, -1);
}

TEST(MATHS, DoubleToS16InvalidValue)
{
	// Invalid buffer test
	int8_t code = 0;
	float val = -1.6f;

	int retval = Double2S16(val, &code);
	ASSERT_EQ(retval, -1);
}

TEST(MATHS, DoubleToS16InvalidValue2)
{
	// Invalid buffer test
	int8_t code = 0;
	float val = 1.6f;

	int retval = Double2S16(val, &code);
	ASSERT_EQ(retval, -1);
}

/**
 * ===============================================================
 * SECOND : Conversion tests
 * ===============================================================
 */

/**
 * ADC 2 Double
 */
TEST(MATHS, ADC2DoubleZero)
{
	uint16_t code = 0;
	float val;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, 0);
	EXPECT_NEAR(val, 0.0f, 0.001f);
}

TEST(MATHS, ADC2DoubleOne)
{
	uint16_t code = 1241;
	float val;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, 0);
	EXPECT_NEAR(val, 1.0f, 0.001f);
}

TEST(MATHS, ADC2DoubleTwo)
{
	uint16_t code = 2482;
	float val;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, 0);
	EXPECT_NEAR(val, 2.0f, 0.001f);
}

TEST(MATHS, ADC2DoubleThree)
{
	uint16_t code = 3723;
	float val;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, 0);
	EXPECT_NEAR(val, 3.0f, 0.001f);
}

TEST(MATHS, ADC2DoubleThreeDotThree)
{
	uint16_t code = 4095;
	float val;

	int retval = ADC2Double(code, &val);

	ASSERT_EQ(retval, 0);
	EXPECT_NEAR(val, 3.3f, 0.001f);
}

/**
 * Double 2 ADC
 */

TEST(MATHS, Double2ADCZero)
{
	uint16_t code = 0;
	float val = 0.0f;

	int retval = Double2ADC(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 0);
}

TEST(MATHS, Double2ADCOne)
{
	uint16_t code = 0;
	float val = 1.0f;

	int retval = Double2ADC(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 1241);
}

TEST(MATHS, Double2ADCTwo)
{
	uint16_t code = 0;
	float val = 2.0f;

	int retval = Double2ADC(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 2482);
}

TEST(MATHS, Double2ADCThree)
{
	uint16_t code = 0;
	float val = 3.0f;

	int retval = Double2ADC(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 3723);
}

TEST(MATHS, Double2ADCThreeDotThree)
{
	uint16_t code = 0;
	float val = 3.3f;

	int retval = Double2ADC(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 4095);
}

/**
 * S16 2 Double
 */

TEST(MATHS, S162DoubleZero)
{
	int8_t code = 0;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, 0.0f);
}

TEST(MATHS, S162DoubleOne)
{
	int8_t code = 64;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, 1.0f);
}

TEST(MATHS, S162DoubleMinusOne)
{
	int8_t code = -64;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, -1.0f);
}

TEST(MATHS, S162DoubleOneDotFive)
{
	int8_t code = 96;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, 1.5f);
}

TEST(MATHS, S162DoubleMinusOneDotFive)
{
	int8_t code = -96;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, -1.5f);
}

TEST(MATHS, S162DoubleDotFive)
{
	int8_t code = 32;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, 0.5f);
}

TEST(MATHS, S162DoubleMinusDotFive)
{
	int8_t code = -32;
	float val;

	int retval = S162Double(code, &val);

	ASSERT_EQ(retval, 0);
	ASSERT_FLOAT_EQ(val, -0.5f);
}

/**
 * Double 2 S16
 */

TEST(MATHS, Double2S16Zero)
{
	int8_t code;
	float val = 0.0f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 0);
}

TEST(MATHS, Double2S16One)
{
	int8_t code;
	float val = 1.0f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 64);
}

TEST(MATHS, Double2S16MinusOne)
{
	int8_t code;
	float val = -1.0f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, -64);
}

TEST(MATHS, Double2S16OneDotFive)
{
	int8_t code;
	float val = 1.5f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 96);
}

TEST(MATHS, Double2S16MinusOneDotFive)
{
	int8_t code;
	float val = -1.5f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, -96);
}

TEST(MATHS, Double2S16DotFive)
{
	int8_t code;
	float val = 0.5f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, 32);
}

TEST(MATHS, Double2S16MinusDotFive)
{
	int8_t code;
	float val = -0.5f;

	int retval = Double2S16(val, &code);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(code, -32);
}




