#include "Enemy.h"
#include <cassert>
#include <Function.h>


void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("raiden.jpg");
	//���[���h�ϊ��̏�����
	worldtransform_.Initialize();
}

void Enemy::Update()
{
	

	Vector3 move = { 0,0,0 };

	//�G�̈ړ��̑���
	const float enemySpeed = 0.03f;
	//�G�̈ړ�����
	move = { 0.0f, 0.0f,enemySpeed };
	/*worldtransform_.translation_ += move;*/

	//�s��X�V
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
	    //�ړ��i�x�N�g�������Z�j
	    worldtransform_.translation_ -= move;
	    //�K��̈ʒu�ɓ��B�����痣�E
	    if (worldtransform_.translation_.z < -10.0f) {
	        phase_ = Enemy::Phase::Leave;
	    }
	    break;

	case Enemy::Phase::Leave:
	    //�ړ��i�׃N�g�������Z�j
		worldtransform_.translation_ += {0.02,0.02,0};
	    break;

	}
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Attack() {

	if (input_->PushKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldtransform_);

		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_, velocity);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));

	}
}

void Enemy::Fire()
{
}

