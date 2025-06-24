#include "gtest/gtest.h"
#include "parser/parser.h"
#include "parser/commands.h"

/**
 * ===============================================================
 * FIRST : UICON parser
 * ===============================================================
 */

TEST(UICON, CorrectParsing)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.posSlider, 1);
	ASSERT_EQ(command.appSlider, 0xdeadbeef);
	ASSERT_EQ(command.appStore, 1);

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
		ASSERT_EQ(command.icon[k], qobuz[k]);
	}
}

TEST(UICON, InvalidBufferLen)
{
	// Invalid buffer test
	char buf[] = "{\"icons\":{\"slider\":\"0\",\"app\":\"deadf\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -2);
}

TEST(UICON, InvalidHeader)
{
	// Invalid buffer test
	char buf[] = "{\"icons\":{\"slider\":\"0\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -20);
}

TEST(UICON, InvalidSliderValue)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"n\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -21);
}

TEST(UICON, InvalidSlider1)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"0\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -22);
}

TEST(UICON, InvalidSlider2)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"6\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -22);
}

TEST(UICON, IncorrectAppName)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"2\",\"apps\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -23);
}

TEST(UICON, IncorrectAppHash)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"zzzzzzzz\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -24);
}

TEST(UICON, IncorrectIconName)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icons\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -25);
}

TEST(UICON, IncorrectIconValue)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icon\":\"00000000000000n00000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -26);
}

TEST(UICON, IncorrectStoreName)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"storse\":1}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -27);
}

TEST(UICON, IncorrectStoreValue)
{
	// Invalid buffer test
	char buf[] = "{\"icon\":{\"slider\":\"1\",\"app\":\"deadbeef\",\"icon\":\"00000000000000000000000000f81f00001e78000003c00080c18301c0f81f03600c30063006600c1003c0089801801998c0031988600611c8300c13c8100813c8100813c8300c138860061198c01b199801b819100370083006e00c600cd005c0f89f0380c103070003c00e001e780c00f81f00000000000000000000000000\",\"store\":z}}";
	struct CMD_UICON_TX command;
	int len = sizeof(buf);

	int retval = parse_uicon_payload(buf, len, &command);

	ASSERT_EQ(retval, -28);
}
