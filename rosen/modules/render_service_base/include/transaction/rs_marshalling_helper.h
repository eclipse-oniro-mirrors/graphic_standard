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

#ifndef RENDER_SERVICE_BASE_TRANSACTION_RS_MARSHALLING_HELPER_H
#define RENDER_SERVICE_BASE_TRANSACTION_RS_MARSHALLING_HELPER_H

#ifdef ROSEN_OHOS

#include <parcel.h>

#include "common/rs_common_def.h"

template<typename T>
class sk_sp;
class SkData;
class SkFlattenable;
class SkPath;

namespace OHOS {
namespace Rosen {
class RSPath;
class RSShader;
template<typename T>
class RSRenderCurveAnimation;
template<typename T>
class RSRenderKeyframeAnimation;
class RSRenderTransition;
class RSRenderPathAnimation;

class RSMarshallingHelper {
public:
    // default marshalling and unmarshalling method for POD types
    // TODO: implement marshalling & unmarshalling methods for other types (e.g. RSImage, drawCMDList)
    template<typename T>
    static bool Marshalling(Parcel& parcel, const T& val)
    {
        return parcel.WriteUnpadBuffer(&val, sizeof(T));
    }
    template<typename T>
    static bool Unmarshalling(Parcel& parcel, T& val)
    {
        if (const uint8_t* buff = parcel.ReadUnpadBuffer(sizeof(T))) {
            val = *(reinterpret_cast<const T*>(buff));
            return true;
        }
        return false;
    }

    static bool Marshalling(Parcel& parcel, const std::shared_ptr<RSRenderPathAnimation>& val);
    static bool Unmarshalling(Parcel& parcel, std::shared_ptr<RSRenderPathAnimation>& val);

    static bool Marshalling(Parcel& parcel, const std::shared_ptr<RSRenderTransition>& val);
    static bool Unmarshalling(Parcel& parcel, std::shared_ptr<RSRenderTransition>& val);

    // specialized marshalling & unmarshalling function for certain types
#define DECLEAR_SPECIALIZATION(TYPE)                   \
    template<>                                         \
    bool Marshalling(Parcel& parcel, const TYPE& val); \
    template<>                                         \
    bool Unmarshalling(Parcel& parcel, TYPE& val);

    DECLEAR_SPECIALIZATION(bool)
    DECLEAR_SPECIALIZATION(int8_t)
    DECLEAR_SPECIALIZATION(uint8_t)
    DECLEAR_SPECIALIZATION(int16_t)
    DECLEAR_SPECIALIZATION(uint16_t)
    DECLEAR_SPECIALIZATION(int32_t)
    DECLEAR_SPECIALIZATION(uint32_t)
    DECLEAR_SPECIALIZATION(int64_t)
    DECLEAR_SPECIALIZATION(uint64_t)
    DECLEAR_SPECIALIZATION(float)
    DECLEAR_SPECIALIZATION(double)
    DECLEAR_SPECIALIZATION(sk_sp<SkData>)
    DECLEAR_SPECIALIZATION(sk_sp<SkFlattenable>)
    DECLEAR_SPECIALIZATION(SkPath)
    DECLEAR_SPECIALIZATION(RSShader)
    DECLEAR_SPECIALIZATION(RSPath)
#undef DECLEAR_SPECIALIZATION

#define DECLEAR_TEMPLATE_OVERLOAD(TEMPLATE)                                           \
    template<typename T>                                                              \
    static bool Marshalling(Parcel& parcel, const std::shared_ptr<TEMPLATE<T>>& val); \
    template<typename T>                                                              \
    static bool Unmarshalling(Parcel& parcel, std::shared_ptr<TEMPLATE<T>>& val);

    DECLEAR_TEMPLATE_OVERLOAD(RSRenderCurveAnimation)
    DECLEAR_TEMPLATE_OVERLOAD(RSRenderKeyframeAnimation)
#undef DECLEAR_TEMPLATE_OVERLOAD
};

} // namespace Rosen
} // namespace OHOS

#endif // ROSEN_OHOS
#endif // RENDER_SERVICE_BASE_TRANSACTION_RS_MARSHALLING_HELPER_H