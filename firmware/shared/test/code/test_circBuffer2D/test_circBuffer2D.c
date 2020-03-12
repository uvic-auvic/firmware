
#include <cgreen/cgreen.h>

#include "circBuffer2D_componentSpecific.h"
#include "circBuffer2D.h"
#include <stdio.h>


Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer2D_addToBuffer)
{
	uint8_t res = 0U;
	uint8_t chk = 0U;

	uint8_t data[] = {1U, 2U, 3U, 4U, 5U};
	for(int i = 0; i < 10; i++){
		chk = circBuffer2D_push(CIRCBUFFER2D_CHANNEL_TEST_1, data, 5U);
		assert_that(chk, is_equal_to(1U));
	}

	res = circBuffer2D_getSpaceAvailable(CIRCBUFFER2D_CHANNEL_TEST_1);


    assert_that(res, is_equal_to(0U));
    assert_that(0U, is_equal_to(0U));
}

int main(int argc, char **argv)
{

    circBuffer2D_init();

    TestSuite *suite = create_named_test_suite("test_circBuffer2D");

    add_test(suite, test_circBuffer2D_addToBuffer);

    return run_test_suite(suite, create_text_reporter());
}
