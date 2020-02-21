
# TEST_CIRCBUFFER2D
$(shell mkdir -p $(OUTPUT_DIR)/test_circBuffer2D)
$(shell mkdir -p $(OUTPUT_DIR)/test_circBuffer2D/code)

TEST_CIRCBUFFER2D_INCLUDE_PATHS := \
-I$(TEST_CODE_DIR)/test_circBuffer2D \
-I$(CODE_DIR) \
-I$(CODE_DIR)/utils 

TEST_CIRCBUFFER2D_OBJS += \
$(OUTPUT_DIR)/test_circBuffer2D/test_circBuffer2D.o \
$(OUTPUT_DIR)/test_circBuffer2D/code/circBuffer2D.o \
$(OUTPUT_DIR)/test_circBuffer2D/circBuffer2D_componentSpecific.o

# Create the executable and run it
test_circBuffer2D: $(TEST_CIRCBUFFER2D_OBJS)
	gcc $^ -lcgreen -o $(OUTPUT_DIR)/test_circBuffer2D/test
	./$(OUTPUT_DIR)/test_circBuffer2D/test

# Create object file for test
$(OUTPUT_DIR)/test_circBuffer2D/%.o: $(TEST_CODE_DIR)/test_circBuffer2D/%.c
	gcc -c $< -o $@ $(TEST_CIRCBUFFER2D_INCLUDE_PATHS)

# Create the object file for the file being tested
$(OUTPUT_DIR)/test_circBuffer2D/code/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@ $(TEST_CIRCBUFFER2D_INCLUDE_PATHS)
