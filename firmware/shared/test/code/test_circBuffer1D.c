#include <cgreen/cgreen.h>

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test_circBuffer1D_addToBuffer)
{
    assert_that(1U, is_equal_to(1U));
}

int main(int argc, char **argv)
{

    TestSuite *suite = create_named_test_suite("test_circBuffer1D");

    add_test(suite, test_circBuffer1D_addToBuffer);

    return run_test_suite(suite, create_text_reporter());
}
