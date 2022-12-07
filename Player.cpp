#include "Player.h"
#include "Function.h"
#include <cassert>
#include <base/WinApp.cpp>

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

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("player.png");

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldtransform_.translation_.z = 50;
	worldtransform_.Initialize();
}

void Player::Update(ViewProjection viewProjection_)
{
	//�f�X�t���O�̗������e�̍폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->isDead();
		});


	Vector3 move = { 0,0,0 };

	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//�s��X�V
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.matWorld_ *= worldtransform_.parent_->matWorld_;
	worldtransform_.TransferMatrix();
	
	//�L�[�{�[�h���͂ɂ��ړ�����
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//�L�����N�^�[���񏈗�
	//�����������ňړ��x�N�g����ύX
	const float kCharacterRootSpeed = 0.02f;

	if (input_->PushKey(DIK_U)) {
		worldtransform_.rotation_.y -= kCharacterRootSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldtransform_.rotation_.y += kCharacterRootSpeed;
	}

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//�}�E�X�J�[�\��
	{
		POINT mousePosition;
		//�}�E�X���W(�X�N���[�����W)���擾����
		GetCursorPos(&mousePosition);

		//�N���C�A���g�G���A���W�ɕϊ�����
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//�}�E�X���W��2D���e�B�N���ɑ��
		sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));



		//�r���[�|�[�g�s��
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		// �r���[�v���W�F�N�V�����r���[�|�[�g����
		Matrix4 matVPV = viewProjection_.matView *= viewProjection_.matProjection *= Viewport;

		//�����s��̋t�s����v�Z����
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		//�X�N���[�����W			 
		Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
		Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

		//�X�N���[�����W�n���烏�[���h���W�n��
		posNear = Affin_->WDivision(matInverseVPV, posNear);
		posFar = Affin_->WDivision(matInverseVPV, posFar);

		//�}�E�X���C�̕���
		Vector3 mouseDirection = posFar -= posNear;

		float len = sqrt(mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y + mouseDirection.z * mouseDirection.z);
		if (len != 0)
		{
			mouseDirection /= len;
		}

		//�J��������Ə��I�u�W�F�N�g�̋���
		const float kDistanceTestObject = 180.0f;
		worldTransform3DReticle_.translation_ = posNear += (mouseDirection *= kDistanceTestObject);

		//�s��X�V
		worldtransform_.matWorld_ = CreateMatIdentity();
		worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
		worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
		worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
		worldtransform_.matWorld_ *= worldtransform_.parent_->matWorld_;
		worldtransform_.TransferMatrix();

	}

}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldtransform_);

		Vector3 position = GetWorldPosition();
		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position,velocity);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));
	}

}
Vector3  Player::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision(){}

float Player::GetRadius() { return radius_; }
