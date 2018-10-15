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

#ifndef MFX_DISPATCH_TESTS_MAIN_H
#define MFX_DISPATCH_TESTS_MAIN_H

#include "mfx_dispatch_test_hooks.h"

#include <gtest/gtest.h>
#include <map>

#ifndef MFX_MODULES_DIR
#define MFX_MODULES_DIR "ERROR: MFX_MODULES_DIR was not defined!"
#endif

#ifndef MFX_PLUGINS_DIR
#define MFX_PLUGINS_DIR "ERROR: MFX_PLUGINS_DIR was not defined!"
#endif

extern DlopenHook g_dlopen_hook;
extern DlcloseHook g_dlclose_hook;
extern DlsymHook g_dlsym_hook;
extern FopenHook g_fopen_hook;
extern MfxInitExHook g_mfxinitex_hook;
extern MfxQueryImplHook g_mfxqueryimpl_hook;
extern MfxQueryVersionHook g_mfxqueryversion_hook;
extern MfxCloseHook g_mfxclose_hook;

extern "C"
{
    mfxStatus MFXInitExHookWrap(mfxInitParam par, mfxSession *session);
    mfxStatus MFXQueryVersionHookWrap(mfxSession session, mfxVersion *version);
    mfxStatus MFXQueryIMPLHookWrap(mfxSession session, mfxIMPL *impl);
    mfxStatus MFXCloseHookWrap(mfxSession session);
}

inline void ResetHooks()
{
    g_dlopen_hook          = TEST_DLOPEN_HOOKS::AlwaysNull;
    g_dlclose_hook         = TEST_DLCLOSE_HOOKS::AlwaysSuccess;
    g_dlsym_hook           = TEST_DLSYM_HOOKS::AlwaysNull;
    g_fopen_hook           = TEST_FOPEN_HOOKS::AlwaysNull;
    g_mfxinitex_hook       = TEST_MFXINITEX_HOOKS::AlwaysUnsupported;
    g_mfxqueryimpl_hook    = TEST_MFXQUERYIMPL_HOOKS::AlwaysUnsupported;
    g_mfxqueryversion_hook = TEST_MFXQUERYVERSION_HOOKS::AlwaysUnsupported;
    g_mfxclose_hook        = TEST_MFXCLOSE_HOOKS::AlwaysErrNone;
}


class DispatcherTest : public ::testing::Test
{
public:
    DispatcherTest()
    {
        ResetHooks();
    }
protected:
    mfxIMPL impl = 0;
    mfxVersion ver{};
    mfxSession session = NULL;
    HookInternalParams par {};
};

#endif /* MFX_DISPATCH_TESTS_MAIN_H */
