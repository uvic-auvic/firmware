
PROTO_FILE_DIR=$(REPO_ROOT)/shared/data/protobuf
GENERATED_PROTO_DIR=$(OUTPUT_DIR)/generated/protobuf
NANOPB_LIB_DIR=$(REPO_ROOT)/libs/nanopb
#NANOPB_COMPILER=$(NANOPB_LIB_DIR)/generator-bin/protoc
NANOPB_COMPILER=protoc
NANOPB_GENERATOR=$(REPO_ROOT)/libs/nanopb/generator/protoc-gen-nanopb

PROTO_OBJS += \
$(OUTPUT_DIR)/obj/protobuf/POLARIS.pb.o \
$(OUTPUT_DIR)/obj/protobuf/pb_common.o \
$(OUTPUT_DIR)/obj/protobuf/pb_decode.o \
$(OUTPUT_DIR)/obj/protobuf/pb_encode.o

PROTO_C_DEPS += $(PROTO_OBJS:%.o=%.d)

INCLUDE_PATH += \
-I$(GENERATED_PROTO_DIR) \
-I$(NANOPB_LIB_DIR)

$(shell mkdir -p $(GENERATED_PROTO_DIR))
$(shell mkdir -p $(OUTPUT_DIR)/obj/protobuf)

ARM_TOOLS_COMPILER_FLAGS := \
-O0 \
-g3 \
-Wall

#Generate source and header files
$(OUTPUT_DIR)/generated/protobuf/%.pb.c $(OUTPUT_DIR)/generated/protobuf/%.pb.h: $(PROTO_FILE_DIR)/%.proto
	@echo 'Generating probuf files'
	$(NANOPB_COMPILER) --plugin=protoc-gen-nanopb=$(NANOPB_GENERATOR) -I=$(PROTO_FILE_DIR) --nanopb_out=$(GENERATED_PROTO_DIR) $<

#Build object file from generated c file
$(OUTPUT_DIR)/obj/protobuf/%.pb.o: $(OUTPUT_DIR)/generated/protobuf/%.pb.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
	
$(OUTPUT_DIR)/obj/protobuf/%.o: $(NANOPB_LIB_DIR)/%.c
	@$(MAKE) --no-print-directory arm-complier ARM_TOOLS_COMPILER_SOURCE_FILE=$< ARM_TOOLS_COMPILER_OBJECT_FILE=$@ ARM_TOOLS_COMPILER_DEFS_FILE=$(@:%.o=%.d)
