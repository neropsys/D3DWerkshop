#pragma once
#include "ModelLoader.h"
class Model
{
public:
	Model(const char* fileName);
	Model();
	~Model();
	void Update(float delta) const;
	void Draw() const;
	inline void SetPos(XMFLOAT3 vec3) { m_pos = vec3; };
	inline void SetRot(XMFLOAT3 vec3) { m_rot = vec3; };
	inline void SetScale(XMFLOAT3 vec3) { m_scale = vec3; };
protected:
	UINT mstride;
	UINT moffset;
	UINT mindexCount;
	ModelLoader mloader;
	bool mSetwireframe;;
	//�� ���ۿ� �� �����ھƼ� ���� ����Ī ���̴°͵� ����ؾ���.
	//�ϴ� �𵨺��� ���� �ϳ��� �Ҵ�
	//InitResources�� �ִ� �ߺ� �ڵ�(���� �ʱ�ȭ, ���� desc���� ��) �ű⵵��
	ID3D11Buffer* mVBuffer;
	ID3D11Buffer* mIBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11RasterizerState* mWireframe;

	XMFLOAT3 m_rot;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_scale;
	XMMATRIX m_prs;
};

