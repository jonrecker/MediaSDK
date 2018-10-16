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

#include "mfx_dispatch_test_main.h"

#include "gtest/gtest.h"
#include "mfxplugin.h"
#include <mfxvideo.h>
#include <functional>

using namespace std::placeholders;

TEST_F(DispatcherTest, ShouldSearchForPluginsCfgByCorrectPath)
{
    ver = {{28, 1}};
    par.emulated_api_version = ver;
    g_dlopen_hook = TEST_DLOPEN_HOOKS::AlwaysMock;
    g_dlsym_hook = std::bind(TEST_DLSYM_HOOKS::EmulateAPIParametrized, _1, _2, par);
    g_fopen_hook = TEST_FOPEN_HOOKS::AlwaysNullPluginPathCheck;
    g_mfxinitex_hook = TEST_MFXINITEX_HOOKS::AlwaysErrNone;
    g_mfxqueryimpl_hook = TEST_MFXQUERYIMPL_HOOKS::AlwaysErrNone;
    g_mfxqueryversion_hook = std::bind(TEST_MFXQUERYVERSION_HOOKS::AlwaysErrNoneParametrized, _1, _2, par);

    mfxStatus sts = MFXInit(impl, &ver, &session);
    ASSERT_EQ(sts, MFX_ERR_NONE);
    mfxPluginUID uid{0};
    std::fill(uid.Data, uid.Data + sizeof(mfxPluginUID), 0xBE);
    sts = MFXVideoUSER_Load(session, &uid, 0);
}

