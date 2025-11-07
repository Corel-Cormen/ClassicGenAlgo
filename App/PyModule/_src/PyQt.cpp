#include <Python.h>

#include <QDebug>

#include "PyQt.hpp"

namespace PyModuleData {

PyObject* pyFunctionFabricModuleHandler = nullptr;
PyObject* pyFunctionCallerModuleHandler = nullptr;
PyObject* pyFunctionVisualizationModuleHandler = nullptr;
PyObject* pyFunctionVisualizationChartsModuleHandler = nullptr;

PyObject* pyFunctionHandler = nullptr;
PyObject* pyFunctionCaller = nullptr;
PyObject* pyFunctionPlotCaller = nullptr;
PyObject* pySummaryPlotCaller = nullptr;
PyObject* pyFitnessHistoryPlotCaller = nullptr;

bool pyImportModule(PyObject** hnd, const QStringView &name)
{
    PyObject* pName = PyUnicode_DecodeFSDefault(
        name.toString().toStdString().c_str());
    *hnd = PyImport_Import(pName);
    Py_DECREF(pName);
    return (*hnd != nullptr);
}

bool pyCreateFunctionHnd(PyObject** hnd, PyObject** module, const QStringView &name)
{
    *hnd = PyObject_GetAttrString(*module, name.toString().toStdString().c_str());
    return (*hnd != nullptr) && PyCallable_Check(*hnd);
}

} // end PyModuleData

bool PyQt::startPython()
{
    using namespace PyModuleData;

    bool status = (Py_IsInitialized() == 1) ? true : false;

    if (!status)
    {
        do
        {
            status = pyInitConfig();
            if (!status)
            {
                qDebug() << "Python init config error";
                break;
            }

            Py_Initialize();

            status = addModulePath();
            if (!status)
            {
                qDebug() << "Python config path error";
                break;
            }

            status = createFileHandlers();
            if (!status)
            {
                qDebug() << "Python create file handlers error";
                break;
            }

            status = createFunctionHandlers();
            if (!status)
            {
                PyErr_Print();
                qDebug() << "Python create functions handlers error";
                break;
            }

            qDebug() << "Initialize Python interpreter success";
        } while(false);
    }

    return status;
}

bool PyQt::pyInitConfig()
{
    bool result = false;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    PyStatus status = PyConfig_SetString(&config, &config.home,
                                         PyMainPath.toString().toStdWString().data());
    if (!PyStatus_Exception(status))
    {
        status = Py_InitializeFromConfig(&config);
        if (!PyStatus_Exception(status))
        {
            result = true;
        }
    }

    PyConfig_Clear(&config);
    return result;
}

bool PyQt::addModulePath()
{
    PyObject* sysPath = PySys_GetObject("path");
    PyObject* pyPath = PyUnicode_FromString(PyModulePath.toString().toStdString().c_str());
    const qint32 status = PyList_Append(sysPath, pyPath);
    Py_DECREF(pyPath);
    return (status != -1) ? true : false;
}

bool PyQt::createFileHandlers()
{
    using namespace PyModuleData;

    bool status = pyImportModule(&pyFunctionFabricModuleHandler, PyFunctionFabricModuleName);
    status = pyImportModule(&pyFunctionCallerModuleHandler, PyFunctionCallerModuleName) & status;
    status = pyImportModule(&pyFunctionVisualizationModuleHandler, PyFunctionVisualizationModuleName) & status;
    status = pyImportModule(&pyFunctionVisualizationChartsModuleHandler, PyFunctionVisualizationChartsModuleName) & status;

    return status;
}

bool PyQt::createFunctionHandlers()
{
    using namespace PyModuleData;

    bool status = pyCreateFunctionHnd(&pyFunctionCaller,
                                      &pyFunctionCallerModuleHandler,
                                      PyCalcFunctionName);
    status = pyCreateFunctionHnd(&pyFunctionPlotCaller,
                                 &pyFunctionVisualizationModuleHandler,
                                 PyShowCharFunctionName) & status;
    status = pyCreateFunctionHnd(&pySummaryPlotCaller,
                                 &pyFunctionVisualizationChartsModuleHandler,
                                 PyShowCharSummaryName) & status;
    status = pyCreateFunctionHnd(&pyFitnessHistoryPlotCaller,
                                 &pyFunctionVisualizationChartsModuleHandler,
                                 PyShowCharFitnessHistoryName) & status;

    return status;
}

