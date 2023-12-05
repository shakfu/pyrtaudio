MIN_OSX_VER := -mmacosx-version-min=13.6

RTAUDIO_STATICLIB := ./lib/librtaudio.a

.PHONY: cmake clean test

all: cmake

cmake:
	@rm -f projects/cyrtaudio/cyrtaudio.cpp
	@mkdir -p build && cd build && cmake .. && make

test:
	@echo "testing"

clean:
	@rm -rf build

