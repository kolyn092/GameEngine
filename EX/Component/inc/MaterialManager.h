#pragma once
#include "CMaterial.h"

///
///
///  �� �𵨿� �ִ� ��� ����� ���͸����� �����Ѵ�.
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
		std::vector<CMaterial*> m_ModelMaterialList;	// ��� ������ ���͸����� ��� �ִ�.

	public:
		MaterialManager();
		~MaterialManager();

	public:
		void SetMaterial(CMaterial* material);
		void CreateMaterialData(GRAPHICENGINE::IModelInterface* model);
		int GetMaterialCount() const { return m_ModelMaterialList.size(); }

	};
}

