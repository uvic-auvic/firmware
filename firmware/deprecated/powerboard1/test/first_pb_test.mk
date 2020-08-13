
# FIRST_PB_TEST
$(shell mkdir -p $(OUTPUT_DIR)/first_pb_test)
$(shell mkdir -p $(OUTPUT_DIR)/first_pb_test/code)

FIRST_PB_TEST_INCLUDE_PATHS := \
-I$(TEST_CODE_DIR)/first_pb_test \
-I$(SHARED_CODE_DIR) \
-I$(SHARED_CODE_DIR)/utils

FIRST_PB_TEST_OBJS += \
$(OUTPUT_DIR)/first_pb_test/first_pb_test.o \

# Create the executable and run it
first_pb_test: $(FIRST_PB_TEST_OBJS)
	gcc $^ -lcgreen -o $(OUTPUT_DIR)/first_pb_test/test
	./$(OUTPUT_DIR)/first_pb_test/test

# Create object file for test
$(OUTPUT_DIR)/first_pb_test/%.o: $(TEST_CODE_DIR)/first_pb_test/%.c
	gcc -c $< -o $@ $(FIRST_PB_TEST_INCLUDE_PATHS)

# Create the object file for the file being tested
$(OUTPUT_DIR)/first_pb_test/code/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@ $(FIRST_PB_TEST_INCLUDE_PATHS)
