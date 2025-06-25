#include "gtest/gtest.h"
#include "parser/parser.h"
#include "parser/commands.h"
#include <stdio.h>

/**
 * ===============================================================
 * FIRST : SLPOS Builder
 * ===============================================================
 */

TEST(SLPOS, CorrectBuilding)
{
	// Invalid buffer test
	char buf[] = "{\"sliders\":[{\"1\":082,\"1M\":1},{\"2\":029,\"2M\":0},{\"3\":050,\"3M\":1},{\"4\":100,\"4M\":1},{\"5\":000,\"5M\":0}]}";
	char buf2[256] = {0};
	struct CMD_SLPOS_RX command;
	int len = 256;

	command.slider1.Mute = 1;
	command.slider1.Pos = 82;

	command.slider2.Mute = 0;
	command.slider2.Pos = 29;

	command.slider3.Mute = 1;
	command.slider3.Pos = 50;

	command.slider4.Mute = 1;
	command.slider4.Pos = 100;

	command.slider5.Mute = 0;
	command.slider5.Pos = 0;


	int retval = build_slpos_payload(&command, buf2, &len);

	ASSERT_EQ(retval, 0);
	ASSERT_EQ(len, 98);
	EXPECT_STREQ(buf2, buf);
}
