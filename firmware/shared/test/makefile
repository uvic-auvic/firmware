
OUTPUT_DIR=SHARED_TEST
CODE_DIR=code

TEST_OBJS += \
$(OUTPUT_DIR)/obj/test/my_first_test.o

#TEST_C_DEPS += $(TEST_OBJS:%.o=%.d)

$(shell mkdir -p $(OUTPUT_DIR)/obj/test)

$(OUTPUT_DIR)/obj/test/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@

$(OUTPUT_DIR)/shared_test: $(TEST_OBJS)
	gcc $< -lcgreen -o $@

test: $(OUTPUT_DIR)/shared_test
	./$(OUTPUT_DIR)/shared_test

clean:
	rm -rf $(OUTPUT_DIR)

#-include $(TEST_C_DEPS)
