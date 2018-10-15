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

#include "gtest/gtest.h"
#include "mfx_dispatch_test_main.h"


DlopenHook g_dlopen_hook(TEST_DLOPEN_HOOKS::AlwaysNull);
DlcloseHook g_dlclose_hook(TEST_DLCLOSE_HOOKS::AlwaysSuccess);
DlsymHook g_dlsym_hook(TEST_DLSYM_HOOKS::AlwaysNull);
FopenHook g_fopen_hook(TEST_FOPEN_HOOKS::AlwaysNull);
MfxInitExHook g_mfxinitex_hook(TEST_MFXINITEX_HOOKS::AlwaysUnsupported);
MfxQueryVersionHook g_mfxqueryversion_hook(TEST_MFXQUERYVERSION_HOOKS::AlwaysUnsupported);
MfxQueryImplHook g_mfxqueryimpl_hook(TEST_MFXQUERYIMPL_HOOKS::AlwaysUnsupported);
MfxCloseHook g_mfxclose_hook(TEST_MFXCLOSE_HOOKS::AlwaysErrNone);



extern "C"
{
    void *dlopen(const char *filename, int flag)
    {
        return g_dlopen_hook(filename, flag);
    }

    int dlclose(void* handle)
    {
        return g_dlclose_hook(handle);
    }

    void *dlsym(void *handle, const char *symbol)
    {
        return g_dlsym_hook(handle, symbol);
    }

    FILE * fopen(const char *filename, const char *opentype)
    {
        return g_fopen_hook(filename, opentype);
    }

    // Pointers to these wrappers are returned inside the dlsym hook,
    // because actual hooks are stored as std::function objects, produced
    // (generally speaking) via an std::bind, and therefore cannot be cast to
    // the proper MFXInitEx/MFXQueryVersion/etc. prototypes inside the dlsym hook.
    mfxStatus MFXInitExHookWrap(mfxInitParam par, mfxSession *session)
    {
        return g_mfxinitex_hook(par, session);
    }

    mfxStatus MFXQueryVersionHookWrap(mfxSession session, mfxVersion *version)
    {
        return g_mfxqueryversion_hook(session, version);
    }

    mfxStatus MFXQueryIMPLHookWrap(mfxSession session, mfxIMPL *impl)
    {
        return g_mfxqueryimpl_hook(session, impl);
    }

    mfxStatus MFXCloseHookWrap(mfxSession session)
    {
        return g_mfxclose_hook(session);
    }

} // extern "C"





int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
