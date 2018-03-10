#pragma once
#include <DirectXMath.h>
class IRender {
public:
	virtual ~IRender() {};
	virtual void SetViewProj(const DirectX::XMMATRIX& ref) abstract;
	virtual void Update(float delta) const abstract;
	virtual void Draw() abstract;
};

class IRenderWireFrame {
	virtual void SetWireFrame(bool setWireframe) abstract;
};