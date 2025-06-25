#include "gtest/gtest.h"
#include "parser/parser.h"
#include "parser/commands.h"
#include <stdio.h>

/**
 * ===============================================================
 * FIRST : ASYNC parser
 * ===============================================================
 */

TEST(ASYNCP, CorrectParsing)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\":deadbeef},{\"2\":beefdead},{\"3\":aaaaaaaa},{\"4\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(command.appSlider1, 0xdeadbeef);
	ASSERT_EQ(command.appSlider2, 0xbeefdead);
	ASSERT_EQ(command.appSlider3, 0xaaaaaaaa);
	ASSERT_EQ(command.appSlider4, 0x55555555);
	ASSERT_EQ(command.appSlider5, 0xfefefefe);
}

TEST(ASYNCP, IncorrectAppsName)
{
	// Invalid buffer test
	char buf[] = "{\"apqs\":[{\"1\":deadbeef},{\"2\":beefdead},{\"3\":aaaaaaaa},{\"4\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -50);
}

TEST(ASYNCP, IncorrectAppsStructure)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\";deadbeef},{\"2\":beefdead},{\"3\":aaaaaaaa},{\"4\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -51);
}

TEST(ASYNCP, IncorrectChannelNumber)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\":deadbeef},{\"2\":beefdead},{\"3\":aaaaaaaa},{\"u\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -52);
}

TEST(ASYNCP, IncorrectChannelValue)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\":deadbeef},{\"2\":beefdead},{\"6\":aaaaaaaa},{\"4\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -53);
}

TEST(ASYNCP, IncorrectAppHashValue)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\":deadbeef},{\"2\":beefdead},{\"3\":aaaaazaa},{\"4\":55555555},{\"5\":fefefefe}]}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -54);
}

TEST(ASYNCP, IncorrectEndAppHeader)
{
	// Invalid buffer test
	char buf[] = "{\"apps\":[{\"1\":deadbeef},{\"2\":beefdead},{\"3\":aaaaaaaa},{\"4\":55555555},{\"5\":fefefefe})}";
	struct CMD_ASYNC_TX command;
	int len = sizeof(buf);

	int retval = parse_async_payload(buf, len, &command);

	ASSERT_EQ(retval, -55);
}


/**
 * ===============================================================
 * SECOND : ASYNC builder
 * ===============================================================
 */

TEST(ASYNCB, CorrectBuilding)
{
	// Invalid buffer test
	char buf[] = "{\"match\":0,\"apps\":[{\"1\":deadbeef,\"match\":1},{\"2\":deadbeef,\"match\":1},{\"3\":deadbeef,\"match\":2},{\"4\":deadbeef,\"match\":1},{\"5\":deadbeef,\"match\":0}]}";
	char buf2[256] = {0};
	struct CMD_ASYNC_RX command;
	int len = 256;

	command.sync = 0;
	command.slider1.appMatch = 1;
	command.slider1.appSlider = 0xdeadbeef;
	command.slider2.appMatch = 1;
	command.slider2.appSlider = 0xdeadbeef;
	command.slider3.appMatch = 2;
	command.slider3.appSlider = 0xdeadbeef;
	command.slider4.appMatch = 1;
	command.slider4.appSlider = 0xdeadbeef;
	command.slider5.appMatch = 0;
	command.slider5.appSlider = 0xdeadbeef;

	int retval = build_async_payload(&command, buf2, &len);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(len, 145);
	EXPECT_STREQ(buf2, buf);
}

TEST(ASYNCB, ExtremeCorrectBuilding)
{
	// Invalid buffer test
	char buf[] = "{\"match\":0,\"apps\":[{\"1\":ffffffff,\"match\":1},{\"2\":00000000,\"match\":4},{\"3\":00000017,\"match\":3},{\"4\":45789624,\"match\":5},{\"5\":55555555,\"match\":9}]}";
	char buf2[256] = {0};
	struct CMD_ASYNC_RX command;
	int len = 256;

	command.sync = 0;
	command.slider1.appMatch = 1;
	command.slider1.appSlider = 0xFFFFFFFF;
	command.slider2.appMatch = 4;
	command.slider2.appSlider = 0x00;
	command.slider3.appMatch = 3;
	command.slider3.appSlider = 0x17;
	command.slider4.appMatch = 5;
	command.slider4.appSlider = 0x45789624;
	command.slider5.appMatch = 9;
	command.slider5.appSlider = 0x55555555;

	int retval = build_async_payload(&command, buf2, &len);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(len, 145);
	EXPECT_STREQ(buf2, buf);
}
