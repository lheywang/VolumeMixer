#include "gtest/gtest.h"
#include "parser/parser.h"
#include "parser/commands.h"

/**
 * ===============================================================
 * FIRST : ASYNC parser
 * ===============================================================
 */

TEST(AsyncP, CorrectParsing)
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
