#include "emscriptenmodule.c"
#include "browsermodule.c"

#include "Python.h"
#include <emscripten.h>

extern PyObject *PyInit_core();
extern PyObject *PyInit_direct();

extern void init_libOpenALAudio();
extern void init_libpnmimagetypes();
extern void init_libwebgldisplay();

extern void task_manager_poll();

EMSCRIPTEN_KEEPALIVE void loadPython() {
    PyConfig config;
    PyConfig_InitIsolatedConfig(&config);
    config.pathconfig_warnings = 0;
    config.use_environment = 0;
    config.write_bytecode = 0;
    config.site_import = 0;
    config.user_site_directory = 0;
    config.buffered_stdio = 0;

    PyStatus status = Py_InitializeFromConfig(&config);
    if (!PyStatus_Exception(status)) {
        fprintf(stderr, "Python %s\\n", Py_GetVersion());

        EM_ASM({
            Module.setStatus('Importing Panda3D...');
            window.setTimeout(_loadPanda, 0);
        });
    }
    PyConfig_Clear(&config);
}

EMSCRIPTEN_KEEPALIVE void loadPanda() {
    PyObject *panda3d_module = PyImport_AddModule("panda3d");
    PyModule_AddStringConstant(panda3d_module, "__package__", "panda3d");
    PyModule_AddObject(panda3d_module, "__path__", PyList_New(0));

    PyObject *panda3d_dict = PyModule_GetDict(panda3d_module);

    PyObject *core_module = PyInit_core();
    PyDict_SetItemString(panda3d_dict, "core", core_module);

    PyObject *direct_module = PyInit_direct();
    PyDict_SetItemString(panda3d_dict, "direct", direct_module);

    //PyObject *physics_module = PyInit_physics();
    //PyDict_SetItemString(panda3d_dict, "physics", physics_module);

    PyObject *sys_modules = PySys_GetObject("modules");
    PyDict_SetItemString(sys_modules, "panda3d.core", core_module);
    PyDict_SetItemString(sys_modules, "panda3d.direct", direct_module);

    PyDict_SetItemString(sys_modules, "emscripten", PyInit_emscripten());
    PyDict_SetItemString(sys_modules, "browser", PyInit_browser());

    init_libOpenALAudio();
    init_libpnmimagetypes();
    init_libwebgldisplay();

    EM_ASM({
        Module.setStatus('Done!');
    });
}

EMSCRIPTEN_KEEPALIVE void stopPythonCode() {
    emscripten_cancel_main_loop();
    PyRun_SimpleString("import builtins, gc, sys\\nsys.modules.pop('__main__', None)\\nsys.modules.pop('direct.directbase.DirectStart', None)\\nif hasattr(builtins, 'base'):\\n    base.taskMgr.destroy()\\n    base.destroy()\\nif hasattr(builtins, 'cpMgr'):\\n    while cpMgr.get_num_explicit_pages():\\n        cpMgr.delete_explicit_page(cpMgr.get_explicit_page(0))\\nif hasattr(builtins, 'base'):\\n    del builtins.base\\nif hasattr(builtins, 'taskMgr'):\\n    del builtins.taskMgr\\ngc.collect()\\n");
}

EMSCRIPTEN_KEEPALIVE void runPythonCode(char *codeToExecute) {
    if (PyRun_SimpleString(codeToExecute)) {
        // An exception occurred.
        stopPythonCode();
    } else {
        emscripten_set_main_loop(&task_manager_poll, 0, 0);
        EM_ASM({
            document.getElementById('stop-button').disabled = false;
        });
    }
}