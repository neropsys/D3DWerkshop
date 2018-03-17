#pragma once
#include "ModelLoader.h"
#include "IRender.h"
class Model : public IRender, public IRenderWireFrame
{
public:
	Model(const char* fileName);
	Model();
	~Model();
	void Update(float delta) const override;
	void Draw() override;
	inline void SetPos(XMFLOAT3 vec3) { m_pos = vec3; };
	inline void SetRot(XMFLOAT3 vec3) { m_rot = vec3; };
	inline void SetScale(XMFLOAT3 vec3) { m_scale = vec3; };
	inline void SetWireFrame(bool setWireframe) override{ m_setwireframe = setWireframe; }


	virtual void SetViewProj(const DirectX::XMMATRIX& ref) override;


	virtual void PreRenderState(std::function<void()> preState) override;

	//virtual inline  void SetViewProj(const XMMATRIX& viewProj) { m_viewProj = viewProj; };
protected:
	std::function<void()> m_preState;
	UINT m_stride;
	UINT m_offset;
	UINT m_indexCount;
	ModelLoader mloader;
	bool m_setwireframe;
	//한 버퍼에 다 때려박아서 버퍼 스위칭 줄이는것도 고려해야함.
	//일단 모델별로 버퍼 하나씩 할당
	//InitResources에 있는 중복 코드(버퍼 초기화, 버퍼 desc지정 등) 옮기도록
	ID3D11Buffer* m_VBuffer;
	ID3D11Buffer* m_IBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11RasterizerState* m_wireframe;
	ID3D11SamplerState* m_samplerState;

	XMFLOAT3 m_rot;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_scale;
	XMMATRIX m_world = XMMatrixIdentity();
	XMMATRIX m_viewProj = XMMatrixIdentity();
};

