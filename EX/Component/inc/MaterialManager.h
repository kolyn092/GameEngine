#pragma once
#include "CMaterial.h"

///
///
///  한 모델에 있는 모든 노드의 메터리얼을 관리한다.
///
///  [5/21/2021 KoSuKyung]

namespace ComponentEngine
{
	class MaterialManager
	{

	public:
		enum class eMaterial
		{
			None,
			Albedo,
			NormalMap,
			OcculsionMatalicRoughness,
			Emission,
			DetailMask,
			///HeightMap,
		};

	private:
		std::vector<CMaterial*> m_ModelMaterialList;	// 모든 노드들의 메터리얼을 들고 있다.

	public:
		MaterialManager();
		~MaterialManager();

	public:
		void SetMaterial(CMaterial* material);
		void CreateMaterialData(GRAPHICENGINE::IModelInterface* model);
		int GetMaterialCount() const { return m_ModelMaterialList.size(); }

	};
}

