# Development Notes

## pyrtaudio crashes

- using a debug python

```bash
% PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 ../python/bin/python3.11d

Python 3.11.7 (main, Dec 10 2023, 17:58:40) [Clang 15.0.0 (clang-1500.0.40.1)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import rtest;a=rtest.RtAudio();exit()
RtAudio_new: start.
RtAudio_new: start 1
RtAudio_new: start 2
RtAudio_new: start 3
RtAudio_new: start 4
RtAudio_new: start 5
RtAudio_new: api==NULL.
Constructor Invoked for RtAudio class
RtAudio_new: start 6
RtAudio_init.
RtAudio_dealloc: start.
RtAudio_dealloc: pre:  delete self->dac
Destructor Invoked for RtAudio class
RtAudio_dealloc: post: delete self->dac
RtAudio_dealloc: self->dac = NULL
RtAudio_dealloc: pre: Py_TYPE(self)->tp_free((PyObject*)self)
Debug memory block at address p=0x1002b09a0: API 'o'
    8 bytes originally requested
    The 7 pad bytes at p-7 are FORBIDDENBYTE, as expected.
    The 8 pad bytes at tail=0x1002b09a8 are not all FORBIDDENBYTE (0xfd):
        at tail+0: 0x08 *** OUCH
        at tail+1: 0x41 *** OUCH
        at tail+2: 0x67 *** OUCH
        at tail+3: 0x00 *** OUCH
        at tail+4: 0x01 *** OUCH
        at tail+5: 0x00 *** OUCH
        at tail+6: 0x00 *** OUCH
        at tail+7: 0x00 *** OUCH
    Data at p: 00 00 00 00 00 00 00 00

Memory block allocated at (most recent call first):
  File "<stdin>", line 1

Fatal Python error: _PyMem_DebugRawFree: bad trailing pad byte
Python runtime state: finalizing (tstate=0x0000000100d2b4b0)

Current thread 0x00000001ebba2080 (most recent call first):
  Garbage-collecting
  <no Python frame>
zsh: abort      PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 ../python/bin/python3.11d
```


- Using the following env vars to reproduce

```bash
$ PYTHONMALLOC=debug PYTHONTRACEMALLOC=1 python3 test_rtaudio_min.py

RtAudio Version 6.0.1
RtAudio_new: start.
RtAudio_new: api==NULL.
RtAudio_new: end.
RtAudio_init.
RtAudio_dealloc: start.
RtAudio_dealloc: self->dac
RtAudio_dealloc: pre: delete self->dac
RtAudio_dealloc: post: delete self->dac
RtAudio_dealloc: pre: Py_TYPE(self)->tp_free((PyObject*)self)
Debug memory block at address p=0x102d10440: API 'o'
    8 bytes originally requested
    The 7 pad bytes at p-7 are FORBIDDENBYTE, as expected.
    The 8 pad bytes at tail=0x102d10448 are not all FORBIDDENBYTE (0xfd):
        at tail+0: 0xa0 *** OUCH
        at tail+1: 0x60 *** OUCH
        at tail+2: 0xf9 *** OUCH
        at tail+3: 0x02 *** OUCH
        at tail+4: 0x01 *** OUCH
        at tail+5: 0x00 *** OUCH
        at tail+6: 0x00 *** OUCH
        at tail+7: 0x00 *** OUCH
    Data at p: 00 00 00 00 00 00 00 00

Memory block allocated at (most recent call first):
  File "/Users/sa/projects/python-rtaudio/build/test_rtaudio_min.py", line 3

Fatal Python error: _PyMem_DebugRawFree: bad trailing pad byte
Python runtime state: finalizing (tstate=0x00000001034bfd28)

Current thread 0x00007ff8562ab700 (most recent call first):
  <no Python frame>
Abort trap: 6
```

- trying again with with debug python

```bash
$ cd /Python-3.11.7
$ mkdir debug && cd debug
$ ../configure --with-pydebug --prefix=$HOME/projects/python-rtaudio/python
$ make install EXTRA_CFLAGS="-DPy_DEBUG"
```



## Handling Thread Safety

- https://pythoncapi.readthedocs.io/type_object.html

- https://pythondev.readthedocs.io/debug_tools.html

- https://docs.python.org/3/c-api/init.html#thread-state-and-the-global-interpreter-lock

- https://docs.python.org/3/extending/newtypes_tutorial.html

- https://github.com/python/cpython/blob/main/Modules/zlibmodule.c

- https://pythonextensionpatterns.readthedocs.io/en/latest/thread_safety.html

- https://github.com/paulross/skiplist

- https://opensource.com/article/17/4/grok-gil


## Handling std::function

cython can't handle `std::function` directly, here are some links to workarounds:

- https://stackoverflow.com/questions/45943940/cython-stdfunction-callbacks-with-custom-parameter-types 

- https://stackoverflow.com/questions/73083770/how-to-wrap-a-stdfunction-using-cython-for-python-usage

- https://stackoverflow.com/questions/39044063/pass-a-closure-from-cython-to-c/39052204#39052204

- https://github.com/hildensia/py_c_py

- https://stackoverflow.com/questions/21242160/how-to-build-a-cython-wrapper-for-c-function-with-stl-list-parameter

