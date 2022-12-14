#include "Player.h"
#include "Function.h"
#include <cassert>

/// <summary>
/// 初期化
/// 引数からモデルデータとテクスチャデータを受け取る
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle){

	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
	//3Dレティクルの初期化
	worldtransform3DReticle_.Initialize();
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	const float kCharacterJumpSpeed = 0.6f;
	const float gravitySpeed = 0.15f;

	//常に重力を足してく
	move.y -= gravitySpeed;
	//プレイヤーの移動
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	/*if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}*/
	//ジャンプ
	if (input_->TriggerKey(DIK_SPACE)) {
		if (jumpFlg == 0) {
			jumpFlg = 1;
		}
	}
	if (jumpFlg == 1) {
		jumpTimer--;
	}
	if (jumpTimer <= 25 && jumpTimer >= 1) {
		move.y += kCharacterJumpSpeed;
	}
	if (jumpTimer <= -80) {
		jumpFlg = 0;
		jumpTimer = 26;
	}
	
	//キーボード入力による移動処理
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 10;

	//範囲を超えない処理
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	//自機のワールド座標から3Dレティクルのワールド座標を計算

	//自機から3Dレティクルへの距離
	/*const float kDistancePlayerTo3DReticle = 60.0f;*/
	//自機から3Dレティクルへのオフセット(Z+向き)
	/*Vector3 offset = { 0, 0, 1.0f };*/
	//自機のワールド座標の回転を反映

}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

}
