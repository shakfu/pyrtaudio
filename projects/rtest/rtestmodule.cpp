#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <memory>


// #include "RtAudio.h"

class RtAudio {
    public:
        RtAudio() {
            // Constructor is defined.
            printf("Constructor Invoked for RtAudio class\n");
        }
    
    ~RtAudio() {
        // Destructor is defined.
        printf("Destructor Invoked for RtAudio class\n");
    }
};

extern "C" {

typedef struct {
    PyObject_HEAD
    RtAudio* dac;
    // std::unique_ptr<RtAudio> dac;
} PyRtAudio;


static void RtAudio_dealloc(PyRtAudio* self)
{
    printf("RtAudio_dealloc: start.\n");
    if (self == NULL) {
        printf("RtAudio_dealloc: self==NULL.\n");
        return;
    }

    if (self->dac) {
        printf("RtAudio_dealloc: pre:  delete self->dac\n");
        delete self->dac;
        printf("RtAudio_dealloc: post: delete self->dac\n");        
        // self->dac = NULL;
        printf("RtAudio_dealloc: self->dac = NULL\n");
    }

    printf("RtAudio_dealloc: pre: Py_TYPE(self)->tp_free((PyObject*)self)\n");
    Py_TYPE(self)->tp_free((PyObject*)self);
    printf("RtAudio_dealloc: end.\n");

}


static PyObject* RtAudio_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{

    printf("RtAudio_new: start.\n");
    PyRtAudio* self;
    char* api = NULL;

    printf("RtAudio_new: start 1\n");

    if (!PyArg_ParseTuple(args, "|s", &api)) {
        return NULL;
    }

    printf("RtAudio_new: start 2\n");

    self = (PyRtAudio*)type->tp_alloc(type, 0);


    printf("RtAudio_new: start 3\n");

    if (self == NULL)
        return NULL;

    printf("RtAudio_new: start 4\n");

    // self->dac = NULL;

    printf("RtAudio_new: start 5\n");
    
    if (api == NULL) {
        printf("RtAudio_new: api==NULL.\n");
        self->dac = new RtAudio();
        // self->dac = std::make_unique<RtAudio>();
    }

    printf("RtAudio_new: start 6\n");


    // if (self->dac == NULL) {
    //     Py_XDECREF(self);
    //     return NULL;
    // }

    // printf("RtAudio_new: end.\n");
    

    return (PyObject*)self;
}

static int RtAudio_init(PyRtAudio* self, PyObject* args, PyObject* kwds)
{
    printf("RtAudio_init.\n");
    // if (self == NULL) return 0;
    return 0;
}


static PyTypeObject RtAudio_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "rtest.RtAudio",
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


PyMODINIT_FUNC PyInit_rtest(void)
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
