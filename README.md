# python-rtaudio


Contains three almost identical experimental wrappers of [RtAudio](https://github.com/thestk/rtaudio):

- cyrtaudio  - cython

- npyrtaudio - nanobind

- pyrtaudio  - pybind11


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

