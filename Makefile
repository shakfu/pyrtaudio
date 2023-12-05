MIN_OSX_VER := -mmacosx-version-min=13.6

RTAUDIO_STATICLIB := ./lib/librtaudio.a

.PHONY: cmake clean test

all: cmake

cmake:
	@mkdir -p build && cd build && cmake .. && make

test:
	@echo "testing"
	@mkdir -p build && cd build && cmake .. -DBUILD_TESTS=1 && make

clean:
	@rm -rf build

