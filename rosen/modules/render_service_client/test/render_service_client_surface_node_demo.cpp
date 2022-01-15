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

#include <iostream>
#include <surface.h>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <securec.h>
#include <sstream>
#include <unistd.h>

#include "display_type.h"
#include "wm/window.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkImageInfo.h"
#include "transaction/rs_transaction.h"
#include "ui/rs_root_node.h"
#include "ui/rs_display_node.h"
#include "ui/rs_surface_node.h"
#include "ui/rs_surface_extractor.h"
#include "ui/rs_ui_director.h"
#include "image/bitmap.h"
#include "draw/canvas.h"
#include "draw/color.h"

#include "media_callback.h"

#include "player.h"
#include "nocopyable.h"
#include "media_data_source.h"

const char* SURFACE_STRIDE_ALIGNMENT = "8";
constexpr int32_t SURFACE_QUEUE_SIZE = 5;

using namespace OHOS;
using namespace OHOS::Rosen;
using namespace OHOS::Rosen::Drawing;
using namespace std;

std::map<std::string, std::function<int32_t()>> playerTable;

void RegisterTable(std::shared_ptr<OHOS::Media::Player> player)
{
    (void)playerTable.emplace("prepare", std::bind(&OHOS::Media::Player::Prepare, player));
    (void)playerTable.emplace("prepareasync", std::bind(&OHOS::Media::Player::PrepareAsync, player));
    (void)playerTable.emplace("", std::bind(&OHOS::Media::Player::Play, player)); // ENTER -> play
    (void)playerTable.emplace("play", std::bind(&OHOS::Media::Player::Play, player));
    (void)playerTable.emplace("pause", std::bind(&OHOS::Media::Player::Pause, player));
    (void)playerTable.emplace("stop", std::bind(&OHOS::Media::Player::Stop, player));
    (void)playerTable.emplace("reset", std::bind(&OHOS::Media::Player::Reset, player));
    (void)playerTable.emplace("release", std::bind(&OHOS::Media::Player::Release, player));
}

void DoNext()
{
    cout << "Enter your step:" << endl;
    std::string cmd;
    while (std::getline(std::cin, cmd)) {
        auto iter = playerTable.find(cmd);
        if (iter != playerTable.end()) {
            auto func = iter->second;
            if (func() != 0) {
                cout << "Operation error" << endl;
            }
            continue;
        } else if (cmd.find("quit") != std::string::npos || cmd == "q") {
            break;
        }
    }
}


void Init(std::shared_ptr<RSUIDirector> rsUiDirector, int width, int height)
{
    std::cout << "rs app demo Init Rosen Backend!" << std::endl;

    if (!rsUiDirector) {
        return;
    }
    rsUiDirector->SetSurfaceNodeSize(width, height);
    rsUiDirector->Init();
    std::cout << "Init Rosen Backend" << std::endl;

    auto rootNode = RSRootNode::Create();
    cout << "RootNode id = " << rootNode->GetId() << endl;
    rootNode->SetFrame(0, 0, width, height);
    rootNode->SetBackgroundColor(SK_ColorWHITE);
    rsUiDirector->SetRoot(rootNode->GetId());

    auto canvasNode = RSCanvasNode::Create();
    cout << "canvasNode id = " << canvasNode->GetId() << endl;
    canvasNode->SetFrame(100, 100, 960, 1000);
    canvasNode->SetBackgroundColor(SK_ColorRED);
    rootNode->AddChild(canvasNode, -1);

    struct RSSurfaceNodeConfig rsSurfaceNodeConfig;
    auto surfaceNode = RSSurfaceNode::Create(rsSurfaceNodeConfig, false);
    cout << "surfaceNode id = " << surfaceNode->GetId() << endl;
    surfaceNode->SetBounds(300, 300, 960, 540);

    canvasNode->AddChild(surfaceNode, -1);
    rsUiDirector->SendMessages();
    auto player = OHOS::Media::PlayerFactory::CreatePlayer();
    if (player == nullptr) {
        cout << "player is null" << endl;
        return;
    }
    RegisterTable(player);
    std::shared_ptr<OHOS::Ace::MediaCallback> cb = std::make_shared<OHOS::Ace::MediaCallback>();
    int32_t media_ret = -1;
    media_ret = player->SetPlayerCallback(cb);
    if (media_ret != 0) {
        cout << "SetPlayerCallback fail" << endl;
    }
    // Use Local file source here
    // path is /data/H264_Main.mp4
    media_ret = player->SetSource("/data/H264_Main.mp4");
    if (media_ret != 0) {
        cout << "SetSource fail" << endl;
        return;
    }
    sptr<Surface> producerSurface = nullptr;
    producerSurface = surfaceNode->GetSurface();
    producerSurface->SetQueueSize(SURFACE_QUEUE_SIZE);
    producerSurface->SetUserData("SURFACE_STRIDE_ALIGNMENT", SURFACE_STRIDE_ALIGNMENT);
    producerSurface->SetUserData("SURFACE_FORMAT", std::to_string(PIXEL_FMT_RGBA_8888));
    if (producerSurface != nullptr) {
        media_ret = player->SetVideoSurface(producerSurface);
        if (media_ret != 0) {
            cout << "SetVideoSurface fail" << endl;
        }
    }

    media_ret = player->PrepareAsync();
        if (media_ret !=  0) {
            cout << "PrepareAsync fail" << endl;
            return;
        }
    DoNext();
    sleep(10);
}

int main()
{
    std::cout << "rs app demo start!" << std::endl;
    sptr<WindowOption> option = new WindowOption();
    option->SetWindowMode(WindowMode::WINDOW_MODE_FLOATING);
    option->SetWindowRect({200, 200, 1501, 1200});
    auto window = Window::Create("app_demo", option);

    window->Show();
    auto rect = window->GetRect();
    std::cout << "rs app demo create window " << rect.width_ << " " << rect.height_ << std::endl;
    auto windowSurfaceNode = window->GetSurfaceNode();
    cout << "windowSurfaceNode id = " << windowSurfaceNode->GetId() << endl;

    auto rsUiDirector = RSUIDirector::Create();
    rsUiDirector->Init();
    RSTransaction::FlushImplicitTransaction();
    sleep(1);

    rsUiDirector->SetRSSurfaceNode(windowSurfaceNode);
    Init(rsUiDirector, rect.width_, rect.height_);
    std::cout << "rs app demo end!" << std::endl;
    window->Hide();
    window->Destroy();
    return 0;
}