
OUTPUT_DIR=MC_TEST
CODE_DIR=code

TEST_TARGETS += \
$(OUTPUT_DIR)/test/first_mc_test

#TEST_C_DEPS += $(TEST_OBJS:%.o=%.d)

$(shell mkdir -p $(OUTPUT_DIR)/test)
$(shell mkdir -p $(OUTPUT_DIR)/obj)

$(OUTPUT_DIR)/obj/%.o: $(CODE_DIR)/%.c
	gcc -c $< -o $@

$(OUTPUT_DIR)/test/%: $(OUTPUT_DIR)/obj/%.o
	gcc $< -lcgreen -o $@
	./$@

test: $(TEST_TARGETS)

clean:
	rm -rf $(OUTPUT_DIR)

#-include $(TEST_C_DEPS)

