PRESET ?= dev
T ?=

CTEST = ctest --preset $(PRESET) --progress $(if $(T),-R '$(T)')

.PHONY: check help dev build test clean

check: build
	$(CTEST)

help:
	@echo "make check  - build, then test (default)"
	@echo "make dev    - configure"
	@echo "make build  - compile"
	@echo "make test   - run tests without rebuilding (filter: make test T=UniquePtr)"
	@echo "make clean  - remove build directory"

dev:
	cmake --preset $(PRESET)

build:
	cmake --build --preset $(PRESET)

test:
	$(CTEST)

clean:
	rm -rf build
