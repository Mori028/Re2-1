#include "Skydome.h"
#include <Function.h>

void Skydome::Initialize()
{
	worldtransform_.Initialize();
	//�X�P�[�����O�̐ݒ�
	worldtransform_.scale_ = { 250.0f, 250.0f, 250.0f };
	//�X�P�[�����O�s��
	worldtransform_.matWorld_ = CreateMatScale(worldtransform_.scale_);
	//�s��̓]��
	worldtransform_.TransferMatrix();

	model_ = Model::CreateFromOBJ("skydome", true);
}

void Skydome::Update()
{
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	//3D���f���`��
	model_->Draw(worldtransform_, viewProjection_);
}
