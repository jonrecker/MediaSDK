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
#include <iostream>
// dlopen hooks implementation:

void* TEST_DLOPEN_HOOKS::AlwaysNull(const char *filename, int flag)
{
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
