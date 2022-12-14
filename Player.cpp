#include "Player.h"
#include "Function.h"
#include <cassert>

/// <summary>
/// ������
/// �������烂�f���f�[�^�ƃe�N�X�`���f�[�^���󂯎��
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle){

	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
	//3D���e�B�N���̏�����
	worldtransform3DReticle_.Initialize();
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };

	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;
	const float kCharacterJumpSpeed = 0.6f;
	const float gravitySpeed = 0.15f;

	//��ɏd�͂𑫂��Ă�
	move.y -= gravitySpeed;
	//�v���C���[�̈ړ�
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	/*if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}*/
	//�W�����v
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
	
	//�L�[�{�[�h���͂ɂ��ړ�����
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 10;

	//�͈͂𒴂��Ȃ�����
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//�s��X�V
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	//���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z

	//���@����3D���e�B�N���ւ̋���
	/*const float kDistancePlayerTo3DReticle = 60.0f;*/
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	/*Vector3 offset = { 0, 0, 1.0f };*/
	//���@�̃��[���h���W�̉�]�𔽉f

}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

}
