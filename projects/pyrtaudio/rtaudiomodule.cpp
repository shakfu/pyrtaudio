/************************************************************************/
/*  PyRtAudio: a python wrapper around RtAudio
    Copyright (c) 2011 Antoine Lefebvre

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    Any person wishing to distribute modifications to the Software is
    asked to send the modifications to the original developer so that
    they can be incorporated into the canonical version.  This is,
    however, not a binding provision of this license.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/************************************************************************/

// This software is in the development stage
// Do not expect compatibility with future versions.
// Comments, suggestions, new features, bug fixes, etc. are welcome

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "RtAudio.h" 

extern "C" {

    typedef struct 
    {
        PyObject_HEAD
        RtAudio *dac;
        RtAudioFormat _format;
        int _bufferSize;
        unsigned int inputChannels;
        PyObject *callback_func;
    } PyRtAudio;

    static PyObject *RtAudioErrorException;

    static int callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data )
    {
        PyRtAudio* self = (PyRtAudio*) data;

        if (status == RTAUDIO_OUTPUT_UNDERFLOW)
            printf("underflow.\n");

        if (self == NULL) return -1;

        float* in = (float *) inputBuffer;
        float* out = (float *) outputBuffer;

        PyObject *py_callback_func = self->callback_func;

        int retval = 0;

        if (py_callback_func) {
            PyGILState_STATE gstate = PyGILState_Ensure();

            PyObject* iBuffer = PyMemoryView_FromMemory((char*)in, sizeof(float) * self->inputChannels * nBufferFrames, PyBUF_READ);
            PyObject* oBuffer = PyMemoryView_FromMemory((char*)out, sizeof(float) * nBufferFrames, PyBUF_WRITE);
            PyObject *arglist =  Py_BuildValue("(O,O)", oBuffer, iBuffer);

            if (arglist == NULL) {
                printf("error.\n");
                PyErr_Print();
                PyGILState_Release(gstate);
                return 2;
            }

            // Calling the callback
            PyObject *result = PyObject_CallObject(py_callback_func, arglist);
            // PyObject *result = PyEval_CallObject(py_callback_func, arglist);

            if (PyErr_Occurred() != NULL) {
                PyErr_Print();
            }
            else if (result == NULL)
              retval = 0;
            else if (PyLong_Check(result)) {
              retval = PyLong_AsLong(result);
            }
            
            Py_DECREF(arglist);
            Py_DECREF(oBuffer);
            Py_DECREF(iBuffer);
            Py_XDECREF(result);

            PyGILState_Release(gstate);            
        }

        return retval;
    }



    static void RtAudio_dealloc(PyRtAudio *self)
    {
        printf("RtAudio_dealloc: start.\n");
        if (self == NULL) return;

        if (self->dac) {
            if (self->dac->isStreamOpen())
                self->dac->closeStream();
            Py_CLEAR(self->callback_func);
            delete self->dac;
        }

        Py_TYPE(self)->tp_free((PyObject *) self);
        printf("RtAudio_dealloc: end.\n");
    }


    static PyObject* RtAudio_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
    {
        printf("\nRtAudio Version %s\n", RtAudio::getVersion().c_str());
        // std::cout << "\nRtAudio Version " << RtAudio::getVersion() << std::endl;
        printf("RtAudio_new: start.\n");
        PyRtAudio *self;
        char *api = NULL;

        if(!PyArg_ParseTuple(args, "|s", &api)) {
            return NULL;
        }

        self = (PyRtAudio *) type->tp_alloc(type, 0);

        if(self == NULL) return NULL;

        self->dac = NULL;
        self->callback_func = NULL;

        if (api == NULL)
            self->dac = new RtAudio;
        else if(!strcmp(api, "jack"))
            self->dac = new RtAudio(RtAudio::UNIX_JACK);
        else if(!strcmp(api, "alsa"))
            self->dac = new RtAudio(RtAudio::LINUX_ALSA);
        else if(!strcmp(api, "oss"))
            self->dac = new RtAudio(RtAudio::LINUX_ALSA);
        else if(!strcmp(api, "core"))
            self->dac = new RtAudio(RtAudio::MACOSX_CORE);
        else if(!strcmp(api, "asio"))
            self->dac = new RtAudio(RtAudio::WINDOWS_ASIO);
        else if(!strcmp(api, "directsound"))
            self->dac = new RtAudio(RtAudio::WINDOWS_DS);

        if (self->dac == NULL) {
            Py_DECREF(self);
            // PyErr_SetString(RtAudioErrorException, "Cannot instantiate RtAudio");
            // Py_INCREF(RtAudioErrorException);
            return NULL;
        }

        self->dac->showWarnings(false);

        printf("RtAudio_new: end.\n");

        //Py_XINCREF(self);
        return (PyObject *) self;
    }

    static int RtAudio_init(PyRtAudio *self, PyObject *args, PyObject *kwds)
    {
        printf("RtAudio_init.\n");
        //if (self == NULL) return 0;
        return 0;
    }

    // This functions does not yet support all the features of the RtAudio::openStream method.
    // Please send your patches if you improves this.
    static PyObject* RtAudio_openStream(PyRtAudio *self, PyObject *args)
    {
        printf("RtAudio_openStream: start.\n");
        if (self == NULL) return NULL;

        if (self->dac == NULL) {
            printf("the dac is null.\n");
            Py_RETURN_NONE;
        }

        PyObject *oParamsObj;
        PyObject *iParamsObj;
        int fs;
        unsigned int bf;
        PyObject *pycallback;

        if (!PyArg_ParseTuple(args, "OOiiO", &oParamsObj, &iParamsObj, &fs, &bf, &pycallback)) 
            return NULL;

        RtAudio::StreamParameters oParams;
        oParams.deviceId = 1;
        oParams.nChannels = 1;
        oParams.firstChannel = 0;

        if (PyDict_Check(oParamsObj)) {
            if (PyDict_Contains(oParamsObj, PyUnicode_FromString("deviceId"))) {
                PyObject *value = PyDict_GetItem(oParamsObj, PyUnicode_FromString("deviceId"));
                oParams.deviceId = PyLong_AsLong(value);
            }
            if (PyDict_Contains(oParamsObj, PyUnicode_FromString("nChannels"))) {
                PyObject *value = PyDict_GetItem(oParamsObj, PyUnicode_FromString("nChannels"));
                oParams.nChannels = PyLong_AsLong(value);
            }
            if (PyDict_Contains(oParamsObj, PyUnicode_FromString("firstChannel"))) {
                PyObject *value = PyDict_GetItem(oParamsObj, PyUnicode_FromString("firstChannel"));
                oParams.firstChannel = PyLong_AsLong(value);
            }
        }
        else {
            printf("First argument must be a dictionary. Default values will be used.\n");
        }

        RtAudio::StreamParameters iParams;
        iParams.deviceId = 1;
        iParams.nChannels = 2;
        iParams.firstChannel = 0;

        if (PyDict_Check(iParamsObj)) {
            if (PyDict_Contains(iParamsObj, PyUnicode_FromString("deviceId"))) {
                PyObject *value = PyDict_GetItem(iParamsObj, PyUnicode_FromString("deviceId"));
                iParams.deviceId = PyLong_AsLong(value);
            }
            if (PyDict_Contains(iParamsObj, PyUnicode_FromString("nChannels"))) {
                PyObject *value = PyDict_GetItem(iParamsObj, PyUnicode_FromString("nChannels"));
                iParams.nChannels = PyLong_AsLong(value);
            }
            if (PyDict_Contains(iParamsObj, PyUnicode_FromString("firstChannel"))) {
                PyObject *value = PyDict_GetItem(iParamsObj, PyUnicode_FromString("firstChannel"));
                iParams.firstChannel = PyLong_AsLong(value);
            }
        }
        else {
            printf("Second argument must be a dictionary. Default values will be used.\n");
        }


        if (!PyCallable_Check(pycallback)) {
            PyErr_SetString(PyExc_TypeError, "Need a callable object!");
            Py_XINCREF(PyExc_TypeError);
            return NULL;
        }

        // sanity check the callback ?


        Py_INCREF(pycallback);         /* Add a reference to new callback */
        self->callback_func = pycallback; /*Remember new callback */

        // add support for other format
        self->_format = RTAUDIO_FLOAT32;

        // add support for other options
        RtAudio::StreamOptions options;
        options.flags = RTAUDIO_NONINTERLEAVED;


        if (self->dac->isStreamOpen())
            self->dac->closeStream();

        RtAudioErrorType err = self->dac->openStream(&oParams, &iParams, self->_format, fs, &bf, &callback, self, &options);
        if (err != RTAUDIO_NO_ERROR) {
            PyErr_SetString(RtAudioErrorException, self->dac->getErrorText().c_str());
            Py_INCREF(RtAudioErrorException);
            return NULL;
        }

        self->inputChannels = iParams.nChannels;

        printf("RtAudio_openStream: end.\n");

        Py_RETURN_NONE;
    }

    static PyObject* RtAudio_closeStream(PyRtAudio *self, PyObject *args)
    {
        printf("RtAudio_closeStream: start.\n");

        if (self == NULL || self->dac == NULL) return NULL;

        if (self->dac->isStreamOpen())
            self->dac->closeStream();

        Py_CLEAR(self->callback_func);

        printf("RtAudio_closeStream: end.\n");

        Py_RETURN_NONE;
    }

    static PyObject* RtAudio_startStream(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;

        RtAudioErrorType err = self->dac->startStream();
        
        if (err != RTAUDIO_NO_ERROR) {
            PyErr_SetString(RtAudioErrorException, self->dac->getErrorText().c_str());
            Py_INCREF(RtAudioErrorException);
            return NULL;
        }

        Py_RETURN_NONE;
    }


    static PyObject* RtAudio_stopStream(PyRtAudio *self, PyObject *args)
    {
        printf("RtAudio_stopStream.\n");
        if (self == NULL || self->dac == NULL) return NULL;

        RtAudioErrorType err = self->dac->stopStream();

        if (err != RTAUDIO_NO_ERROR) {
            PyErr_SetString(RtAudioErrorException, self->dac->getErrorText().c_str());
            Py_INCREF(RtAudioErrorException);
            return NULL;
        }

        Py_RETURN_NONE;
    }

    static PyObject* RtAudio_abortStream(PyRtAudio *self, PyObject *args)
    {
        printf("RtAudio_abortStream.\n");
        if (self == NULL || self->dac == NULL) return NULL;

        
        RtAudioErrorType err = self->dac->abortStream();
        
        if (err != RTAUDIO_NO_ERROR) {
            PyErr_SetString(RtAudioErrorException, self->dac->getErrorText().c_str());
            Py_INCREF(RtAudioErrorException);
            return NULL;
        }
        Py_RETURN_NONE;
    }

    static PyObject* RtAudio_isStreamRunning(PyRtAudio *self, PyObject *args)
    {
       if (self == NULL || self->dac == NULL) return NULL;

       if (self->dac == NULL) {
            Py_RETURN_FALSE;
        }
        if (self->dac->isStreamRunning())
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    static PyObject* RtAudio_isStreamOpen(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;

        if (self->dac == NULL) {
            Py_RETURN_FALSE;
        }
        if (self->dac->isStreamOpen())
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;

    }

    static PyObject* RtAudio_getDeviceCount(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;

        printf("RtAudio_getDeviceCount\n");

        return PyLong_FromLong(self->dac->getDeviceCount());
    }

    static PyObject* RtAudio_getDeviceInfo(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;

        int device;
        if (!PyArg_ParseTuple(args, "i", &device))
            return NULL;

        RtAudio::DeviceInfo info = self->dac->getDeviceInfo(device);

        PyObject* info_dict = PyDict_New();

        // if (info.probed) {
        //     Py_INCREF(Py_True);
        //     PyDict_SetItemString(info_dict, "probed", Py_True);
        // }
        // else {
        //     Py_INCREF(Py_False);
        //     PyDict_SetItemString(info_dict, "probed", Py_False);
        // }
        PyObject* obj;

        obj = PyUnicode_FromString(info.name.c_str());
        PyDict_SetItemString(info_dict, "name", obj);

        obj = PyLong_FromLong(info.outputChannels);
        PyDict_SetItemString(info_dict, "outputChannels", obj);

        obj = PyLong_FromLong(info.inputChannels);
        PyDict_SetItemString(info_dict, "inputChannels", obj);

        obj = PyLong_FromLong(info.duplexChannels);
        PyDict_SetItemString(info_dict, "duplexChannels", obj);

        if (info.isDefaultOutput) {
            Py_INCREF(Py_True);
            PyDict_SetItemString(info_dict, "isDefaultOutput", Py_True);
        }
        else {
            Py_INCREF(Py_False);
            PyDict_SetItemString(info_dict, "isDefaultOutput", Py_False);
        }

        if (info.isDefaultInput) {
            Py_INCREF(Py_True);
            PyDict_SetItemString(info_dict, "isDefaultInput", Py_True);
        }
        else {
            Py_INCREF(Py_False);
            PyDict_SetItemString(info_dict, "isDefaultInput", Py_False);
        }

        return info_dict;
    }

    static PyObject* RtAudio_getDefaultOutputDevice(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;
        printf("RtAudio_getDefaultOutputDevice\n");
        return PyLong_FromLong(self->dac->getDefaultOutputDevice());
    }

    static PyObject* RtAudio_getDefaultInputDevice(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;
        return PyLong_FromLong(self->dac->getDefaultInputDevice());
    }

    static PyObject* RtAudio_getStreamTime(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;
        return PyFloat_FromDouble( self->dac->getStreamTime() );
    }

    static PyObject* RtAudio_getStreamLatency(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;
        return PyLong_FromLong( self->dac->getStreamLatency() );
    }

    static PyObject* RtAudio_getStreamSampleRate(PyRtAudio *self, PyObject *args)
    {
        if (self == NULL || self->dac == NULL) return NULL;
        return PyLong_FromLong( self->dac->getStreamSampleRate() );
    }

    static PyObject* RtAudio_showWarnings(PyRtAudio *self, PyObject *args)
    {

        printf("RtAudio_showWarnings: start.\n");

        if (self == NULL || self->dac == NULL) return NULL;

        PyObject *obj;
        if (!PyArg_ParseTuple(args, "O", &obj))
            return NULL;

        if (!PyBool_Check(obj))
            return NULL;

        if (Py_IsTrue(obj))
            self->dac->showWarnings(true);
        else if (Py_IsFalse(obj))
            self->dac->showWarnings(false);
        else {
            printf("not true nor false\n");
        }
        printf("RtAudio_showWarnings: end.\n");
        Py_RETURN_NONE;
    }


    static PyMethodDef RtAudio_methods[] = 
    {
        // TO BE DONE: getCurrentApi(void)
        {"getDeviceCount", (PyCFunction) RtAudio_getDeviceCount, METH_NOARGS,
        "A public function that queries for the number of audio devices available."},
        {"getDeviceInfo", (PyCFunction) RtAudio_getDeviceInfo, METH_VARARGS,
        "Return a dictionary with information for a specified device number."},
        {"getDefaultOutputDevice", (PyCFunction) RtAudio_getDefaultOutputDevice, METH_NOARGS,
        "A function that returns the index of the default output device."},
        {"getDefaultInputDevice", (PyCFunction) RtAudio_getDefaultInputDevice, METH_NOARGS,
        "A function that returns the index of the default input device."},
        {"openStream", (PyCFunction) RtAudio_openStream, METH_VARARGS,
        "A public method for opening a stream with the specified parameters."},
        {"closeStream", (PyCFunction) RtAudio_closeStream, METH_NOARGS,
        "A function that closes a stream and frees any associated stream memory. "},
        {"startStream", (PyCFunction) RtAudio_startStream, METH_NOARGS,
        "A function that starts a stream. "},
        {"stopStream", (PyCFunction) RtAudio_stopStream, METH_NOARGS,
        "Stop a stream, allowing any samples remaining in the output queue to be played. "},
        {"abortStream", (PyCFunction) RtAudio_abortStream, METH_NOARGS,
        "Stop a stream, discarding any samples remaining in the input/output queue."},
        {"isStreamOpen", (PyCFunction) RtAudio_isStreamOpen, METH_NOARGS,
        "Returns true if a stream is open and false if not."},
        {"isStreamRunning", (PyCFunction) RtAudio_isStreamRunning, METH_NOARGS,
        "Returns true if the stream is running and false if it is stopped or not open."},
        {"getStreamTime", (PyCFunction) RtAudio_getStreamTime, METH_NOARGS,
        "Returns the number of elapsed seconds since the stream was started."},
        {"getStreamLatency", (PyCFunction) RtAudio_getStreamLatency, METH_NOARGS,
        "Returns the internal stream latency in sample frames."},
        {"getStreamSampleRate", (PyCFunction) RtAudio_getStreamSampleRate, METH_NOARGS,
        "Returns actual sample rate in use by the stream."},
        {"showWarnings", (PyCFunction) RtAudio_showWarnings, METH_VARARGS,
        "Specify whether warning messages should be printed to stderr."},
        // TO BE DONE: getCompiledApi (std::vector< RtAudio::Api > &apis) throw ()
        {NULL}
    };

    static PyTypeObject RtAudio_type = {
        .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "rtaudio.RtAudio",
        .tp_doc = PyDoc_STR("Audio input device"),
        .tp_basicsize = sizeof(RtAudio),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        .tp_new = RtAudio_new,
        .tp_init = (initproc) RtAudio_init,
        .tp_dealloc = (destructor) RtAudio_dealloc,
        .tp_members = 0,
        .tp_methods = RtAudio_methods,
    };

    static PyModuleDef RtAudio_module = {
        .m_base = PyModuleDef_HEAD_INIT,
        .m_name = "RtAudio",
        .m_doc = "RtAudio wrapper.",
        .m_size = -1,
    };



    PyMODINIT_FUNC
    PyInit_rtaudio(void)
    {
        // DOES NOTHING
        // if (!PyEval_ThreadsInitialized())
        //     PyEval_InitThreads();

        if (PyType_Ready(&RtAudio_type) < 0)
            return NULL;

        PyObject* module = PyModule_Create(&RtAudio_module);
        if (module == NULL)
            return NULL;


        if (PyModule_AddObjectRef(module, "RtAudio", (PyObject *) &RtAudio_type) < 0) {
            Py_DECREF(module);
            return NULL;
        }

        RtAudioErrorException = PyErr_NewException("rtaudio.RtError", NULL, NULL);
        Py_INCREF(RtAudioErrorException);
        PyModule_AddObject(module, "RtError", RtAudioErrorException);
        return module;
    }

} // extern C
