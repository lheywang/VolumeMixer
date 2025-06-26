#include "gtest/gtest.h"
#include "utils/lstrtof.h"

/**
 * ===============================================================
 * FIRST : Error TEST
 * ===============================================================
 */

TEST(LSTRTOF, NullString)
{
	char buf[12] = {0};
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 0.0f);
	ASSERT_EQ(buf, tmp);
}

/**
 * ===============================================================
 * SECOND : Operationnal caster
 * ===============================================================
 */

TEST(LSTRTOF, OneDotZeroFive)
{
	char buf[12] = "+1.050";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 1.050f);
	ASSERT_EQ(&buf[5], tmp);
}

TEST(LSTRTOF, DotTwoFiveSix)
{
	char buf[12] = "0.256";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 0.256f);
	ASSERT_EQ(&buf[4], tmp);
}

TEST(LSTRTOF, MinusOneDotFiveFourThree)
{
	char buf[12] = "-1.543";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, -1.543f);
	ASSERT_EQ(&buf[5], tmp);
}

TEST(LSTRTOF, LowerRange)
{
	char buf[12] = "-9.999";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, -9.999f);
	ASSERT_EQ(&buf[5], tmp);
}

TEST(LSTRTOF, UpperRange)
{
	char buf[12] = "9.999";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 9.999f);
	ASSERT_EQ(&buf[4], tmp);
}

TEST(LSTRTOF, Zero)
{
	char buf[12] = "0";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 0.0f);
	ASSERT_EQ(buf, tmp);
}

/**
 * ===============================================================
 * THIRD : BUFFER OVERFLOWS (Depend on some config file !!!)
 * ===============================================================
 */

TEST(LSTRTOF, OutOfRange1)
{
	char buf[12] = "9999.999999";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 0.0f);
	ASSERT_EQ(buf, tmp);
}

TEST(LSTRTOF, OutOfRange2)
{
	char buf[12] = "9.9999999";
	char *tmp;

	float val = lstrtof(buf, &tmp);

	ASSERT_FLOAT_EQ(val, 0.0f);
	ASSERT_EQ(buf, tmp);
}
