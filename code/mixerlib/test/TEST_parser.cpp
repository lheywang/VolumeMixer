#include "gtest/gtest.h"
#include "parser/parser.h"

/**
 * ===============================================================
 * FIRST : STRING FORMATION ERRORS
 * ===============================================================
 */

TEST(PARSER, InvalidBufferCheck)
{
	// Invalid buffer test
	char *buf = 0;
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -1);
}

TEST(PARSER, InvalidStructCheck)
{
	// Invalid struct test
	char buf[1024] = "Hello World !";
	struct CMD *command = 0;

	int retval = parser(buf, command, 0);

	ASSERT_EQ(retval, -1);
}


TEST(PARSER, TokenStartNotFound)
{
	// No START token
	char buf[1024] = "YouWon'tFindStartGna";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -10);
}

TEST(PARSER, TokenCommandNotFound)
{
	// No command found
	char buf[1024] = "START;GnaGnaGnaGnaGna";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -11);
	ASSERT_EQ(command.result, NACK);
}

TEST(PARSER, TokenLengthNotFound)
{
	// No Length token
	char buf[1024] = "START;SHUTD;000:TUTUTUTUTUTUTUTUTTU";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -12);
}

TEST(PARSER, TokenCRCNotFound)
{
	// No CRC token
	char buf[1024] = "START;SHUTD;000;;DEADBEEF:";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -14);
}

