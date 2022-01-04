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

#ifndef HDI_BACKEND_HDI_BACKEND_H
#define HDI_BACKEND_HDI_BACKEND_H

#include <functional>
#include <unordered_map>
#include <refbase.h>

#include "hdi_device.h"
#include "hdi_screen.h"
#include "hdi_layer.h"
#include "hdi_output.h"
#include "surface_type.h"

namespace OHOS {
namespace Rosen {

using OutputPtr = std::shared_ptr<HdiOutput>;

struct PrepareCompleteParam {
    bool needFlushFramebuffer;
    std::vector<LayerInfoPtr> layers;
    std::vector<CompositionType> types;
};

using OnScreenHotplugFunc = std::function<void(OutputPtr &output, bool connected, void* data)>;
using OnPrepareCompleteFunc = std::function<void(sptr<Surface> &surface,
                                                 const struct PrepareCompleteParam &param, void* data)>;

class HdiBackend {
public:
    /* for RS begin */
    static HdiBackend* GetInstance();
    RosenError RegScreenHotplug(OnScreenHotplugFunc func, void* data);
    RosenError RegPrepareComplete(OnPrepareCompleteFunc func, void* data);
    void Repaint(std::vector<OutputPtr> &outputs);
    /* for RS end */

private:
    HdiBackend() = default;
    virtual ~HdiBackend() = default;

    HdiBackend(const HdiBackend& rhs) = delete;
    HdiBackend& operator=(const HdiBackend& rhs) = delete;
    HdiBackend(HdiBackend&& rhs) = delete;
    HdiBackend& operator=(HdiBackend&& rhs) = delete;

    Base::HdiDevice *device_ = nullptr;
    void* onHotPlugCbData_ = nullptr;
    void* onPrepareCompleteCbData_ = nullptr;
    OnScreenHotplugFunc onScreenHotplugCb_ = nullptr;
    OnPrepareCompleteFunc onPrepareCompleteCb_ = nullptr;
    std::unordered_map<uint32_t, OutputPtr> outputs_;

    static void OnHdiBackendHotPlugEvent(uint32_t deviceId, bool connected, void *data);
    RosenError InitDevice();
    void OnHdiBackendConnected(uint32_t screenId);
    void OnHdiBackendDisconnected(uint32_t screenId);
    void CreateHdiOutput(uint32_t screenId);
    void OnScreenHotplug(uint32_t screenId, bool connected);
    void SetHdiLayerInfo(uint32_t screenId, uint32_t layerId, LayerPtr &layer);
    void OnPrepareComplete(bool needFlush, OutputPtr &output,
        std::vector<LayerInfoPtr> &changedLayerInfos, std::vector<CompositionType> &compTypes);
    void ReleaseLayerBuffer(uint32_t screenId, const std::unordered_map<uint32_t, LayerPtr> &layersMap);
    int32_t FlushScreen(uint32_t screenId, OutputPtr &output, std::vector<LayerPtr> &changedLayers);
    int32_t SetScreenClientInfo(uint32_t screenId, const sptr<SyncFence> &fbAcquireFence, OutputPtr &output);
    int32_t GetCompChangedLayers(uint32_t screenId, std::vector<LayerPtr> &changedLayers,
        std::vector<LayerInfoPtr> &changedLayerInfos, std::vector<CompositionType> &compTypes,
        const std::unordered_map<uint32_t, LayerPtr> &layersMap);

    inline void CheckRet(int32_t ret, const char* func);
    int32_t newScreenId_ = -1;
    void UpdateScreenId(uint32_t screenId);
    int32_t GetScreenId();
};
} // namespace Rosen
} // namespace OHOS

#endif // HDI_BACKEND_HDI_BACKEND_H