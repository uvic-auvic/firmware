
#include <cgreen/cgreen.h>
#include "circBuffer1D.h"


Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer1D_addToBuffer)
{
//    assert_that(1U, is_equal_to(0U));
    uint8_t data[10U] = {0U};
    bool ret = circBuffer1D_popByte(CIRCBUFFER1D_CHANNEL_TEST_1, data);
    assert_that(ret, is_equal_to(0U));
}


int main(int argc, char **argv)
{

    TestSuite *suite = create_named_test_suite("test_circBuffer1D");

    add_test(suite, test_circBuffer1D_addToBuffer);

    return run_test_suite(suite, create_text_reporter());
}
