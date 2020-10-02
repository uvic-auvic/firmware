
#include <cgreen/cgreen.h>
#include "circBuffer1D.h"
#include "circBuffer1D_componentSpecific.h"

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer1D)
{
    circBuffer1D_init();

    uint32_t result = 0U;
	uint8_t chk = 0U;

	uint8_t data[] = {1U, 3U, 5U, 7U, 9U, 11U, 13U, 15U};
	uint8_t returnData[8];

	//performs test 2 times with a 1D buffer with size of 8U
	for(circBuffer1D_channel_E channel = CIRCBUFFER1D_CHANNEL_TEST_1; channel < CIRCBUFFER1D_CHANNEL_COUNT * 2 + 1  ; channel += 2)
    {
		circBuffer1D_channel_E testChannel = channel % 2;

		

        //Push to first 3 slots of the buffer channel, check each push is successful
        for (int i = 0; i < 3 ; i++)
        {
            chk = circBuffer1D_pushByte(testChannel, data[i]);
            assert_that(chk, is_equal_to(1U));
        }

        //Tests getSpaceAvailable to make sure it recognizes an partly filled buffer channel
        result = circBuffer1D_getSpaceAvailable(testChannel);
        assert_that(result, is_equal_to(5U));

        //Pop 2 oldest slots of the buffer channel, check each pop is successful and data being popped matches the data pushed
        for (int i = 0; i < 2 ; i++)
        {
            chk = circBuffer1D_popByte(testChannel, returnData);
            assert_that(chk, is_equal_to(1U));
            assert_that(*returnData, is_equal_to(2*i+1));
        }

        //Push to next slot of the buffer channel, check each push is successful
        for (int i = 3; i < 4 ; i++)
        {
            chk = circBuffer1D_pushByte(testChannel, data[i]);
            assert_that(chk, is_equal_to(1U));
        }

        //Tests getSpaceAvailable to make sure it recognizes an partly filled buffer channel
        result = circBuffer1D_getSpaceAvailable(testChannel);
        assert_that(result, is_equal_to(6U));

        //Pop 4 oldest slots of the buffer channel, check each pop is successful and data being popped matched the data pushed
        for (int i = 2; i < 4 ; i++)
        {
            chk = circBuffer1D_popByte(testChannel, returnData);
            assert_that(chk, is_equal_to(1U));
            assert_that(*returnData, is_equal_to(2*i+1));
        }

       // Tries to pop an empty buffer channel, ensures failure
		chk = circBuffer1D_pop(testChannel, returnData);
		assert_that(chk, is_equal_to(0U));

		// Tests getSpaceAvailable to make sure it recognizes an empty buffer channel
		result = circBuffer1D_getSpaceAvailable(testChannel);
		assert_that(result, is_equal_to(8U));
      
		// Pushes to all of the slots of the buffer channel, checks push is successful
        	chk = circBuffer1D_push(testChannel, data, 8U);
			assert_that(chk, is_equal_to(1U));
	      
		// Tries to push when the buffer channel is full, checks to ensure failure
		chk = circBuffer1D_push(testChannel, data, 8U);
		assert_that(chk, is_equal_to(0U));
       
		// Tests getSpaceAvailable to make sure it recognizes a full buffer channel
		result = circBuffer1D_getSpaceAvailable(testChannel);
		assert_that(result, is_equal_to(0U));

        

		//Pops all of the slots of the buffer channel, ensures data being popped matches the data pushed
            chk = circBuffer1D_pop(testChannel, returnData);
		    assert_that(chk, is_equal_to(8U));
         for(int i = 0; i < 8 ; i++)
        {  
			assert_that(returnData[i], is_equal_to(2*i+1));
		}

        // Tests getSpaceAvailable to make sure it recognizes an empty buffer channel
		result = circBuffer1D_getSpaceAvailable(testChannel);
		assert_that(result, is_equal_to(8U));
	
    }
}

int main(int argc, char **argv)
{

    TestSuite *suite = create_named_test_suite("test_circBuffer1D");

    add_test(suite, test_circBuffer1D);

    return run_test_suite(suite, create_text_reporter());
}
