#include "RailCamera.h"
#include <Function.h>

void RailCamera::Initialize(const Vector3 position, const Vector3 rotation)
{
	//ワールドトランスフォームの初期設定
	worldtransform_.translation_ = position;
	worldtransform_.rotation_ = rotation;
	
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldtransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 200.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{

    //移動
    worldtransform_.translation_ += Vector3(0.005f, 0, 0.005f);
    //ワールドトランスフォームを更新
    worldtransform_.matWorld_ = CreateMatIdentity();
    worldtransform_.matWorld_ = CreateMatScale(worldtransform_.scale_);
    worldtransform_.matWorld_ = CreateMatRotationX(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatRotationY(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatRotationZ(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatTranslation(worldtransform_.translation_);

    //ワールド行列の平行移動成分
    viewProjection_.eye.x = worldtransform_.matWorld_.m[3][0];
    viewProjection_.eye.y = worldtransform_.matWorld_.m[3][1];
    viewProjection_.eye.z = worldtransform_.matWorld_.m[3][2];

    //ワールド前方ベクトル
    Vector3 forward(0, 0, 1);
    //レールカメラの回転を反映
    forward = bvector(forward, worldtransform_.matWorld_);

    //視点から前方に適当な距離進んだ
    forward += viewProjection_.eye;
    viewProjection_.target = forward;

    //ワールド上方
    Vector3 up(0, 1, 0);

    //レールカメラの回転
    viewProjection_.up = bvector(up, worldtransform_.matWorld_);

    //ビュープロジェクションを更新
    viewProjection_.UpdateMatrix();

    /*debugText_->SetPos(50, 130);
    debugText_->Printf(
        "eye:(%f,%f,%f)",
        viewProjection_.eye.x,
        viewProjection_.eye.y,
        viewProjection_.eye.z);*/

    if (input_->PushKey(DIK_F))
    {
        worldtransform_.rotation_.y -= 0.1f;
    }
}