/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILS_TRACE_GRAPHIC_BYTRACE_H
#define UTILS_TRACE_GRAPHIC_BYTRACE_H

#include <string>

class GraphicBytrace {
public:
    static void BytraceBegin(const std::string &proc);
    static void BytraceEnd(const std::string &proc);
};

class ScopedBytrace {
public:
    ScopedBytrace(const std::string &proc);
    ~ScopedBytrace();

private:
    std::string proc_;
};

#endif // UTILS_TRACE_GRAPHIC_BYTRACE_H
