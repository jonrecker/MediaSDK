// Copyright (c) 2018 Intel Corporation
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
#include <mfxvideo.h>
#include "mfx_dispatch_test_main.h"
#include <functional>

using namespace std::placeholders;

TEST(DispatcherLibraryLoadTest, ShouldFailIfNoLibrayIsFound)
{
    mfxIMPL impl = 0;
    mfxVersion ver{};
    mfxSession session = NULL;

    g_dlopen_hook = TEST_DLOPEN_HOOKS::AlwaysNull;
    g_dlsym_hook  = TEST_DLSYM_HOOKS::AlwaysNull;

    mfxStatus sts = MFXInit(impl, &ver, &session);
    ASSERT_EQ(sts, MFX_ERR_UNSUPPORTED);
}


TEST(DispatcherLibraryLoadTest, ShouldFailIfAvailLibVersionLessThanRequested)
{
    mfxIMPL impl = 0;
    mfxVersion ver{{1, 28}};
    mfxSession session = NULL;

    HookInternalParams par {};
    g_dlopen_hook = std::bind(TEST_DLOPEN_HOOKS::AlwaysNullParametrized, _1, _2, par);
    g_dlsym_hook  = std::bind(TEST_DLSYM_HOOKS::AlwaysNullParametrized, _1, _2, par);

    mfxStatus sts = MFXInit(impl, &ver, &session);
    ASSERT_EQ(sts, MFX_ERR_UNSUPPORTED);
}

