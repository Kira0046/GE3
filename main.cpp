#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region 基盤システムの初期化
    //WindowsAPI
    WinApp* winApp = nullptr;
    winApp = new WinApp();
    winApp->Initialize();

    //DirectXCommon
    DirectXCommon* dxCommon = nullptr;
    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);

    //Input
    Input* input = nullptr;
    input = new Input();
    input->Initialize(winApp);

    //SpriteCommon
    SpriteCommon* spriteCommon = nullptr;
    spriteCommon = new SpriteCommon;
    spriteCommon->Initialize(dxCommon);
    spriteCommon->LoadTexture(0, "texture.png");
    spriteCommon->LoadTexture(1, "reimu.png");

#pragma endregion 基盤システムの初期化

#pragma region 最初のシーンの初期化
    //Sprite
    Sprite* sprite = nullptr;
    sprite = new Sprite();
    sprite->Initialize(spriteCommon);
    sprite->SetTextureIndex(1);


#pragma endregion 最初のシーンの初期化
    // ゲームループ
    while (true) {
#pragma region 基盤システムの更新
        //メッセージ処理
        if (winApp->ProcessMessage()) {
            break;
        }

        XMFLOAT2 position = sprite->GetPosition();
        position.x += 0.5f;
        position.y += 0.5f;
        sprite->SetPosition(position);

        XMFLOAT4 color = sprite->GetColor();
        color.x -= 0.01f;
        sprite->SetColor(color);

        XMFLOAT2 size = sprite->GetSize();
        size.x += 1.0f;
        sprite->SetSize(size);
        
        bool isFlipY = sprite->GetIsFlipY();
        if (input->PushKey(DIK_SPACE)) {
            isFlipY = true;
        }
        else {
            isFlipY = false;
        }
        sprite->SetIsFlipY(isFlipY);

        bool isInvisible = sprite->GetInvisible();
        if (input->PushKey(DIK_A)) {
            isInvisible = true;
        }
        else {
            isInvisible = false;
        }
        sprite->SetIsInvisible(isInvisible);

        //入力の更新処理
        input->Update();
#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新
        sprite->Update();
        

#pragma endregion 最初のシーンの更新

        //描画前処理
        dxCommon->PreDraw();
#pragma region 最初のシーンの描画
        spriteCommon->PreDraw();
        sprite->Draw();

#pragma endregion 最初のシーンの描画

        dxCommon->PostDraw();
       

    }
#pragma region 最初のシーンの終了

#pragma endregion 最初のシーンの終了
    delete sprite;
    sprite = nullptr;

#pragma region 基盤システムの終了
    delete input;
    input = nullptr;

    // ウィンドウクラスを登録解除
    //UnregisterClass(w.lpszClassName, w.hInstance);
    delete dxCommon;
    dxCommon = nullptr;

    delete spriteCommon;
    spriteCommon = nullptr;


    winApp->Finalize();
    delete winApp;
    winApp = nullptr;
#pragma endregion 基盤システムの終了

    return 0;
}
