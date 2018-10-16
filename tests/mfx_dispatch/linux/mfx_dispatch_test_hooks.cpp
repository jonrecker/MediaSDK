// Copyright (c) 2017-2018 Intel Corporation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "mfx_dispatch_test_hooks.h"
#include "mfx_dispatch_test_main.h"

#include <mfxcommon.h>
#include <mfxdefs.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#undef FUNCTION
#define FUNCTION(return_value, func_name, formal_param_list, actual_param_list) \
  e##func_name,

enum Function
{
  eMFXInit,
  eMFXInitEx,
  eMFXClose,
  eMFXJoinSession,
#include "mfxvideo_functions.h"
  eFunctionsNum,
  eNoMoreFunctions = eFunctionsNum
};

struct FunctionsTable
{
  Function id;
  const char* name;
  mfxVersion version;
};

#define VERSION(major, minor) {{minor, major}}

#undef FUNCTION
#define FUNCTION(return_value, func_name, formal_param_list, actual_param_list) \
    { e##func_name, #func_name, API_VERSION },


static const FunctionsTable g_mfxFuncTable[] =
{
    { eMFXInit, "MFXInit", VERSION(1, 0) },
    { eMFXInitEx, "MFXInitEx", VERSION(1, 14) },
    { eMFXClose, "MFXClose", VERSION(1, 0) },
    { eMFXJoinSession, "MFXJoinSession", VERSION(1, 1) },
#include "mfxvideo_functions.h"
    { eNoMoreFunctions }
};
#undef FUNCTION

void* MOCK_DLOPEN_HANDLE = reinterpret_cast<void*>(0x0BADCAFE);
void* MOCK_FUNC_PTR = reinterpret_cast<void*>(0xDEADBEEF);
mfxSession MOCK_SESSION_HANDLE = reinterpret_cast<mfxSession>(0xFADEBABE);

// dlopen hooks implementation:

void* TEST_DLOPEN_HOOKS::AlwaysNull(const char *filename, int flag)
{
    return nullptr;
}

void* TEST_DLOPEN_HOOKS::AlwaysMock(const char *filename, int flag)
{
    return MOCK_DLOPEN_HANDLE;
}

void* TEST_DLOPEN_HOOKS::NullThenMock(const char *filename, int flag, bool& run_already)
{
    if (!run_already)
    {
        run_already = true;
        return nullptr;
    }
    return MOCK_DLOPEN_HANDLE;
}

void* TEST_DLOPEN_HOOKS::AlwaysNullLibNameCheck(const char *filename, int flag, HookInternalParams par)
{
#if defined(__i386__)
    const std::string LIBMFXSW("libmfxsw32.so.1");
    const std::string LIBMFXHW("libmfxhw32.so.1");
#elif defined(__x86_64__)
    const std::string LIBMFXSW("libmfxsw64.so.1");
    const std::string LIBMFXHW("libmfxhw64.so.1");
#endif
    std::string modules_dir(MFX_MODULES_DIR);
    std::vector<std::string> libs;

    if (MFX_IMPL_BASETYPE(par.requested_implementation) == MFX_IMPL_AUTO ||
        MFX_IMPL_BASETYPE(par.requested_implementation) == MFX_IMPL_AUTO_ANY)
    {
        libs.emplace_back(LIBMFXHW);
        libs.emplace_back(modules_dir + "/" + LIBMFXHW);
        libs.emplace_back(LIBMFXSW);
        libs.emplace_back(modules_dir + "/" + LIBMFXSW);
    }
    else if ((par.requested_implementation & MFX_IMPL_HARDWARE) ||
             (par.requested_implementation & MFX_IMPL_HARDWARE_ANY))
    {
        libs.emplace_back(LIBMFXHW);
        libs.emplace_back(modules_dir + "/" + LIBMFXHW);
    }
    else if (par.requested_implementation & MFX_IMPL_SOFTWARE)
    {
        libs.emplace_back(LIBMFXSW);
        libs.emplace_back(modules_dir + "/" + LIBMFXSW);
    }
    const std::string supplied_fname(filename);
    EXPECT_TRUE(std::find(libs.begin(), libs.end(), supplied_fname) != libs.end());
    return nullptr;
}

// dlopen hooks implementation:
int TEST_DLCLOSE_HOOKS::AlwaysSuccess(void* handle)
{
    return 0;
}

// dlsym hooks implementation:

void* TEST_DLSYM_HOOKS::AlwaysNull(void *handle, const char *symbol)
{
    return nullptr;
}

void* TEST_DLSYM_HOOKS::EmulateAPIParametrized(void *handle, const char *symbol, HookInternalParams par)
{
    std::string requested_symbol(symbol);
    std::string mfxinitex_symbol("MFXInitEx");
    std::string mfxqueryimpl_symbol("MFXQueryIMPL");
    std::string mfxqueryversion_symbol("MFXQueryVersion");
    std::string mfxclose_symbol("MFXClose");

    if (symbol == mfxinitex_symbol)
    {
        return reinterpret_cast<void*>(MFXInitExHookWrap);
    }
    else if (symbol == mfxqueryimpl_symbol)
    {
        return reinterpret_cast<void*>(MFXQueryIMPLHookWrap);
    }
    else if (symbol == mfxqueryversion_symbol)
    {
        return reinterpret_cast<void*>(MFXQueryVersionHookWrap);
    }
    else if (symbol == mfxclose_symbol)
    {
        return reinterpret_cast<void*>(MFXCloseHookWrap);
    }
    else
    {
        for (int i = 0; i < eFunctionsNum; ++i)
        {
            std::string available_symbol(g_mfxFuncTable[i].name);
            mfxVersion symbol_api_version = g_mfxFuncTable[i].version;
            if (requested_symbol == available_symbol && par.emulated_api_version.Version > symbol_api_version.Version)
            {
                return MOCK_FUNC_PTR;
            }
        }
    }
    return nullptr;
}

void* TEST_DLSYM_HOOKS::NullThenEmulateAPIParametrized(void *handle, const char *symbol, HookInternalParams par, bool& run_already)
{
    if (!run_already)
    {
        run_already = true;
        return nullptr;
    }
    return TEST_DLSYM_HOOKS::EmulateAPIParametrized(handle, symbol, par);
}


// fopen hooks implementation:

FILE* TEST_FOPEN_HOOKS::AlwaysNull(const char *filename, const char *opentype)
{
    return nullptr;
}

FILE* TEST_FOPEN_HOOKS::AlwaysNullPluginPathCheck(const char *filename, const char *opentype)
{
    std::string requested_plugins_path(filename);
    std::string expected_plugins_path = std::string(MFX_PLUGINS_DIR) + "/plugins.cfg";
    EXPECT_EQ(requested_plugins_path, expected_plugins_path);
    return nullptr;
}

// MFXInitEx hook implementation:

mfxStatus TEST_MFXINITEX_HOOKS::AlwaysUnsupported(mfxInitParam par, mfxSession *session)
{
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus TEST_MFXINITEX_HOOKS::AlwaysErrNone(mfxInitParam par, mfxSession *session)
{
    *session = MOCK_SESSION_HANDLE;
    return MFX_ERR_NONE;
}

// MFXQueryVersion hook implementation:

mfxStatus TEST_MFXQUERYVERSION_HOOKS::AlwaysUnsupported(mfxSession session, mfxVersion *pVersion)
{
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus TEST_MFXQUERYVERSION_HOOKS::AlwaysErrNoneParametrized(mfxSession session, mfxVersion *pVersion, HookInternalParams par)
{
    *pVersion = par.emulated_api_version;
    return MFX_ERR_NONE;
}

mfxStatus TEST_MFXQUERYVERSION_HOOKS::AlwaysErrNoneNullVersion(mfxSession session, mfxVersion *pVersion)
{
    *pVersion = {0};
    return MFX_ERR_NONE;
}

// MFXQueryIMPL hook implementation:

mfxStatus TEST_MFXQUERYIMPL_HOOKS::AlwaysUnsupported(mfxSession session, mfxIMPL *impl)
{
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus TEST_MFXQUERYIMPL_HOOKS::AlwaysErrNone(mfxSession session, mfxIMPL *impl)
{
    return MFX_ERR_NONE;
}

// MFXClose hook implementation:

mfxStatus TEST_MFXCLOSE_HOOKS::AlwaysErrNone(mfxSession session)
{
    return MFX_ERR_NONE;
}
