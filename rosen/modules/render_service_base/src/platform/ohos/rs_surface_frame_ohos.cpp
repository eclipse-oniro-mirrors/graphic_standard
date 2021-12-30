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

#include "rs_surface_frame_ohos.h"

#include "platform/common/rs_log.h"

namespace OHOS {
namespace Rosen {

RSSurfaceFrameOhos::RSSurfaceFrameOhos(int32_t width, int32_t height)
{
    requestConfig_.width = width;
    requestConfig_.height = height;
}

void RSSurfaceFrameOhos::SetDamageRegion(int32_t left, int32_t top, int32_t width, int32_t height)
{
    flushConfig_.damage.x = left;
    flushConfig_.damage.y = top;
    flushConfig_.damage.w = width;
    flushConfig_.damage.h = height;
}

std::shared_ptr<SkCanvas> RSSurfaceFrameOhos::GetCanvas()
{
    if (buffer_ == nullptr || buffer_->GetWidth() <= 0 || buffer_->GetHeight() <= 0) {
        ROSEN_LOGW("buffer is invalid");
        return nullptr;
    }
    if (canvas_ == nullptr) {
        CreateCanvas();
    }

    return canvas_;
}

void RSSurfaceFrameOhos::CreateCanvas()
{
    auto addr = static_cast<uint32_t*>(buffer_->GetVirAddr());
    if (addr == nullptr ) {
        ROSEN_LOGW("buffer addr is invalid");
        return;
    }
    SkImageInfo info = SkImageInfo::Make(buffer_->GetWidth(), buffer_->GetHeight(),
                                        kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    auto uniqueCanvasPtr = SkCanvas::MakeRasterDirect(info, addr, buffer_->GetSize() / buffer_->GetHeight());
    canvas_ = std::move(uniqueCanvasPtr);
}

int32_t RSSurfaceFrameOhos::GetReleaseFence() const
{
    return releaseFence_;
}

void RSSurfaceFrameOhos::SetReleaseFence(const int32_t& fence)
{
    releaseFence_ = fence;
}

} // namespace Rosen
} // namespace OHOS