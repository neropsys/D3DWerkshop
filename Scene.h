#pragma once
#include <vector>
#include <memory>
#include "Model.h"
#include "Camera.h"
class Scene {

public:
	Scene();
	void Init();
	void Update(float delta);
	void Draw();
protected:
	std::vector<Model> m_models;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_constantBuffer;
	ID3D11InputLayout* m_inputLayout;
	std::unique_ptr<Camera> g_cam;
};