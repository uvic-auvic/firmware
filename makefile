
motorcontroller2: _motorcontroller2
_motorcontroller2:
	@echo "Building Motor Controller 1 - $(TARGET)"
	$(MAKE) -C firmware/motorcontroller2/makefiles $(TARGET)

powerboard1: _powerboard1
_powerboard1:
	@echo "Building Power Board 1 - $(TARGET)"

all:
	$(MAKE) motorcontroller2 TARGET=app
	$(MAKE) motorcontroller2 TARGET=bootloader
	$(MAKE) powerboard1 TARGET=app
	$(MAKE) powerboard1 TARGET=bootloader

clean:
	$(MAKE) motorcontroller2 TARGET=clean
	$(MAKE) -C firmware/motorcontroller2/test clean
	$(MAKE) -C firmware/shared/test clean
	
help:
	@echo Component: motorcontroller2
	@echo       Available TARGET options: app, bootloader
	@echo
	@echo Component: powerboard1
	@echo       Available TARGET options: app, bootloader

test:
	$(MAKE) -C firmware/shared/test test
	$(MAKE) -C firmware/motorcontroller2/test test


sandbox:
	@echo SANDBOX
	$(shell mkdir -p firmware/motorcontroller2/MOTORCONTROLLER2/generated/protobuf)
	libs/nanopb/nanopb-0.3.9.3-windows-x86/generator-bin/protoc --nanopb_out=motorcontroller2/MOTORCONTROLLER2/generated/protobuf motorcontroller2/data/protobuf/motorcontroller2.proto
