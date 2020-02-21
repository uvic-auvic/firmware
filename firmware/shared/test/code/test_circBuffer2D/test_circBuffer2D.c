
#include <cgreen/cgreen.h>
#include "circBuffer2D.h"


Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer2D_addToBuffer)
{
    assert_that(0U, is_equal_to(0U));
}

int main(int argc, char **argv)
{

    TestSuite *suite = create_named_test_suite("test_circBuffer2D");

    add_test(suite, test_circBuffer2D_addToBuffer);

    return run_test_suite(suite, create_text_reporter());
}