void PyQt::stopPython()
{
    using namespace PyModuleData;

    qDebug() << "Deinit Python modules";
    if (pyFunctionFabricModuleHandler != nullptr)
    {
        Py_DECREF(pyFunctionFabricModuleHandler);
        pyFunctionFabricModuleHandler = nullptr;
    }

    if (pyFunctionCallerModuleHandler != nullptr)
    {
        Py_XDECREF(pyFunctionCaller);
        Py_DECREF(pyFunctionCallerModuleHandler);
        pyFunctionCaller = nullptr;
        pyFunctionCallerModuleHandler = nullptr;
    }

    if (pyFunctionVisualizationModuleHandler != nullptr)
    {
        Py_XDECREF(pyFunctionPlotCaller);
        Py_DECREF(pyFunctionVisualizationModuleHandler);
        pyFunctionPlotCaller = nullptr;
        pyFunctionVisualizationModuleHandler = nullptr;
    }

    if (pyFunctionVisualizationChartsModuleHandler != nullptr)
    {
        Py_XDECREF(pySummaryPlotCaller);
        Py_XDECREF(pyFitnessHistoryPlotCaller);
        Py_DECREF(pyFunctionVisualizationChartsModuleHandler);
        pySummaryPlotCaller = nullptr;
        pyFitnessHistoryPlotCaller = nullptr;
        pyFunctionVisualizationChartsModuleHandler = nullptr;
    }

    Py_Finalize();
}

bool PyQt::makeFunction(const QString& funcName, const size_t dimension)
{
    using namespace PyModuleData;

    bool status = false;

    if ((pyFunctionHandler == nullptr) && (dimension >= 1U))
    {
        pyFunctionHandler = PyObject_CallMethod(pyFunctionFabricModuleHandler,
                                                funcName.toStdString().c_str(),
                                                "(i)",
                                                dimension);
        if (pyFunctionHandler)
        {
            qDebug() << "Create function object success";
            status = true;
        }
        else
        {
            PyErr_Print();
            qDebug() << "Not created Python function instance";
        }
    }
    else
    {
        qDebug() << "Function parameters error";
    }

    return status;
}

void PyQt::discardFunction()
{
    Py_XDECREF(PyModuleData::pyFunctionHandler);
    PyModuleData::pyFunctionHandler = nullptr;
}

