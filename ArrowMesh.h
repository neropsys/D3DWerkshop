#pragma once
#include <vector>
#include "Vertex.h"
class ArrowMesh {
public:
	enum DIRECTION {
		X,
		Y,
		Z
	};
	ArrowMesh(DIRECTION dir);
	inline std::vector<Vertex>& GetVertice() { return m_vertice; }
	inline std::vector<UINT>& GetIndice() { return m_indice; }
protected:
	ArrowMesh();
	std::vector<Vertex> m_vertice;
	std::vector<UINT> m_indice;
	DIRECTION m_dir;
	DirectX::XMFLOAT3 m_color;

};