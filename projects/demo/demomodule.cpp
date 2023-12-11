#define PY_SSIZE_T_CLEAN
#include <Python.h>

class Demo {
public:
    int x;
    Demo() {
        this->x = 100;
        printf("Constructor Invoked for Demo class\n");
    }

    ~Demo() {
        printf("Destructor Invoked for Demo class\n");
    }
};

extern "C" {

typedef struct {
    PyObject_HEAD
    Demo* demo;
    // std::unique_ptr<Demo> demo;
} PyDemo;


static void Demo_dealloc(PyDemo* self)
{
    printf("Demo_dealloc: start.\n");
    if (self == NULL) {
        printf("Demo_dealloc: self==NULL.\n");
        return;
    }

    if (self->demo) {
        printf("Demo_dealloc: pre:  delete self->demo\n");
        delete self->demo;
        printf("Demo_dealloc: post: delete self->demo\n");        
        self->demo = NULL;
        printf("Demo_dealloc: self->demo = NULL\n");
    }

    printf("Demo_dealloc: pre: Py_TYPE(self)->tp_free((PyObject*)self)\n");
    Py_TYPE(self)->tp_free((PyObject*)self);
    printf("Demo_dealloc: end.\n");

}


static PyObject* Demo_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{

    printf("Demo_new: start.\n");
    PyDemo* self;
    char* api = NULL;

    printf("Demo_new: start 1\n");

    if (!PyArg_ParseTuple(args, "|s", &api)) {
        return NULL;
    }

    printf("Demo_new: start 2\n");

    self = (PyDemo*)type->tp_alloc(type, 0);

    printf("Demo_new: start 3\n");

    if (self == NULL)
        return NULL;

    printf("Demo_new: start 4\n");

    // self->demo = NULL;

    printf("Demo_new: start 5\n");
    
    if (api == NULL) {
        printf("Demo_new: api==NULL.\n");
        self->demo = new Demo();
    }

    printf("Demo_new: start 6\n");

    if (self->demo == NULL) {
        Py_XDECREF(self);
        return NULL;
    }

    printf("Demo_new: end.\n");
    

    return (PyObject*)self;
}

static int Demo_init(PyDemo* self, PyObject* args, PyObject* kwds)
{
    printf("Demo_init.\n");
    // if (self == NULL) return 0;
    return 0;
}


static PyTypeObject Demo_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "rtest.Demo",
    .tp_doc = PyDoc_STR("Audio input device"),
    .tp_basicsize = sizeof(Demo) + sizeof(Demo),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = Demo_new,
    .tp_init = (initproc)Demo_init,
    .tp_dealloc = (destructor)Demo_dealloc,
    .tp_members = 0,
    .tp_methods = 0,
};

static PyModuleDef Demo_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "Demo",
    .m_doc = "Demo wrapper.",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_demo(void)
{
    if (PyType_Ready(&Demo_type) < 0)
        return NULL;

    PyObject* module = PyModule_Create(&Demo_module);
    if (module == NULL)
        return NULL;


    if (PyModule_AddObjectRef(module, "Demo", (PyObject*)&Demo_type)< 0) {
        Py_XDECREF(module);
        return NULL;
    }

    return module;
}

} // extern C
