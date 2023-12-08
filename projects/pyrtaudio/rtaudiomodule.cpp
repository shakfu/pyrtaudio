#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "RtAudio.h"

extern "C" {

typedef struct {
    PyObject_HEAD
    RtAudio* dac;
} PyRtAudio;


static void RtAudio_dealloc(PyRtAudio* self)
{
    printf("RtAudio_dealloc: start.\n");
    if (self == NULL) {
        printf("RtAudio_dealloc: self==NULL.\n");
        return;
    }

    if (self->dac) {
        printf("RtAudio_dealloc: self->dac\n");
        if (self->dac->isStreamOpen()) {
            printf("RtAudio_dealloc: isStreamOpen -> closeStream\n");
            self->dac->closeStream();
        }
        printf("RtAudio_dealloc: pre:  delete self->dac\n");
        delete self->dac;
        self->dac = NULL;
        printf("RtAudio_dealloc: post: delete self->dac\n");
    }
    printf("RtAudio_dealloc: pre: Py_TYPE(self)->tp_free((PyObject*)self)\n");
    Py_TYPE(self)->tp_free((PyObject*)self);
    printf("RtAudio_dealloc: end.\n");
}


static PyObject* RtAudio_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    printf("\nRtAudio Version %s\n", RtAudio::getVersion().c_str());
    printf("RtAudio_new: start.\n");
    PyRtAudio* self;
    char* api = NULL;

    if (!PyArg_ParseTuple(args, "|s", &api)) {
        return NULL;
    }

    self = (PyRtAudio*)type->tp_alloc(type, 0);

    if (self == NULL)
        return NULL;

    self->dac = NULL;

    if (api == NULL) {
        printf("RtAudio_new: api==NULL.\n");
        self->dac = new RtAudio;
    }
    else if (!strcmp(api, "core")) {
        printf("RtAudio_new: api==core.\n");
        self->dac = new RtAudio(RtAudio::MACOSX_CORE);
    }

    if (self->dac == NULL) {
        Py_XDECREF(self);
        return NULL;
    }

    self->dac->showWarnings(true);

    printf("RtAudio_new: end.\n");

    return (PyObject*)self;
}

static int RtAudio_init(PyRtAudio* self, PyObject* args, PyObject* kwds)
{
    printf("RtAudio_init.\n");
    // if (self == NULL) return 0;
    return 0;
}


static PyTypeObject RtAudio_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "rtaudio.RtAudio",
    .tp_doc = PyDoc_STR("Audio input device"),
    .tp_basicsize = sizeof(RtAudio),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = RtAudio_new,
    .tp_init = (initproc)RtAudio_init,
    .tp_dealloc = (destructor)RtAudio_dealloc,
    .tp_members = 0,
    .tp_methods = 0,
};

static PyModuleDef RtAudio_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "RtAudio",
    .m_doc = "RtAudio wrapper.",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_rtaudio(void)
{
    if (PyType_Ready(&RtAudio_type) < 0)
        return NULL;

    PyObject* module = PyModule_Create(&RtAudio_module);
    if (module == NULL)
        return NULL;


    if (PyModule_AddObjectRef(module, "RtAudio", (PyObject*)&RtAudio_type)< 0) {
        Py_XDECREF(module);
        return NULL;
    }

    return module;
}

} // extern C
