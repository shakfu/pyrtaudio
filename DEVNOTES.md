# Development Notes



## Handling std::function

cython can't handle `std::function` directly, here are some links to workarounds:

- https://stackoverflow.com/questions/45943940/cython-stdfunction-callbacks-with-custom-parameter-types 

- https://stackoverflow.com/questions/73083770/how-to-wrap-a-stdfunction-using-cython-for-python-usage

- https://stackoverflow.com/questions/39044063/pass-a-closure-from-cython-to-c/39052204#39052204

- https://github.com/hildensia/py_c_py

- https://stackoverflow.com/questions/21242160/how-to-build-a-cython-wrapper-for-c-function-with-stl-list-parameter




## Handling Thread Safety

- https://docs.python.org/3/c-api/init.html#thread-state-and-the-global-interpreter-lock

- https://docs.python.org/3/extending/newtypes_tutorial.html

- https://github.com/python/cpython/blob/main/Modules/zlibmodule.c

- https://pythonextensionpatterns.readthedocs.io/en/latest/thread_safety.html

- https://github.com/paulross/skiplist

- https://opensource.com/article/17/4/grok-gil

- 