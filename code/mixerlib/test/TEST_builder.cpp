#include "gtest/gtest.h"
#include "parser/parser.h"

/**
 * ===============================================================
 * FIRST : BUFFER AND GLOBAL ISSUES
 * ===============================================================
 */

TEST(BUILDER, InvalidBufferCheck)
{
	// Invalid buffer test
	char *buf = 0;
	struct CMD command;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, -1);
}

TEST(BUILDER, InvalidCommandCheck)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD *command = 0;

	int retval = builder(command, buf);

	ASSERT_EQ(retval, -1);
}

/**
 * ===============================================================
 * SECOND : STATUS ERRORS
 * ===============================================================
 */

TEST(BUILDER, InvalidResultCheck)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.result = INPUT;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, -2);
}

TEST(BUILDER, InvalidDirectionCheck)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = TX;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, -3);
}

/**
 * ===============================================================
 * THIRD : NACK COMMANDS
 * ===============================================================
 */

TEST(BUILDER, NACK_SLPOS)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = SLPOS;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;SLPOS;004;NACK;c4c62382;END");
}

TEST(BUILDER, NACK_UICON)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = UICON;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;UICON;004;NACK;0e30dac5;END");
}

TEST(BUILDER, NACK_ASYNC)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = ASYNC;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;ASYNC;004;NACK;aac2078a;END");
}

TEST(BUILDER, NACK_CONNC)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = CONNC;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;CONNC;004;NACK;8777bbd8;END");
}

TEST(BUILDER, NACK_DCONF)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = DCONF;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;DCONF;004;NACK;9a425028;END");
}

TEST(BUILDER, NACK_RINIT)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = RINIT;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;RINIT;004;NACK;71798c0e;END");
}

TEST(BUILDER, NACK_SHUTD)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = SHUTD;
	command.result = NACK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;SHUTD;004;NACK;3a028730;END");
}

/**
 * ===============================================================
 * FOURTH : ACK COMMANDS
 * ===============================================================
 */

TEST(BUILDER, ACK_SHUTD)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = SHUTD;
	command.result = OK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;SHUTD;003;ACK;668281c8;END");
}

TEST(BUILDER, ACK_RINIT)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = RINIT;
	command.result = OK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;RINIT;003;ACK;195df8ed;END");
}


TEST(BUILDER, ACK_DCONF)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = DCONF;
	command.result = OK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;DCONF;003;ACK;2cb7e128;END");
}


TEST(BUILDER, ACK_UICON)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = UICON;
	command.result = OK;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;UICON;003;ACK;3a0688a1;END");
}


TEST(BUILDER, ACK_ASYNC)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = ASYNC;
	command.result = OK;

	command.ASYNC_RX.sync = 1;

	command.ASYNC_RX.slider1.appSlider = 0xdeadbeef;
	command.ASYNC_RX.slider2.appSlider = 0xfeefbeef;
	command.ASYNC_RX.slider3.appSlider = 0x12345678;
	command.ASYNC_RX.slider4.appSlider = 0x9abcdef0;
	command.ASYNC_RX.slider5.appSlider = 0xaaaaaaaa;

	command.ASYNC_RX.slider1.appMatch = 1;
	command.ASYNC_RX.slider2.appMatch = 0;
	command.ASYNC_RX.slider3.appMatch = 1;
	command.ASYNC_RX.slider4.appMatch = 0;
	command.ASYNC_RX.slider5.appMatch = 1;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;ASYNC;145;{\"match\":1,\"apps\":[{\"1\":deadbeef,\"match\":1},{\"2\":feefbeef,\"match\":0},{\"3\":12345678,\"match\":1},{\"4\":9abcdef0,\"match\":0},{\"5\":aaaaaaaa,\"match\":1}]};21839d75;END");
}

TEST(BUILDER, ACK_SLPOS)
{
	// Invalid buffer test
	char buf[1024] = {0};
	struct CMD command;

	command.direction = RX;
	command.type = SLPOS;
	command.result = OK;

	command.SLPOS_RX.slider1.Mute = 0;
	command.SLPOS_RX.slider2.Mute = 1;
	command.SLPOS_RX.slider3.Mute = 0;
	command.SLPOS_RX.slider4.Mute = 1;
	command.SLPOS_RX.slider5.Mute = 0;

	command.SLPOS_RX.slider1.Pos = 1;
	command.SLPOS_RX.slider2.Pos = 10;
	command.SLPOS_RX.slider3.Pos = 33;
	command.SLPOS_RX.slider4.Pos = 66;
	command.SLPOS_RX.slider5.Pos = 100;

	int retval = builder(&command, buf);

	ASSERT_EQ(retval, 0);
	EXPECT_STREQ(buf, "START;SLPOS;098;{\"sliders\":[{\"1\":001,\"1M\":0},{\"2\":010,\"2M\":1},{\"3\":033,\"3M\":0},{\"4\":066,\"4M\":1},{\"5\":100,\"5M\":0}]};53e2d09e;END");
}


