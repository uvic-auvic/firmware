
motorcontroller2: _motorcontroller2
_motorcontroller2:
	@echo "Building Motor Controller 1 - $(TARGET) - $(DEVICE)"
	$(MAKE) -C motorcontroller2/makefiles $(TARGET)

powerboard1: _powerboard1
_powerboard1:
	@echo "Building Power Board 1 - $(TARGET)"

all:
	$(MAKE) motorcontroller2 TARGET=app DEVICE=STM32F411
	$(MAKE) motorcontroller2 TARGET=bootloader DEVICE=STM32F411
	$(MAKE) motorcontroller2 TARGET=app DEVICE=STM32F413
	$(MAKE) motorcontroller2 TARGET=bootloader DEVICE=STM32F413
	$(MAKE) powerboard1 TARGET=app
	$(MAKE) powerboard1 TARGET=bootloader

clean:
	$(MAKE) motorcontroller2 TARGET=clean
	$(MAKE) -C motorcontroller2/test clean
	$(MAKE) -C shared/test clean
	
help:
	@echo Component: motorcontroller2
	@echo       Available TARGET options: app, bootloader
	@echo       Available DEVICE options: STM32F411 \(default\), STM32F413
	@echo
	@echo Component: powerboard1
	@echo       Available TARGET options: app, bootloader

test:
	$(MAKE) -C shared/test test
	$(MAKE) -C motorcontroller2/test test


sandbox:
	@echo SANDBOX
	$(shell mkdir -p motorcontroller2/MOTORCONTROLLER2/generated/protobuf)
	libs/nanopb/nanopb-0.3.9.3-windows-x86/generator-bin/protoc --nanopb_out=motorcontroller2/MOTORCONTROLLER2/generated/protobuf motorcontroller2/data/protobuf/motorcontroller2.proto
