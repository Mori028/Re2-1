#pragma once
#include <3d/Model.h>
#include "DebugText.h"
#include "Input.h"
#include <EnemyBullet.h>

/// <summary>
/// ���L����
/// </summary>

class Enemy {
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

	void Attack();

	void Fire();

	//�s���t�F�[�Y
	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
	};

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
};