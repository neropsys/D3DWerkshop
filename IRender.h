#pragma once
#include <DirectXMath.h>
#include <functional>
class IRender {
public:
	virtual ~IRender() {};
	virtual void PreRenderState(std::function<void()> preState) abstract;
	virtual void SetViewProj(const DirectX::XMMATRIX& ref) abstract;
	virtual void Update(float delta) const abstract;
	virtual void Draw() abstract;
};

class IRenderWireFrame {
	virtual void SetWireFrame(bool setWireframe) abstract;
};