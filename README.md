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


## Notes

- https://stackoverflow.com/questions/45943940/cython-stdfunction-callbacks-with-custom-parameter-types 

- https://stackoverflow.com/questions/73083770/how-to-wrap-a-stdfunction-using-cython-for-python-usage

- https://stackoverflow.com/questions/39044063/pass-a-closure-from-cython-to-c/39052204#39052204

- https://github.com/hildensia/py_c_py

- https://stackoverflow.com/questions/21242160/how-to-build-a-cython-wrapper-for-c-function-with-stl-list-parameter