TEST(PARSER, TokenEndNotFound)
{
	// No END token
	char buf[1024] = "START;SHUTD;000;;DEADBEEF;EMD";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

/**
 * ===============================================================
 * SECOND : COMMAND PARSING NAMES
 * ===============================================================
 */

TEST(PARSER, CommandSHUTDParsing)
{
	//SHUTD command
	char buf[1024] = "START;SHUTD;000;;765eb2c0;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, SHUTD);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandRINITParsing)
{
	// RINIT command
	char buf[1024] = "START;RINIT;000;;f6e762a6;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, RINIT);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandCONNCParsing)
{
	// CONNC command
	char buf[1024] = "START;CONNC;000;;3c3da2b3;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, CONNC);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandASYNCParsing)
{
	// ASYNC command
	char buf[1024] = "START;ASYNC;000;;916255ac;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, ASYNC);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandUICONParsing)
{
	// UICON command
	char buf[1024] = "START;UICON;000;;2b525a75;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, UICON);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandSLPOSParsing)
{
	// SLPOS command
	char buf[1024] = "START;SLPOS;000;;b68ad8c8;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, SLPOS);
	ASSERT_EQ(command.direction, TX);
}

TEST(PARSER, CommandDCONFParsing)
{
	// DCONF command
	char buf[1024] = "START;DCONF;000;;4a796c5a;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.type, DCONF);
	ASSERT_EQ(command.direction, TX);
}

/**
 * ===============================================================
 * THIRD : Lenghts and PAYLOADS
 * ===============================================================
 */

TEST(PARSER, CommandLengthParsing000)
{
	// 0 char payload
	char buf[1024] = "START;DCONF;000;;4a796c5a;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 0);

	ASSERT_EQ(retval, 0);
}

TEST(PARSER, CommandLengthParsing001)
{
	// 1 char payload
	char buf[1024] = "START;DCONF;001;A;85913136;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 1);
	EXPECT_STREQ((char*)command.payload, "A");
	ASSERT_EQ(retval, 0);
}

TEST(PARSER, CommandLengthParsing001E)
{
	// 1 char payload with issue.
	char buf[1024] = "START;DCONF;;;4bbb066d;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -12);
}

TEST(PARSER, CommandInvalidLength)
{
	// 1 char payload with issue.
	char buf[1024] = "START;DCONF;AAA;e;4bbb066d;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 0);
	ASSERT_EQ(retval, -13);
}

TEST(PARSER, CommandInvalidCRC)
{
	// 1 char payload with issue.
	char buf[1024] = "START;DCONF;001;e;nnnnnnnn;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 1);
	ASSERT_EQ(retval, -15);
}

TEST(PARSER, CommandLengthParsing010)
{
	// 10 char payload
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;31d96751;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 10);
	EXPECT_STREQ((char*)command.payload, "AAAAAAAAAA");
	ASSERT_EQ(retval, 0);
}

TEST(PARSER, CommandLengthParsing100)
{
	// 100 char payload
	char buf[1024] = "START;DCONF;100;AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;da0ad5b9;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 100);
	EXPECT_STREQ((char*)command.payload, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	ASSERT_EQ(retval, 0);
}
/*
 * Removed theses tests after memory optimizations (they does not fit anymore)
 */

/*
TEST(PARSER, CommandLengthParsing500)
{
	// 500 char payload
	char buf[1024] = "START;DCONF;500;AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;ffd15417;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 500);
	EXPECT_STREQ((char*)command.payload, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	ASSERT_EQ(retval, 0);
}

TEST(PARSER, CommandLengthParsing995)
{
	// Full length payload
	char buf[1025] = "START;DCONF;995;AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;9da7896c;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.len, 995);
	EXPECT_STREQ((char*)command.payload, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	ASSERT_EQ(retval, 0);
}
*/

/**
 * ===============================================================
 * FOURTH : CRC32 fetching
 * ===============================================================
 */

TEST(PARSER, CommandCrcParsing1)
{
	// DEADBEEF crc
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;DEADBEEF;END";
	struct CMD command;

	parser(buf, &command, 0);

	ASSERT_EQ(command.crc, 0xdeadbeef);
}

TEST(PARSER, CommandCrcParsing2)
{
	// deadbeef crc
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;deadbeef;END";
	struct CMD command;

	parser(buf, &command, 0);

	ASSERT_EQ(command.crc, 0xdeadbeef);
}

TEST(PARSER, CommandCrcParsing3)
{
	// 12345678 crc
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;12345678;END";
	struct CMD command;

	parser(buf, &command, 0);

	ASSERT_EQ(command.crc, 0x12345678);
}

TEST(PARSER, CommandCrcParsing4)
{
	// 9abcdef1 crc
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;9abcdef1;END";
	struct CMD command;

	parser(buf, &command, 0);

	ASSERT_EQ(command.crc, 0x9abcdef1);
}

TEST(PARSER, CRCComp1)
{
	char buf[1024] = "START;DCONF;010;AAAAAAAAAA;31d96751;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(command.crc, 0x31d96751);
	ASSERT_EQ(retval, 0);
}

/**
 * ===============================================================
 * FOURTH : CRC32 comparison
 * ===============================================================
 */

TEST(PARSER, CRCError1)
{
	// Sub 1 to the crc
	char buf[1024] = "START;DCONF;000;;4a796c59;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

TEST(PARSER, CRCError2)
{
	// 1 char payload changed
	char buf[1024] = "START;DCONF;001;_;85913136;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

TEST(PARSER, CRCError3)
{
	// corrupted a single bit
	char buf[1024] = "START;DCONF;000;;4bba066d;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

TEST(PARSER, CRCError4)
{
	// Changed an A to B
	char buf[1024] = "START;DCONF;010;AAAAAAAAAB;31d96751;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

TEST(PARSER, CRCError5)
{
	// Added 1 to CRC
	char buf[1024] = "START;DCONF;100;AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;da0ad5ba;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

/*
 * Removed theses tests after memory optimizations (they does not fit anymore)
 */

/*
TEST(PARSER, CRCError6)
{
	// changed an A in the middle
	char buf[1024] = "START;DCONF;500;AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;ffd15417;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}

TEST(PARSER, CRCError7)
{
	// changed case of an A
	char buf[1025] = "START;DCONF;995;AAAAAAAAAAAAAAAAAAaAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA;9da7896c;END";
	struct CMD command;

	int retval = parser(buf, &command, 0);

	ASSERT_EQ(retval, -16);
}
*/

/**
 * ===============================================================
 * FIFHT : Some full command parsing !
 * ===============================================================
 */

TEST(PARSER, DCONFParser)
{
	// changed case of an A
	char buf[1025] = "START;DCONF;211;{\"cal\":[{\"1OFF\":\"+0.082\",\"1G\":\"1.01\"},{\"2OFF\":\"+0.029\",\"2G\":\"1.09\"},{\"3OFF\":\"+0.050\",\"3G\":\"1.00\"},{\"4OFF\":\"+0.000\",\"4G\":\"1.12\"},{\"5OFF\":\"-0.012\",\"5G\":\"0.98\"}],\"gain\":\"1.09\",\"offset\":\"+0.012\",\"device\":\"25MYH0D0\"};95b6bdee;END";
	struct CMD command;

	int retval = parser(buf, &command , 1); // This time, parse the output

	ASSERT_EQ(retval, 0);

	ASSERT_EQ(command.len, 211);

	ASSERT_FLOAT_EQ(command.DCONF_TX.slider1.gain, 1.01);
	ASSERT_FLOAT_EQ(command.DCONF_TX.slider1.offset, 0.082);

	ASSERT_FLOAT_EQ(command.DCONF_TX.slider2.gain, 1.09);
	ASSERT_FLOAT_EQ(command.DCONF_TX.slider2.offset, 0.029);

	ASSERT_FLOAT_EQ(command.DCONF_TX.slider3.gain, 1.00);
	ASSERT_FLOAT_EQ(command.DCONF_TX.slider3.offset, 0.050);

	ASSERT_FLOAT_EQ(command.DCONF_TX.slider4.gain, 1.12);
	ASSERT_FLOAT_EQ(command.DCONF_TX.slider4.offset, 0.000);

	ASSERT_FLOAT_EQ(command.DCONF_TX.slider5.gain, 0.98);
	ASSERT_FLOAT_EQ(command.DCONF_TX.slider5.offset, -0.012);

	ASSERT_FLOAT_EQ(command.DCONF_TX.adcGain, 1.09);
	ASSERT_FLOAT_EQ(command.DCONF_TX.adcOffset, 0.012);
}

TEST(PARSER, ASYNCParser)
{
	// changed case of an A
	char buf[1025] = "START;ASYNC;085;{\"apps\":[{\"2\":deadbeef},{\"1\":beefdead},{\"3\":aaaaaaaa},{\"4\":eeeeeeee},{\"5\":55555555}]};885533c2;END";
	struct CMD command;

	int retval = parser(buf, &command , 1); // This time, parse the output

	ASSERT_EQ(retval, 0);

	ASSERT_EQ(command.len, 85);

	ASSERT_EQ(command.ASYNC_TX.appSlider2, 0xdeadbeef);
	ASSERT_EQ(command.ASYNC_TX.appSlider1, 0xbeefdead);
	ASSERT_EQ(command.ASYNC_TX.appSlider3, 0xaaaaaaaa);
	ASSERT_EQ(command.ASYNC_TX.appSlider4, 0xeeeeeeee);
	ASSERT_EQ(command.ASYNC_TX.appSlider5, 0x55555555);
}

TEST(PARSER, UICONParser)
{
	// changed case of an A
	char buf[1025] = "START;UICON;316;{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}};064cb0b4;END";
	struct CMD command;

	int retval = parser(buf, &command , 1); // This time, parse the output

	ASSERT_EQ(retval, 0);

	ASSERT_EQ(command.len, 316);

	ASSERT_EQ(command.UICON_TX.posSlider, 1);
	ASSERT_EQ(command.UICON_TX.appSlider, 0xdeadbeef);
	ASSERT_EQ(command.UICON_TX.appStore, 1);

	uint8_t qobuz[128] =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00,
		0x00, 0x1e, 0x78, 0x00, 0x00, 0x03, 0xc0, 0x00,
		0x80, 0xc1, 0x83, 0x01, 0xc0, 0xf8, 0x1f, 0x03,
		0x60, 0x0c, 0x30, 0x06, 0x30, 0x06, 0x60, 0x0c,
		0x10, 0x03, 0xc0, 0x08, 0x98, 0x01, 0x80, 0x19,
		0x98, 0xc0, 0x03, 0x19, 0x88, 0x60, 0x06, 0x11,
		0xc8, 0x30, 0x0c, 0x13, 0xc8, 0x10, 0x08, 0x13,
		0xc8, 0x10, 0x08, 0x13, 0xc8, 0x30, 0x0c, 0x13,
		0x88, 0x60, 0x06, 0x11, 0x98, 0xc0, 0x1b, 0x19,
		0x98, 0x01, 0xb8, 0x19, 0x10, 0x03, 0x70, 0x08,
		0x30, 0x06, 0xe0, 0x0c, 0x60, 0x0c, 0xd0, 0x05,
		0xc0, 0xf8, 0x9f, 0x03, 0x80, 0xc1, 0x03, 0x07,
		0x00, 0x03, 0xc0, 0x0e, 0x00, 0x1e, 0x78, 0x0c,
		0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	for (int k = 0; k < 128; k++)
	{
		ASSERT_EQ(command.UICON_TX.icon[k], qobuz[k]);
	}
}

