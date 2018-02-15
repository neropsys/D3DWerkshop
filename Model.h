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
	//한 버퍼에 다 때려박아서 버퍼 스위칭 줄이는것도 고려해야함.
	//일단 모델별로 버퍼 하나씩 할당
	//InitResources에 있는 중복 코드(버퍼 초기화, 버퍼 desc지정 등) 옮기도록
	ID3D11Buffer* mVBuffer;
	ID3D11Buffer* mIBuffer;
	ID3D11RasterizerState* mWireframe;
};

