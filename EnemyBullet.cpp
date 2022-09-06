#include "EnemyBullet.h"
#include "Function.h"
#include <assert.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("shot.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldtransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldtransform_.translation_ = position;
}

void EnemyBullet::Update() {
	
	// ���W���ړ�������i�P�t���[�����̈ړ��ʂ𑫂����ށj
	worldtransform_.translation_ += velocity_;
	//�s��X�V
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();
	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ���[���h�g�����X�t�H�[���̍X�V
	CreateMatrixUpdate(worldtransform_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldtransform_, viewProjection, textureHandle_);
}

Vector3  EnemyBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;

}

void EnemyBullet::OnCollision() {

	//�f�X�t���O
	isDead_ = true;
}

float EnemyBullet::GetRadius() { return radius_; }
