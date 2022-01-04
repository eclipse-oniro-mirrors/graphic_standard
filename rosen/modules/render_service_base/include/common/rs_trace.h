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

#ifndef RENDER_SERVICE_BASE_ADAPTER_GENERIC_RS_TRACE_H
#define RENDER_SERVICE_BASE_ADAPTER_GENERIC_RS_TRACE_H

#ifdef ROSEN_OHOS
#include "bytrace.h"
#define ROSEN_TRACE_BEGIN(tag, name) StartTrace(tag, name)
#define ROSEN_TRACE_END(tag) FinishTrace(tag);
#else
#define ROSEN_TRACE_BEGIN(tag, name)
#define ROSEN_TRACE_END(tag)
#endif

#endif // RENDER_SERVICE_BASE_ADAPTER_GENERIC_RS_TRACE_H