
OUTPUT_DIR=SHARED_TEST
TEST_CODE_DIR=code
CODE_DIR=../code

-include test_circBuffer1D.mk
-include test_circBuffer2D.mk

test:
	$(MAKE) test_circBuffer1D
	$(MAKE) test_circBuffer2D

clean:
	rm -rf $(OUTPUT_DIR)

#-include $(TEST_C_DEPS)
