
export REPO_ROOT=$(CURDIR)

powerboard2: _powerboard2
_powerboard2:
	@echo "Building Power Board 2 - $(TARGET)"
	$(MAKE) --no-print-directory -C powerboard2/makefiles $(TARGET)
	
motorcontroller3: _motorcontroller3
_motorcontroller3:
	@echo "Building Motor Controller 3 - $(TARGET)"
	$(MAKE) --no-print-directory -C motorcontroller3/makefiles $(TARGET)
	
torpedoboard: _torpedoboard
_torpedoboard:
	@echo "Building Torpedo Board - $(TARGET)"
	$(MAKE) --no-print-directory -C torpedoboard/makefiles $(TARGET)

generated:

	# Generated Protocol File in Python
	clang2py shared/data/protocol/protocol.h -o tools/lib/generated/protocol.py --clang-args="-I/usr/lib/gcc/arm-none-eabi/4.9.3/include"

all:

	# Power Board 2
	$(MAKE) powerboard2 TARGET=app
	
	# Motor Controller 3
	$(MAKE) motorcontroller3 TARGET=app
	
	# Torpedo Board
	$(MAKE) torpedoboard TARGET=app

clean:

	# Power Board 2
	$(MAKE) powerboard2 TARGET=clean
	# $(MAKE) --no-print-directory -C powerboard2/test clean
	
	# Motor Controller 3
	$(MAKE) motorcontroller3 TARGET=clean

	#Torpedo Board
	$(MAKE) torpedoboard TARGET=clean
	
	# Shared Test
	$(MAKE) --no-print-directory -C shared/test clean

help:

	@echo Component: powerboard2
	@echo       Available TARGET options: app
	@echo
	@echo Component: motorcontroller3
	@echo		Available TARGET options: app
	@echo
	@echo Component: torpedoboard
	@echo		Available TARGET options: app

test:
	$(MAKE) -C shared/test test

sandbox:
	@echo SANDBOX
	$(shell mkdir -p motorcontroller2/MOTORCONTROLLER2/generated/protobuf)
	libs/nanopb/nanopb-0.3.9.3-windows-x86/generator-bin/protoc --nanopb_out=motorcontroller2/MOTORCONTROLLER2/generated/protobuf motorcontroller2/data/protobuf/motorcontroller2.proto
