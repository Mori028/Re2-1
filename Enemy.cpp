#include "Enemy.h"
#include <cassert>


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

}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);
}
