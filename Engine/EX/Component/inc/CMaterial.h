#pragma once
#include <map>
#include "EngineInterface.h"

namespace ComponentEngine
{
	class CMaterial
	{

	public:
		// material texture path
		//std::map<GRAPHICENGINE::IMaterialInterface::TextureSort, std::string> m_MaterialTexturePathList;
		
	public:
		std::string m_AlbedoPath;
		std::string m_NormalMapPath;
		std::string m_ORMPath;			// Occulsion, Roughness, Matalic
		std::string m_EmissionPath;
		std::string m_DetailMaskPath;
		///std::string m_HeightMapPath;

	private:
		GRAPHICENGINE::IMaterialInterface* m_Material;
		std::string m_NodeName;

		// maps option
		bool m_isEmission;
		DirectX::SimpleMath::Vector2 m_Tiling;
		DirectX::SimpleMath::Vector2 m_Offset;

	public:
		CMaterial();
		~CMaterial();

	public:
		void SetNodeName(std::string nodeName);
		void CreateMaterialData(GRAPHICENGINE::IModelInterface* model);
		std::string GetNodeName() const { return m_NodeName; }
	};
}

