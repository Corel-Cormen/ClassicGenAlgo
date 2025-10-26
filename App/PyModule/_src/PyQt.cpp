#include <Python.h>

#include <QDebug>

#include "PyQt.hpp"

namespace PyModuleData {

PyObject* pyModuleHandler = nullptr;
PyObject* pyFunctionHandler = nullptr;
PyObject* pyFunctionCaller = nullptr;

} // end PyModuleData

bool PyQt::startPython()
{
    using namespace PyModuleData;

    if (!Py_IsInitialized())
    {
        Py_Initialize();

        PyObject* sysPath = PySys_GetObject("path");
        PyObject* pyPath = PyUnicode_FromString(PyModulePath.toString().toStdString().c_str());
        PyList_Append(sysPath, pyPath);
        Py_DECREF(pyPath);

        PyObject* pName = PyUnicode_DecodeFSDefault(PyModuleName.toString().toStdString().c_str());
        pyModuleHandler = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pyModuleHandler != nullptr)
        {
            qDebug() << "Python module found";
        }
        else
        {
            PyErr_Print();
            qDebug() << "Python module not found";
        }
    }

    return ((Py_IsInitialized() == 1) && pyModuleHandler) ? true : false;
}

void PyQt::stopPython()
{
    if (PyModuleData::pyModuleHandler != nullptr)
    {
        qDebug() << "Deinit Python module";
        Py_DECREF(PyModuleData::pyModuleHandler);
        PyModuleData::pyModuleHandler = nullptr;
    }
    Py_Finalize();
}

bool PyQt::makeFunction(const QString& funcName, const size_t dimension)
{
    using namespace PyModuleData;

    if (pyModuleHandler && (dimension > 1U))
    {
        pyFunctionHandler = PyObject_CallMethod(pyModuleHandler,
                                                funcName.toStdString().c_str(),
                                                "(i)",
                                                dimension);
        if (pyFunctionHandler)
        {
            pyFunctionCaller = PyObject_GetAttrString(pyModuleHandler,
                                                      PyCalcFunctionName.toString().toStdString().c_str());
            if(pyFunctionCaller && PyCallable_Check(pyFunctionCaller))
            {
                qDebug() << "Crate Python functions handlers";
            }
            else
            {
                PyErr_Print();
                qDebug() << "Not create callable function handler";
            }
        }
        else
        {
            PyErr_Print();
            qDebug() << "Not created Python function instance";
        }
    }
    else
    {
        if (pyModuleHandler)
        {
            qDebug() << "Python module handler not initialized";
        }
        if (dimension <= 1U)
        {
            qDebug() << "Function dim must be bigger than 1";
        }
    }

    return (pyFunctionHandler && pyFunctionCaller) ? true : false;
}

void PyQt::discardFunction()
{
    Py_XDECREF(PyModuleData::pyFunctionHandler);
    Py_XDECREF(PyModuleData::pyFunctionCaller);
    PyModuleData::pyFunctionHandler = nullptr;
    PyModuleData::pyFunctionCaller = nullptr;
}

bool PyQt::calcPoint(const std::vector<qreal>& point, qreal &val)
{
    using namespace PyModuleData;

    bool status = false;
    if(pyFunctionHandler && pyFunctionCaller)
    {
        PyObject* pyArgsList = PyList_New(point.size());
        for (size_t i = 0; i < point.size(); ++i)
        {
            PyList_SetItem(pyArgsList, i, PyFloat_FromDouble(point[i]));
        }

        constexpr size_t functionArgQuantitiy = 2U;
        PyObject* pyArgsTuple = PyTuple_Pack(functionArgQuantitiy,
                                             pyFunctionHandler,
                                             pyArgsList);

        PyObject* pResult = PyObject_CallObject(pyFunctionCaller,
                                                pyArgsTuple);

        if (pResult)
        {
            if(PyFloat_Check(pResult))
            {
                val = static_cast<qreal>(PyFloat_AsDouble(pResult));
                status = true;
            }
            Py_DECREF(pResult);
        }
        else
        {
            PyErr_Print();
        }

        Py_DECREF(pyArgsList);
        Py_DECREF(pyArgsTuple);
    }
    else
    {
        qDebug() << "Python function handler not initialized";
    }

    return status;
}
