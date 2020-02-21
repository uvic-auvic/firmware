
# FIRST_MC_TEST
$(shell mkdir -p $(OUTPUT_DIR)/first_mc_test)
$(shell mkdir -p $(OUTPUT_DIR)/first_mc_test/code)

FIRST_MC_TEST_INCLUDE_PATHS := \
-I$(TEST_CODE_DIR)/first_mc_test \
-I$(SHARED_CODE_DIR) \
-I$(SHARED_CODE_DIR)/utils

FIRST_MC_TEST_OBJS += \
$(OUTPUT_DIR)/first_mc_test/first_mc_test.o \

# Create the executable and run it
first_mc_test: $(FIRST_MC_TEST_OBJS)
	gcc $^ -lcgreen -o $(OUTPUT_DIR)/first_mc_test/test
	./$(OUTPUT_DIR)/first_mc_test/test

# Create object file for test
$(OUTPUT_DIR)/first_mc_test/%.o: $(TEST_CODE_DIR)/first_mc_test/%.c
	gcc -c $< -o $@ $(FIRST_MC_TEST_INCLUDE_PATHS)

# Create the object file for the file being tested
$(OUTPUT_DIR)/first_mc_test/code/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@ $(FIRST_MC_TEST_INCLUDE_PATHS)
