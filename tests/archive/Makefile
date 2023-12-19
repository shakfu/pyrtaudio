.PHONY: cmake clean test debug memtest

all: cmake

cmake:
	@mkdir -p build && cd build && cmake .. && make

test:
	@echo "testing"
	@mkdir -p build && cd build && cmake .. -DBUILD_TESTS=1 && make

clean:
	@rm -rf build

memtest:
	@mkdir -p build
	@cp tests/test_rtaudio_m.py build/
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 python3 test_rtaudio_m.py

debug: clean
	@mkdir -p build && cd build && cmake .. -DDEBUG=ON && make

memray:
	@echo "import demo;a=demo.Demo()" > build/test_demo.py
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 \
		../python/bin/memray run --native test_demo.py

test-demo:
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 \
		../python/bin/python3.11d -c "import demo;a=demo.Demo()"

test-rtaudio:
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 \
		../python/bin/python3.11d -c "import rtaudio;a=rtaudio.RtAudio()"

test-custom:
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 \
		../python/bin/python3.11d -c "import custom;a=custom.Custom()"

test-cpptest:
	@cd build && PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 \
		../python/bin/python3.11d -c "import custom;a=custom.Custom()"
