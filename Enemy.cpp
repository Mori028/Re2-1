#include "Enemy.h"
#include <cassert>
#include <Function.h>


void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldtransform_.Initialize();
}

void Enemy::Update()
{
	Vector3 move = { 0,0,0 };

	//敵の移動の速さ
	const float enemySpeed = 0.03f;
	//敵の移動処理
	move = { enemySpeed, 0.0f, 0.0f };
	/*worldtransform_.translation_ += move;*/

	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();


	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		AccessPhaseUpdate();
		break;

	case Enemy::Phase::Leave:
		EliminationPhaseUpdate();
		break;

	}
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);
}

void Enemy::AccessPhaseUpdate()
{
	//移動（ベクトルを加算）
	worldtransform_.translation_ -= {0.05f, 0.0f, 0.0f};
	//規定の位置に到達したら離脱
	if (worldtransform_.translation_.x < -10.0f) {
		phase_ = Enemy::Phase::Leave;
	}
}

void Enemy::EliminationPhaseUpdate()
{
	//移動（べクトルを加算）
	worldtransform_.translation_ += {0.02f, 0.02f, 0.0f};
}