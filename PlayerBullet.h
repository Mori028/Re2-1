#pragma once
#include <WorldTransform.h>
#include <Model.h>

class PlayerBullet {
public:
	///< summary>
	///������
	///</summary>
	void Initialize(Model* model, const Vector3& position);

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

};