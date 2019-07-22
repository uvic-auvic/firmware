all: motorcontroller2 powerboard1

motorcontroller2: _motorcontroller2
_motorcontroller2:
	@echo "Building Motor Controller 1 - $(ARG1)"
	$(MAKE) -C motorcontroller2/makefiles $(ARG1)

powerboard1: _powerboard1
_powerboard1:
	@echo "Building Power Board 1 - $(ARG1)"