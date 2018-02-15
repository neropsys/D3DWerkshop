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
	ID3D11RasterizerState* mWireframe;
};

