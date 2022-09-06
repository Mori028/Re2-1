#include "RailCamera.h"
#include <Function.h>

void RailCamera::Initialize(const Vector3 position, const Vector3 rotation)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldtransform_.translation_ = position;
	worldtransform_.rotation_ = rotation;
	
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldtransform_.Initialize();
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 200.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{

    //�ړ�
    worldtransform_.translation_ += Vector3(0.005f, 0, 0.005f);
    //���[���h�g�����X�t�H�[�����X�V
    worldtransform_.matWorld_ = CreateMatIdentity();
    worldtransform_.matWorld_ = CreateMatScale(worldtransform_.scale_);
    worldtransform_.matWorld_ = CreateMatRotationX(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatRotationY(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatRotationZ(worldtransform_.rotation_);
    worldtransform_.matWorld_ = CreateMatTranslation(worldtransform_.translation_);

    //���[���h�s��̕��s�ړ�����
    viewProjection_.eye.x = worldtransform_.matWorld_.m[3][0];
    viewProjection_.eye.y = worldtransform_.matWorld_.m[3][1];
    viewProjection_.eye.z = worldtransform_.matWorld_.m[3][2];

    //���[���h�O���x�N�g��
    Vector3 forward(0, 0, 1);
    //���[���J�����̉�]�𔽉f
    forward = bvector(forward, worldtransform_.matWorld_);

    //���_����O���ɓK���ȋ����i��
    forward += viewProjection_.eye;
    viewProjection_.target = forward;

    //���[���h���
    Vector3 up(0, 1, 0);

    //���[���J�����̉�]
    viewProjection_.up = bvector(up, worldtransform_.matWorld_);

    //�r���[�v���W�F�N�V�������X�V
    viewProjection_.UpdateMatrix();

    /*debugText_->SetPos(50, 130);
    debugText_->Printf(
        "eye:(%f,%f,%f)",
        viewProjection_.eye.x,
        viewProjection_.eye.y,
        viewProjection_.eye.z);*/

    if (input_->PushKey(DIK_F))
    {
        worldtransform_.rotation_.y -= 0.1f;
    }
}