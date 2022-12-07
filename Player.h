#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"

/// <summary>
/// ���L����

/// </summary>

class Player {
public:
	/// <summary>
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//3D���e�B�N���p�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;
	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite>sprite2DReticle_;
};