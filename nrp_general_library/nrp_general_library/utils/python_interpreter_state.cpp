//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_general_library/utils/json_converter.h"

#include <boost/python/numpy.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

// Returns the first "python" found in PATH (e.g. an active venv), or an empty string
std::wstring find_venv_python() {
    std::vector<std::string> paths;
    std::string pathEnv = getenv("PATH");
    boost::split(paths, pathEnv, boost::is_any_of(";:"));
    for (std::string path : paths)
    {
      boost::filesystem::path pythonPath = boost::filesystem::path(path) / "python";
      if (boost::filesystem::exists(pythonPath))
      {
        std::string pythonProgramName = pythonPath.string();
        return std::wstring(pythonProgramName.begin(), pythonProgramName.end());
      }
    }
    return std::wstring();
}

void check_py_status(PyConfig &config, const PyStatus &status) {
    if(PyStatus_Exception(status))
    {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
    }
}

// Initializes Python with the PyConfig API (Py_SetProgramName and PySys_SetArgv are deprecated since Python 3.11)
void initialize_python(int argc, wchar_t **argv) {
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // Arguments are parsed by NRP, Python must not interpret them
    config.parse_argv = 0;

    if(argc >= 1)
        check_py_status(config, PyConfig_SetString(&config, &config.program_name, argv[0]));

    const std::wstring venvPython = find_venv_python();
    if(!venvPython.empty())
        check_py_status(config, PyConfig_SetString(&config, &config.program_name, venvPython.c_str()));

    if(argc >= 1)
        check_py_status(config, PyConfig_SetArgv(&config, argc, argv));

    check_py_status(config, Py_InitializeFromConfig(&config));
    PyConfig_Clear(&config);

    // Prepend the directory of argv[0] to sys.path, as PySys_SetArgv used to do
    if(argc >= 1)
    {
        boost::filesystem::path scriptPath(argv[0]);
        if(boost::filesystem::is_symlink(scriptPath))
            scriptPath = boost::filesystem::absolute(boost::filesystem::read_symlink(scriptPath), scriptPath.parent_path());

        PyObject *pathEntry = PyUnicode_FromWideChar(scriptPath.parent_path().wstring().c_str(), -1);
        PyList_Insert(PySys_GetObject("path"), 0, pathEntry);
        Py_XDECREF(pathEntry);
    }
}


PythonInterpreterState::PythonInterpreterState(int argc, const char *const *argv, bool allowThreads)
    : _wcharArgs(argc, argv)
{
    assert(argc >= 1);

    initialize_python(this->_wcharArgs.getWCharSize(), this->_wcharArgs.getWCharTPointers());
    json_converter::initNumpy();
    boost::python::numpy::initialize();

    if(allowThreads)
        this->_state = PyEval_SaveThread();
    else
        this->_state = nullptr;
}

PythonInterpreterState::PythonInterpreterState(int argc, const std::vector<const char*> &argv, bool allowThreads)
    : PythonInterpreterState(argc, argv.data(), allowThreads)
{}

PythonInterpreterState::PythonInterpreterState(bool allowThreads)
    : _wcharArgs(0, nullptr)
{
    initialize_python(0, nullptr);
    json_converter::initNumpy();
    boost::python::numpy::initialize();

    if(allowThreads)
        this->_state = PyEval_SaveThread();
    else
        this->_state = nullptr;
}

void PythonInterpreterState::allowThreads()
{
    if(this->_state == nullptr)
        this->_state = PyEval_SaveThread();
}

bool PythonInterpreterState::threadsAllowed() const
{
    return (this->_state != nullptr);
}

void PythonInterpreterState::endAllowThreads()
{
    if(this->_state != nullptr)
    {
        PyEval_RestoreThread(this->_state);
        this->_state = nullptr;
    }
}

PythonInterpreterState::~PythonInterpreterState()
{
    this->endAllowThreads();
}

PythonGILLock::PythonGILLock(PyGILState_STATE &state, const bool acquire)
    : _state(&state)
{
    if(acquire)
        this->acquire();
}

PythonGILLock::~PythonGILLock()
{
    this->release();
    this->_state = nullptr;
}

void PythonGILLock::acquire()
{
    *(this->_state) = PyGILState_Ensure();
}

void PythonGILLock::release()
{
    PyGILState_Release(*(this->_state));
}
