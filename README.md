# python-rtaudio


Contains three almost identical experimental wrappers of [RtAudio](https://github.com/thestk/rtaudio):

- cyrtaudio  - cython

- npyrtaudio - nanobind

- pbrtaudio  - pybind11


Next on the list is to add [RtMidi](https://github.com/thestk/rtmidi) support.



## To build

Uses `cmake` as the the primary build system and `make` as an optional frontend:


```bash

pip install cython pybind11 nanobind

```


then


```bash

make

```


## TODO

- [ ] more tests!!!

