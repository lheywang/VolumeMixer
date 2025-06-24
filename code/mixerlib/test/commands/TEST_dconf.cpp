#include "gtest/gtest.h"
#include "parser/parser.h"
#include "parser/commands.h"

/**
 * ===============================================================
 * FIRST : DCONF parser
 * ===============================================================
 */

TEST(DCONF, CorrectParsing)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, 0);

	ASSERT_FLOAT_EQ(command.slider1.gain, 1.01);
	ASSERT_FLOAT_EQ(command.slider1.offset, 0.082);

	ASSERT_FLOAT_EQ(command.slider2.gain, 1.09);
	ASSERT_FLOAT_EQ(command.slider2.offset, 0.029);

	ASSERT_FLOAT_EQ(command.slider3.gain, 1.00);
	ASSERT_FLOAT_EQ(command.slider3.offset, 0.050);

	ASSERT_FLOAT_EQ(command.slider4.gain, 1.12);
	ASSERT_FLOAT_EQ(command.slider4.offset, 0.000);

	ASSERT_FLOAT_EQ(command.slider5.gain, 0.98);
	ASSERT_FLOAT_EQ(command.slider5.offset, -0.012);

	ASSERT_FLOAT_EQ(command.adcGain, 1.09);
	ASSERT_FLOAT_EQ(command.adcOffset, 0.012);

	uint8_t buf2[] = "25MYH0D0";
	for (int k = 0; k < 8; k++)
	{
		ASSERT_EQ(command.SN[k], buf2[k]);
	}
}

TEST(DCONF, BufferTooShort)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1FF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -2);
}

TEST(DCONF, InvalidJSONHeader)
{
	// Invalid buffer test
	char buf[] = "{\"caI\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -30);
}

TEST(DCONF, InvalidChannelData)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"40FF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -31);
}

TEST(DCONF, ChannelNAN)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"zOFF\":\"+0.082\",\"nG\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -32);
}

TEST(DCONF, ChannelDoesNotMatch)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"2G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -33);
}

TEST(DCONF, InvalidChannel)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"6OFF\":\"+0.082\",\"6G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -34);
}

TEST(DCONF, UnableToCastChannelOffset)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"u.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -35);
}


TEST(DCONF, UnableToCastChannelGain)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+z.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -36);
}

TEST(DCONF, InvalidJSONHeaderGlobalGain)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"galn\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -37);
}

TEST(DCONF, UnableToCastGain)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\":.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -38);
}

TEST(DCONF, InvalidJSONHeaderGlobalOffset)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"oflset\":\"+0.012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -39);
}

TEST(DCONF, UnableToCastOffset)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0;012\",\"device\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -40);
}

TEST(DCONF, InvalidJSONHeaderSN)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"bevice\":\"25MYH0D0\"}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -41);
}

TEST(DCONF, InvalidJSONEnd)
{
	// Invalid buffer test
	char buf[] = "{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\'}";
	struct CMD_DCONF_TX command;
	int len = sizeof(buf);

	int retval = parse_dconf_payload(buf, len, &command);

	ASSERT_EQ(retval, -42);
}
