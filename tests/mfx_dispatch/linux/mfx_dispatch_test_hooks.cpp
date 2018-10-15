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

#include <gtest/gtest.h>
#include <mfxcommon.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// dlopen hooks implementation:

void* TEST_DLOPEN_HOOKS::AlwaysNull(const char *filename, int flag)
{
    return nullptr;
}

void* TEST_DLOPEN_HOOKS::AlwaysNullNameCheck(const char *filename, int flag, HookInternalParams par)
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


void* TEST_DLOPEN_HOOKS::AlwaysNullParametrized(const char *filename, int flag, HookInternalParams par)
{
    std::cout << "Emulated API version " << par.emulated_api_version.Major << '.' << par.emulated_api_version.Minor << std::endl;
    return nullptr;
}

// dlsym hooks implementation:

void* TEST_DLSYM_HOOKS::AlwaysNull(void *handle, const char *symbol)
{
    return nullptr;
}


void* TEST_DLSYM_HOOKS::AlwaysNullParametrized(void *handle, const char *symbol, HookInternalParams par)
{
    std::cout << "Emulated API version " << par.emulated_api_version.Major << '.' << par.emulated_api_version.Minor << std::endl;
    return nullptr;
}
