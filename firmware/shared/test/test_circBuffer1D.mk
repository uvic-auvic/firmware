
# TEST_CIRCBUFFER1D
$(shell mkdir -p $(OUTPUT_DIR)/test_circBuffer1D)
$(shell mkdir -p $(OUTPUT_DIR)/test_circBuffer1D/code)

TEST_CIRCBUFFER1D_INCLUDE_PATHS := \
-I$(TEST_CODE_DIR)/test_circBuffer1D \
-I$(CODE_DIR) \
-I$(CODE_DIR)/utils 

TEST_CIRCBUFFER1D_OBJS += \
$(OUTPUT_DIR)/test_circBuffer1D/test_circBuffer1D.o \
$(OUTPUT_DIR)/test_circBuffer1D/code/circBuffer1D.o \
$(OUTPUT_DIR)/test_circBuffer1D/circBuffer1D_componentSpecific.o

# Create the executable and run it
test_circBuffer1D: $(TEST_CIRCBUFFER1D_OBJS)
	gcc $^ -lcgreen -o $(OUTPUT_DIR)/test_circBuffer1D/test
	./$(OUTPUT_DIR)/test_circBuffer1D/test

# Create object file for test
$(OUTPUT_DIR)/test_circBuffer1D/%.o: $(TEST_CODE_DIR)/test_circBuffer1D/%.c
	gcc -c $< -o $@ $(TEST_CIRCBUFFER1D_INCLUDE_PATHS)

# Create the object file for the file being tested
$(OUTPUT_DIR)/test_circBuffer1D/code/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@ $(TEST_CIRCBUFFER1D_INCLUDE_PATHS)