bool PyQt::calcPoint(const std::vector<qreal>& point, qreal &val)
{
    using namespace PyModuleData;

    bool status = false;
    if(pyFunctionHandler && pyFunctionCaller)
    {
        PyObject* pyArgsList = PyList_New(point.size());
        for (size_t i = 0U; i < point.size(); ++i)
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

void PyQt::showPlot(const GA::Types::Points &point,
                    const QString &savePath,
                    const bool showFlag)
{
    using namespace PyModuleData;

    if (pyFunctionHandler && pyFunctionPlotCaller)
    {
        PyObject* pyPoint = PyList_New(point.point.size());
        for (size_t i = 0U; i < point.point.size(); ++i)
        {
            PyList_SetItem(pyPoint, i, PyFloat_FromDouble(point.point[i]));
        }

        PyObject* PyPointsList = PyList_New(1);
        PyList_SetItem(PyPointsList, 0, pyPoint);

        constexpr size_t functionArgQuantitiy = 4U;
        PyObject* PySavePlotPath = PyUnicode_FromString(savePath.toStdString().c_str());
        PyObject* PyShowFlag = PyBool_FromLong(showFlag ? 1 : 0);
        PyObject* pyArgsTuple = PyTuple_Pack(functionArgQuantitiy,
                                             PyModuleData::pyFunctionHandler,
                                             PyPointsList,
                                             PySavePlotPath,
                                             PyShowFlag);

        PyObject_CallObject(pyFunctionPlotCaller, pyArgsTuple);

        Py_DECREF(pyPoint);
        Py_DECREF(PyPointsList);
        Py_DECREF(PySavePlotPath);
        Py_DECREF(PyShowFlag);
        Py_DECREF(pyArgsTuple);
    }
}

void PyQt::showStats(const std::vector<GA::Types::Points> &bestValues,
                     const std::vector<qreal> avgValues,
                     const std::vector<qreal> stdValues,
                     const QString &saveBestPlotPath,
                     const QString &saveBestDataPath,
                     const QString &saveStatPlotPath,
                     const QString &saveStatDataPath,
                     const bool showFlag)
{
    using namespace PyModuleData;

    if (pyFunctionVisualizationChartsModuleHandler &&
        pySummaryPlotCaller &&
        pyFitnessHistoryPlotCaller)
    {
        plotBestValues(bestValues,
                       saveBestPlotPath,
                       saveBestDataPath,
                       showFlag);
        plotStatsValues(avgValues,
                        stdValues,
                        saveStatPlotPath,
                        saveStatDataPath,
                        showFlag);
    }
}

void PyQt::plotBestValues(const std::vector<GA::Types::Points> &bestValues,
                          const QString &saveBestPlotPath,
                          const QString &saveBestDataPath,
                          const bool showFlag)
{
    using namespace PyModuleData;

    const size_t chartsSize = bestValues.size();
    PyObject* PyMinValue = PyList_New(chartsSize);
    for (size_t i = 0U; i < chartsSize; ++i)
    {
        PyList_SetItem(PyMinValue, i, PyFloat_FromDouble(bestValues[i].value));
    }

    PyObject* PyPointsList = PyList_New(chartsSize);
    std::vector<PyObject*> pyPoints;
    for (size_t i = 0U; i < chartsSize; ++i)
    {
        pyPoints.emplace_back(PyList_New(bestValues[i].point.size()));
        for (size_t j = 0; j < bestValues[i].point.size(); ++j)
        {
            PyList_SetItem(pyPoints[i], j, PyFloat_FromDouble(bestValues[i].point[j]));
        }
        PyList_SetItem(PyPointsList, i, pyPoints[i]);
    }

    constexpr size_t functionSumaryArgQuantitiy = 6U;
    PyObject* PyChartsSize = PyLong_FromSize_t(chartsSize);
    PyObject* PySavePlotPath = PyUnicode_FromString(saveBestPlotPath.toStdString().c_str());
    PyObject* PySaveDataPath = PyUnicode_FromString(saveBestDataPath.toStdString().c_str());
    PyObject* PyShowFlag = PyBool_FromLong(showFlag ? 1 : 0);
    PyObject* PyArgsTuple = PyTuple_Pack(functionSumaryArgQuantitiy,
                                         PyChartsSize,
                                         PyMinValue,
                                         PySavePlotPath,
                                         PySaveDataPath,
                                         PyShowFlag,
                                         PyPointsList);

    PyObject_CallObject(pySummaryPlotCaller, PyArgsTuple);

    Py_DECREF(PyChartsSize);
    Py_DECREF(PyMinValue);
    for (size_t i = 0; i < chartsSize; ++i)
    {
        Py_DECREF(pyPoints[i]);
    }
    Py_DECREF(PyPointsList);
    Py_DECREF(PySavePlotPath);
    Py_DECREF(PySaveDataPath);
    Py_DECREF(PyShowFlag);
    Py_DECREF(PyArgsTuple);
}

void PyQt::plotStatsValues(const std::vector<qreal> avgValues,
                           const std::vector<qreal> stdValues,
                           const QString &saveStatPlotPath,
                           const QString &saveStatDataPath,
                           const bool showFlag)
{
    using namespace PyModuleData;

    const size_t chartsAvgSize = avgValues.size();
    PyObject* PyChartsSize = PyLong_FromSize_t(chartsAvgSize);
    PyObject* PyAvgPoints = PyList_New(chartsAvgSize);
    for (size_t i = 0U; i < chartsAvgSize; ++i)
    {
        PyList_SetItem(PyAvgPoints, i, PyFloat_FromDouble(avgValues[i]));
    }

    const size_t chartsStdSize = avgValues.size();
    PyObject* PyStdPoints = PyList_New(chartsStdSize);
    for (size_t i = 0U; i < chartsStdSize; ++i)
    {
        PyList_SetItem(PyStdPoints, i, PyFloat_FromDouble(stdValues[i]));
    }

    PyObject* PySavePlotPath = PyUnicode_FromString(saveStatPlotPath.toStdString().c_str());
    PyObject* PySaveDataPath = PyUnicode_FromString(saveStatDataPath.toStdString().c_str());
    PyObject* PyShowFlag = PyBool_FromLong(showFlag ? 1 : 0);

    constexpr size_t functionHistoryArgQuantitiy = 6U;
    PyObject* PyArgsTuple = PyTuple_Pack(functionHistoryArgQuantitiy,
                                         PyChartsSize,
                                         PyAvgPoints,
                                         PyStdPoints,
                                         PySavePlotPath,
                                         PySaveDataPath,
                                         PyShowFlag);

    PyObject_CallObject(pyFitnessHistoryPlotCaller, PyArgsTuple);

    Py_DECREF(PyChartsSize);
    Py_DECREF(PyAvgPoints);
    Py_DECREF(PyStdPoints);
    Py_DECREF(PySavePlotPath);
    Py_DECREF(PySaveDataPath);
    Py_DECREF(PyShowFlag);
    Py_DECREF(PyArgsTuple);
}
