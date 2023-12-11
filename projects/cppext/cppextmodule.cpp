#include <Python.h>
#include <structmember.h>

#include <exception>
#include <iostream>

class MyClass {
public:
    MyClass()
        : m_cnt(0)
    {
        std::cout << "MyClass::MyClass() called!" << std::endl;
    };

    virtual ~MyClass()
    {
        std::cout << "MyClass::~MyClass() called" << std::endl;
    };

    unsigned long addOne() { return ++m_cnt; };

private:
    unsigned long m_cnt;
};


typedef struct {
    PyObject_HEAD int m_value;
    MyClass* m_myclass;
} MyClassObject;


int MyClass_init(PyObject* self, PyObject* args, PyObject* kwds);
PyObject* MyClass_new(PyTypeObject* subtype, PyObject* args, PyObject* kwds);
void MyClass_dealloc(MyClassObject* self);
PyObject* MyClass_addOne(PyObject* self, PyObject* args);


PyObject* MyClass_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    std::cout << "MtClass_new() called!" << std::endl;

    MyClassObject* self;
    self = (MyClassObject*)type->tp_alloc(type, 0);
    if (self != NULL) { // -> allocation successfull
        // assign initial values
        self->m_value = 0;
        self->m_myclass = NULL;
    }
    return (PyObject*)self;
}

int MyClass_init(PyObject* self, PyObject* args, PyObject* kwds)
{

    std::cout << "MyClass_init() called!" << std::endl;
    MyClassObject* m = (MyClassObject*)self;

    m->m_value = 123;
    m->m_myclass = (MyClass*)PyObject_Malloc(sizeof(MyClass));

    if (!m->m_myclass) {
        PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed");
        return -1;
    }

    try {
        new (m->m_myclass) MyClass();
    } catch (const std::exception& ex) {
        PyObject_Free(m->m_myclass);
        m->m_myclass = NULL;
        m->m_value = 0;
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        return -1;
    } catch (...) {
        PyObject_Free(m->m_myclass);
        m->m_myclass = NULL;
        m->m_value = 0;
        PyErr_SetString(PyExc_RuntimeError, "Initialization failed");
        return -1;
    }

    return 0;
}

void MyClass_dealloc(MyClassObject* self)
{
    std::cout << "MyClass_dealloc() called!" << std::endl;
    PyTypeObject* tp = Py_TYPE(self);

    MyClassObject* m = reinterpret_cast<MyClassObject*>(self);

    if (m->m_myclass) {
        m->m_myclass->~MyClass();
        PyObject_Free(m->m_myclass);
    }

    tp->tp_free(self);
    Py_DECREF(tp);
};

PyObject* MyClass_addOne(PyObject* self, PyObject* args)
{
    assert(self);

    MyClassObject* _self = reinterpret_cast<MyClassObject*>(self);
    unsigned long val = _self->m_myclass->addOne();
    return PyLong_FromUnsignedLong(val);
}


static PyMethodDef MyClass_methods[] = {
    { "addOne", (PyCFunction)MyClass_addOne, METH_NOARGS,
      PyDoc_STR("Return an incrmented integer") },
    { NULL, NULL } /* Sentinel */
};

static struct PyMemberDef MyClass_members[] = {
    { "value", T_INT, offsetof(MyClassObject, m_value) },
    { NULL } /* Sentinel */
};

static PyType_Slot MyClass_slots[] = {
    { Py_tp_new, (void*)MyClass_new },
    { Py_tp_init, (void*)MyClass_init },
    { Py_tp_dealloc, (void*)MyClass_dealloc },
    { Py_tp_members, MyClass_members },
    { Py_tp_methods, MyClass_methods },
    { 0, 0 }
};


// static PyType_Spec spec_myclass = {
//     .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "rtest.Demo",
//     .tp_doc = PyDoc_STR("Audio input device"),
//     .tp_basicsize = sizeof(Demo) + sizeof(Demo),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_new = Demo_new,
//     .tp_init = (initproc)Demo_init,
//     .tp_dealloc = (destructor)Demo_dealloc,
//     .tp_members = 0,
//     .tp_methods = 0,
// };


static PyType_Spec spec_myclass = {
    "MyClass",                                // name
    sizeof(MyClassObject) + sizeof(MyClass),  // basicsize
    0,                                        // itemsize
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // flags
    MyClass_slots                             // slots
};


static PyModuleDef cppext_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "cppext",
    .m_doc = "cppext wrapper.",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_cppext(void)
{
    PyObject* module = PyModule_Create(&cppext_module);

    PyObject* myclass = PyType_FromSpec(&spec_myclass);
    if (myclass == NULL) {
        return NULL;
    }
    Py_INCREF(myclass);

    if (PyModule_AddObject(module, "MyClass", myclass) < 0) {
        Py_DECREF(myclass);
        Py_DECREF(module);
        return NULL;
    }
    return module;
}