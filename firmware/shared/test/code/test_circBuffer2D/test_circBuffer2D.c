
#include <cgreen/cgreen.h>

#include "circBuffer2D_componentSpecific.h"
#include "circBuffer2D.h"
#include <stdio.h>


Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer2D)
{
	circBuffer2D_init();

	uint8_t result = 0U;
	uint8_t chk = 0U;

	uint8_t data[] = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U};
	uint8_t returnData[8];

	//performs test 2 times on each channel
	for(circBuffer2D_channel_E channel = CIRCBUFFER2D_CHANNEL_TEST_1; channel < CIRCBUFFER2D_CHANNEL_COUNT*2; channel++){

		circBuffer2D_channel_E testChannel = channel % 2;

		// Pushes to all of the slots of the buffer channel, checks each push is successful
		for(int i = 0; i < 8; i++){
			chk = circBuffer2D_push(testChannel, data, 8U);
			assert_that(chk, is_equal_to(1U));
		}


		// Tries to push when the buffer channel is full, checks to ensure failure
		chk = circBuffer2D_push(testChannel, data, 8U);
		assert_that(chk, is_equal_to(0U));

		// Tests getSpaceAvailable to make sure it recognizes a full buffer channel
		result = circBuffer2D_getSpaceAvailable(testChannel);
		assert_that(result, is_equal_to(0U));


		
		//Pops all of the slots of the buffer channel, ensures data being popped matches the data pushed
		for(int i = 0; i < 8; i++){
			chk = circBuffer2D_pop(testChannel, returnData);
					assert_that(chk, is_equal_to(8U));
					assert_that(returnData[i], is_equal_to(i+1));
		}

		// Tries to pop an empty buffer channel, ensures failure
		chk = circBuffer2D_pop(testChannel, returnData);
		assert_that(chk, is_equal_to(0U));

		// Tests getSpaceAvailable to make sure it recognizes an empty buffer channel
		result = circBuffer2D_getSpaceAvailable(testChannel);
		assert_that(result, is_equal_to(8U));
	}
}

int main(int argc, char **argv)
{
    TestSuite *suite = create_named_test_suite("test_circBuffer2D");

    add_test(suite, test_circBuffer2D);

    return run_test_suite(suite, create_text_reporter());
}
