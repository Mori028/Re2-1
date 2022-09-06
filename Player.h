#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include <PlayerBullet.h>
#include <memory> 
#include <list>

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

	///< summary>
	///������
	///</summary>
	void Attack();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e�̃��X�g���擾
	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	float GetRadius();

	void SetParent(WorldTransform* worldTransform) {
		worldtransform_.parent_ = worldTransform;
	}
public:

	bool isDead() const { return isDead_; }
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	// �f�X�t���O
	bool isDead_ = false;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	
	const float radius_ = 1.0f;
};