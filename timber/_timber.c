#include <Python.h>
#include <frameobject.h>
#include <tmb/tmb.h>

static int extract_caller_info(const char** filename,
                               int* line_no,
                               const char** funcname,
                               int frame_offset) {
    PyFrameObject* frame = PyEval_GetFrame();
    if (frame == NULL) {
        *filename = "<unknown>";
        *line_no  = 0;
        *funcname = "<unknown>";
        return 0;
    }

    Py_INCREF(frame);

    for (int i = 0; i < frame_offset && frame != NULL; i++) {
        PyFrameObject* prev = frame;
        frame               = PyFrame_GetBack(frame);
        Py_DECREF(prev);
        if (frame != NULL) { Py_INCREF(frame); }
    }

    if (frame == NULL) {
        *filename = "<unknown>";
        *line_no  = 0;
        *funcname = "<unknown>";
        return 0;
    }

    *line_no = PyFrame_GetLineNumber(frame);

    PyCodeObject* code = PyFrame_GetCode(frame);
    if (code != NULL) {
        PyObject* filename_obj = code->co_filename;
        if (filename_obj != NULL && PyUnicode_Check(filename_obj)) {
            *filename = PyUnicode_AsUTF8(filename_obj);
        } else {
            *filename = "<unknown>";
        }

        PyObject* funcname_obj = code->co_name;
        if (funcname_obj != NULL && PyUnicode_Check(funcname_obj)) {
            *funcname = PyUnicode_AsUTF8(funcname_obj);
        } else {
            *funcname = "<unknown>";
        }

        Py_DECREF(code);
    } else {
        *filename = "<unknown>";
        *funcname = "<unknown>";
    }

    Py_DECREF(frame);
    return 1;
}

static const char* get_basename(const char* path) {
    const char* basename = path;
    for (const char* p = path; *p != '\0'; p++) {
        if (*p == '/' || *p == '\\') { basename = p + 1; }
    }
    return basename;
}

static PyObject* py_tmb_print_version(PyObject* self, PyObject* args) {
    (void)self;
    (void)args;
    tmb_print_version();
    Py_RETURN_NONE;
}

static PyObject* py_tmb_get_version(PyObject* self, PyObject* args) {
    (void)self;
    (void)args;
    const char* version = tmb_get_version();
    return PyUnicode_FromString(version);
}

static PyObject* py_tmb_fatal(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_FATAL,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyObject* py_tmb_error(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_ERROR,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyObject* py_tmb_warning(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_WARNING,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyObject* py_tmb_info(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_INFO,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyObject* py_tmb_debug(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_DEBUG,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyObject* py_tmb_trace(PyObject* self, PyObject* args) {
    (void)self;
    const char* message;

    if (!PyArg_ParseTuple(args, "s", &message)) { return NULL; }

    const char* filename;
    int line_no;
    const char* funcname;

    extract_caller_info(&filename, &line_no, &funcname, 0);

    const char* filename_base = get_basename(filename);

    tmb_log_ctx_t ctx = { .log_level         = TMB_LOG_LEVEL_TRACE,
                          .line_no           = line_no,
                          .filename          = filename,
                          .filename_len      = (int)strlen(filename),
                          .filename_base     = filename_base,
                          .filename_base_len = (int)strlen(filename_base),
                          .funcname          = funcname,
                          .funcname_len      = (int)strlen(funcname) };

    tmb_log_default(ctx, "%s", message);
    Py_RETURN_NONE;
}

static PyMethodDef timber_methods[] = {
    { "print_version",
     (PyCFunction)py_tmb_print_version,
     METH_NOARGS, "Print the Timber library version" },
    { "get_version",
     (PyCFunction)py_tmb_get_version,
     METH_NOARGS, "Get the Timber library version string" },

    { "fatal",
     (PyCFunction)py_tmb_fatal,
     METH_VARARGS, "Log a fatal message using the default logger" },
    { "error",
     (PyCFunction)py_tmb_error,
     METH_VARARGS, "Log an error message using the default logger" },
    { "warning",
     (PyCFunction)py_tmb_warning,
     METH_VARARGS, "Log a warning message using the default logger" },
    { "info",
     (PyCFunction)py_tmb_info,
     METH_VARARGS, "Log an info message using the default logger" },
    { "debug",
     (PyCFunction)py_tmb_debug,
     METH_VARARGS, "Log a debug message using the default logger" },
    { "trace",
     (PyCFunction)py_tmb_trace,
     METH_VARARGS, "Log a trace message using the default logger" },
    { NULL}
};

static struct PyModuleDef timber_module = { PyModuleDef_HEAD_INIT,
                                            "_timber",
                                            "Python bindings for Timber "
                                            "logging library",
                                            -1,
                                            timber_methods };

PyMODINIT_FUNC PyInit__timber(void) {
    PyObject* m;

    m = PyModule_Create(&timber_module);
    if (m == NULL) { return NULL; }

    PyModule_AddIntConstant(m, "LEVEL_FATAL", TMB_LEVEL_FATAL);
    PyModule_AddIntConstant(m, "LEVEL_ERROR", TMB_LEVEL_ERROR);
    PyModule_AddIntConstant(m, "LEVEL_WARNING", TMB_LEVEL_WARNING);
    PyModule_AddIntConstant(m, "LEVEL_INFO", TMB_LEVEL_INFO);
    PyModule_AddIntConstant(m, "LEVEL_DEBUG", TMB_LEVEL_DEBUG);
    PyModule_AddIntConstant(m, "LEVEL_TRACE", TMB_LEVEL_TRACE);

    return m;
}