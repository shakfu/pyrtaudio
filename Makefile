# set path so `faust` be queried for the path to stdlib
export PATH := $(PWD)/bin:$(PATH)

MIN_OSX_VER := -mmacosx-version-min=13.6

RTAUDIO_STATICLIB := ./lib/librtaudio.a

.PHONY: clean test

all: pyrtaudio

pyrtaudio:
	@mkdir -p build && cd build && cmake .. && make

test:
	@echo "testing"

clean:
	@rm -rf build

