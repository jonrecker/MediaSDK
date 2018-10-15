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

#ifndef MFX_DISPATCH_HOOKS_H
#define MFX_DISPATCH_HOOKS_H

#include <mfxvideo.h>
#include <functional>

struct HookInternalParams
{
    mfxVersion emulated_api_version = {{0, 0}};
    mfxIMPL requested_implementation = MFX_IMPL_UNSUPPORTED;
};

typedef void* HookHandle;

typedef std::function<void* (const char*, int)> DlopenHook;
typedef std::function<void* (void*, const char*)> DlsymHook;


namespace TEST_DLOPEN_HOOKS
{
    void* AlwaysNull(const char *filename, int flag);
    void* AlwaysNullNameCheck(const char *filename, int flag, HookInternalParams par);
    void* AlwaysNullParametrized(const char *filename, int flag, HookInternalParams par);
}

namespace TEST_DLSYM_HOOKS
{
    void* AlwaysNull(void *handle, const char *symbol);
    void* AlwaysNullParametrized(void *handle, const char *symbol, HookInternalParams par);
}

#endif /* MFX_DISPATCH_HOOKS_H */
