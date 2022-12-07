#include "Player.h"
#include "Function.h"
#include <cassert>
#include <base/WinApp.h>
#include "PrimitiveDrawer.h"

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
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };

	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		move.y += kCharacterSpeed;

	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}
	
	//�L�[�{�[�h���͂ɂ��ړ�����
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

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

		//�X�N���[�����W			  ���}�E�X���R���g���[���[��
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
		worldtransform_.TransferMatrix();


	}
}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

}
