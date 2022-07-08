#pragma once
#include <WorldTransform.h>
#include <Model.h>

class PlayerBullet {
public:
	///< summary>
	///������
	///</summary>
	///<param name="model"���f��</param>
	///<param name="postion"�������W</param>
	///<param name="velocity"���x</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///< summary>
	///������
	///</summary>
	void Update();

	///< summary>
	///������
	///</summary>
	void Draw(const ViewProjection& viewProjection);

public:

	bool isDead() const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//������frm��
	static const int32_t KLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = KLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

};