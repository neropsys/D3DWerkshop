#pragma once
#include <vector>
#include <memory>
#include "Camera.h"
#include "IRender.h"
#include <memory>
#include "Model.h"
class Scene {

public:
	Scene();
	~Scene();
	void Init();
	void Update(float delta);
	void Draw();
protected:
	std::unique_ptr<Model> m_model;
	std::vector<std::unique_ptr<IRender>> m_models;
	ID3D11VertexShader* m_vertexShader;
	ID3D11VertexShader* m_brdfVertexShader;
	ID3D11PixelShader* m_brdfPixelShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_constantBuffer;
	ID3D11InputLayout* m_inputLayout;
	ID3D11InputLayout* m_standardInputLayout;
	std::unique_ptr<Camera> g_cam;
};