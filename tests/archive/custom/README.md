# custom: a python debug build failure text example

Show that a basic python debug build fails to run a canonical python c-api example

## Normal Build

Requires `cmake` and a unix environment (linux or macos)

Just type `make` then `cd` into the `build` directory and

```bash
python3
>>> import custom
>>> custom.Custom()
```

The above should give no errors


## Debug Build

To build a fit-for-purpose debug build just run the provided script `get_debug_python.sh` which will install a debug build of python in a `python` directory in this project:

```bash
./get_debug_python
```

The contents of this script are as follows:

```sh
CWD=`pwd`
BUILD=${CWD}/build
DOWNLOADS=${BUILD}/downloads
PYTHON=${CWD}/python # local destination
PYTHON_URL=https://www.python.org/ftp/python/3.11.7/Python-3.11.7.tar.xz
PYTHON_ARCHIVE=`basename ${PYTHON_URL}`
PYTHON_FOLDER=`basename -s .tar.xz ${PYTHON_ARCHIVE}`

mkdir -p ${PYTHON} && \
	mkdir -p ${DOWNLOADS} && \
	cd ${DOWNLOADS} && \
	wget ${PYTHON_URL} && \
	tar xvf ${PYTHON_ARCHIVE} && \
	cd ${PYTHON_FOLDER} && \
	mkdir debug && \
	cd debug && \
	../configure --prefix ${PYTHON} \
				 --with-pydebug \
				 --enable-shared \
				 --disable-test-modules \
				 --without-static-libpython \
				 --with-ensurepip=no && \
	make EXTRA_CFLAGS="-DPy_DEBUG" && \
	make install
```

After debug python is installed, type `make debug` then `cd` into the `build` directory and

```bash
$ python3
>>> import custom
>>> custom.Custom()
```

I get:

```bash
Fatal Python error: _PyInterpreterState_GET: the function must be called with the GIL held, but the GIL is released (the current Python thread state is NULL)
Python runtime state: unknown
```

???


