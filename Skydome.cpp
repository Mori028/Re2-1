#include "Skydome.h"
#include <Function.h>

void Skydome::Initialize()
{
	worldtransform_.Initialize();
	//スケーリングの設定
	worldtransform_.scale_ = { 250.0f, 250.0f, 250.0f };
	//スケーリング行列
	worldtransform_.matWorld_ = CreateMatScale(worldtransform_.scale_);
	//行列の転送
	worldtransform_.TransferMatrix();

	model_ = Model::CreateFromOBJ("skydome", true);
}

void Skydome::Update()
{
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	//3Dモデル描画
	model_->Draw(worldtransform_, viewProjection_);
}
