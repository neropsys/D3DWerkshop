#pragma once
#include "IRender.h"
#include "Vertex.h"
#include <vector>
class Cube : public IRender {

public:
	virtual void SetViewProj(const DirectX::XMMATRIX& ref) override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	virtual void Update(float delta) const override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	virtual void Draw() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

};