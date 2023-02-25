#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"

#include "CollisionPrimitive.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>


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

    //3Dオブジェクト静的初期化
    Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
   
    Model* model = Model::StaticCreateFromOBJ("Ccopy");
    Model* modelH = Model::StaticCreateFromOBJ("C");
    Model* modelground = Model::StaticCreateFromOBJ("g");
    //3Dオブジェクト生成
    Object3d* object3d1 = Object3d::Create(model);
    Object3d* object3d2 = Object3d::Create(modelground);
    
    XMFLOAT3 position = { 0,-30,0 };
    object3d2->SetPosition(position);

    XMFLOAT3 ballposition{ 0.0f,0.0f,20.0f };
    object3d1->SetPosition(ballposition);


    //
    Sphere sphere;
    Plane plane;

    sphere.center = object3d1->GetPosition();
    sphere.radius =19.0f;

    plane.normal = XMVectorSet(0, 1, 0, 0);
    plane.distance = -30.0f;
    

    //
    DirectX::XMFLOAT3 move = { 0.0f,0.0f,20.0f };

#pragma endregion 基盤システムの初期化

#pragma region 最初のシーンの初期化
    //Sprite
    Sprite* sprite = nullptr;
    sprite = new Sprite();
    sprite->Initialize(spriteCommon,1);
   // sprite->SetTextureIndex(1);


#pragma endregion 最初のシーンの初期化
    // ゲームループ
    while (true) {
#pragma region 基盤システムの更新
        //メッセージ処理
        if (winApp->ProcessMessage()) {
            break;
        }

        

        if (input->PushKey(DIK_W)) {
            move.y += 0.1;
            object3d1->SetPosition(move);
            XMVECTOR moveY = { 0,0.1f,0,0 };
            sphere.center += moveY;
        }

        if (input->PushKey(DIK_S)) {
            move.y -= 0.1;
            object3d1->SetPosition(move);
            XMVECTOR moveY = { 0,0.1f,0,0 };
            sphere.center -= moveY;
        }

        bool hit = Collision::CheckSphere2Plane(sphere, plane);
        if (hit) {
            object3d1->SetModel(modelH);
        }
        else {
            object3d1->SetModel(model);
        }
        
       

       

        //入力の更新処理
        input->Update();
        //3Dオブジェクト更新処理
        object3d1->Update();
        object3d2->Update();

#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新
        sprite->Update();
        

#pragma endregion 最初のシーンの更新

        //描画前処理
        dxCommon->PreDraw();
#pragma region 最初のシーンの描画
        spriteCommon->PreDraw();
        //sprite->Draw();

        Object3d::PreDraw(dxCommon->GetCommandList());
        
        object3d1->Draw();
        object3d2->Draw();

        Object3d::PostDraw();

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

    delete object3d1;
    delete object3d2;
    object3d1 = nullptr;

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
