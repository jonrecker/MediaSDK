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
#include "mfx_dispatch_test_mock_call_obj.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mfxvideo.h>
#include <mfxplugin.h>
#include <algorithm>


TEST_F(DispatcherTest, ShouldSearchForPluginsCfgByCorrectPath)
{
    MockCallObj& mock = *g_call_obj_ptr;

    ver = {{MFX_VERSION_MINOR, MFX_VERSION_MAJOR}};
    mock.emulated_api_version = ver;

    EXPECT_CALL(mock, dlopen).Times(AtLeast(1)).WillRepeatedly(Return(MOCK_DLOPEN_HANDLE));
    EXPECT_CALL(mock, dlsym).Times(AtLeast(1)).WillRepeatedly(Invoke(&mock, &MockCallObj::EmulateAPI));
    EXPECT_CALL(mock, MFXInitEx).Times(AtLeast(1)).WillRepeatedly(DoAll(SetArgPointee<1>(MOCK_SESSION_HANDLE), Return(MFX_ERR_NONE)));
    EXPECT_CALL(mock, MFXQueryIMPL).Times(AtLeast(1)).WillRepeatedly(Return(MFX_ERR_NONE));
    EXPECT_CALL(mock, MFXQueryVersion).Times(AtLeast(1)).WillRepeatedly(Invoke(&mock, &MockCallObj::ReturnEmulatedVersion));

    mfxStatus sts = MFXInit(impl, &ver, &session);
    ASSERT_EQ(sts, MFX_ERR_NONE);

    EXPECT_CALL(mock, fopen).Times(AtLeast(1));
    mfxPluginUID uid{0};
    std::fill(uid.Data, uid.Data + sizeof(mfxPluginUID), 0xBE);
    sts = MFXVideoUSER_Load(session, &uid, 0);

}

