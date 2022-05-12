// Inspiration from:
// https://github.com/DEAP/deap/blob/master/examples/gp/ant/
// https://stackoverflow.com/questions/48786693/how-to-wrap-a-c-object-using-pure-python-extension-api-python3

#include "RunnerSimulator.hpp"
#include <stdio.h>

RunnerSimulator::RunnerSimulator(unsigned int inMaxMoves)
{
    this->mMaxMoves = inMaxMoves;
    position = 0;
}

void RunnerSimulator::turnLeft(void) { position--; }

void RunnerSimulator::turnRight(void) { position++; }

void RunnerSimulator::run(PyObject *inWrappedFunc)
{
    this->reset();
    while (nMoves < mMaxMoves)
    {
        this->nMoves++;
        PyObject_CallFunctionObjArgs(inWrappedFunc, NULL);
    }
}

void RunnerSimulator::reset(void)
{
    this->nMoves = 0;
}

/*
 *
 * Python wrappers
 *
 *
 */

typedef struct
{
    PyObject_HEAD
        RunnerSimulator *mInnerClass;
} RunnerSimulatorWrapper;

static int wrapRunnerSimulatorConstructor(RunnerSimulatorWrapper *self, PyObject *args, PyObject *kwargs)
{
    int lMaxMoves;
    const char *keywords[] = {"max_moves", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *)"i", (char **)keywords, &lMaxMoves))
    {
        return -1;
    }
    self->mInnerClass = new RunnerSimulator(lMaxMoves);
    return 0;
}

static PyObject *wrapRun(RunnerSimulatorWrapper *self, PyObject *args)
{
    PyObject *func = PyTuple_GetItem(args, 0);
    self->mInnerClass->run(func);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *wrapString(RunnerSimulatorWrapper *self, PyObject *args)
{
    PyObject *obj = PyTuple_GetItem(args, 0);
    PyObject *repr = PyObject_Str(obj); // Alternatively use PyObject_Repr, but it adds single quotes
    PyObject *str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
    const char *bytes = PyBytes_AS_STRING(str);

    std::cout << bytes << std::endl;

    Py_INCREF(Py_None);
    return Py_None;
}

static void wrapTurnLeft(RunnerSimulatorWrapper *self)
{
    self->mInnerClass->turnLeft();
}

static void wrapTurnRight(RunnerSimulatorWrapper *self)
{
    self->mInnerClass->turnRight();
}

static void wrapReset(RunnerSimulatorWrapper *self)
{
    self->mInnerClass->reset();
}

static PyObject *wrapGetNMaxMoves(RunnerSimulatorWrapper *self)
{
    PyObject *py_retval;

    py_retval = Py_BuildValue((char *)"i", self->mInnerClass->getMMaxMoves());

    return py_retval;
}

// Class method declarations
static PyMethodDef RunnerSimulatorWrapper_methods[] = {
    {(char *)"turn_left", (PyCFunction)wrapTurnLeft, METH_NOARGS, NULL},
    {(char *)"turn_right", (PyCFunction)wrapTurnRight, METH_NOARGS, NULL},
    {(char *)"run", (PyCFunction)wrapRun, METH_VARARGS, NULL},
    {(char *)"string", (PyCFunction)wrapString, METH_VARARGS, NULL},
    {(char *)"reset", (PyCFunction)wrapReset, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}};

// Getters and setters (here only for the 'eaten' attribute)
static PyGetSetDef RunnerSimulatorWrapper_getsets[] = {
    {
        (char *)"max_moves",      // attribute name
        (getter)wrapGetNMaxMoves, // C function to get the attribute
        NULL,                     // C function to set the attribute
        NULL,                     // optional doc string
        NULL                      // optional additional data for getter and setter
    },
    {NULL, NULL, NULL, NULL, NULL}};

static PyModuleDef RunnerSimulatorWrapper_module = {
    PyModuleDef_HEAD_INIT,
    "RunnerSimulatorWrapper",              // Module name to use with Python import statements
    "Provides some functions, but faster", // Module description
    0,
    RunnerSimulatorWrapper_methods // Structure that defines the methods of the module
};

static void RunnerSimulatorWrapperDealloc(RunnerSimulatorWrapper *self)
{
    delete self->mInnerClass;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *RunnerSimulatorWrapperRichcompare(RunnerSimulatorWrapper *self, RunnerSimulatorWrapper *other, int opid)
{
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

static PyTypeObject PyRunnerSimulatorWrapper_Type = {
    PyVarObject_HEAD_INIT(NULL, 0) "RunnerC.Runner" /* tp_name */
};

PyMODINIT_FUNC PyInit_RunnerC()
{
    PyRunnerSimulatorWrapper_Type.tp_new = PyType_GenericNew;
    PyRunnerSimulatorWrapper_Type.tp_basicsize = sizeof(RunnerSimulatorWrapper);
    PyRunnerSimulatorWrapper_Type.tp_dealloc = (destructor)RunnerSimulatorWrapperDealloc;
    PyRunnerSimulatorWrapper_Type.tp_flags = Py_TPFLAGS_DEFAULT;
    PyRunnerSimulatorWrapper_Type.tp_doc = "Runner simulator";
    PyRunnerSimulatorWrapper_Type.tp_methods = RunnerSimulatorWrapper_methods;
    PyRunnerSimulatorWrapper_Type.tp_getset = RunnerSimulatorWrapper_getsets;
    // PyRunnerSimulatorWrapper_Type.tp_richcompare=RunnerSimulatorWrapperRichcompare;
    //~ PyVoiceType.tp_members=Noddy_members;
    PyRunnerSimulatorWrapper_Type.tp_init = (initproc)wrapRunnerSimulatorConstructor;

    PyObject *m = PyModule_Create(&RunnerSimulatorWrapper_module);
    if (m == NULL)
    {
        return NULL;
    }
    if (PyType_Ready(&PyRunnerSimulatorWrapper_Type) < 0)
    {
        return NULL;
    }
    PyModule_AddObject(m, (char *)"Runner", (PyObject *)&PyRunnerSimulatorWrapper_Type);
    return m;
}