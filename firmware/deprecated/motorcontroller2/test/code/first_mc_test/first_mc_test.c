#include <cgreen/cgreen.h>

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(test1)
{
    assert_that(1U, is_equal_to(1U));
}

int main(int argc, char **argv)
{

    TestSuite *suite = create_named_test_suite("first_mc_test");

    add_test(suite, test1);

    return run_test_suite(suite, create_text_reporter());
}

