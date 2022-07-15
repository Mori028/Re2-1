#pragma once
#include <WorldTransform.h>
#include <Model.h>

class EnemyBullet {
public:
	///< summary>
	///������
	///</summary>
	///<param name="model"���f��</param>
	///<param name="postion"�������W</param>
	///<param name="velocity"���x</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///< summary>
	///������
	///</summary>
	void Update();

	///< summary>
	///������
	///</summary>
	void Draw(const ViewProjection& viewProjection);


private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
};