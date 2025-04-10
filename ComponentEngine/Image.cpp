#include "pch.h"
#include "Image.h"
#include "Transform.h"
#include "CUtil.h"
#include "Button.h"
#include "CDebug.h"

using namespace DirectX::SimpleMath;

ComponentEngine::UI_Image::UI_Image()
	: Component("UI_Image"), m_FillAmount(1.0f), m_UI(nullptr), m_ImagePath(""), m_Color(Vector4::One), m_Material(nullptr),
	m_ImageType(eImageType::Simple), m_FillMethod(eFillMethod::Horizontal), m_FillOrigin(eFillOrigin::Left),
	m_ActiveState(true)
{

}

ComponentEngine::UI_Image::~UI_Image()
{
	if (m_UI)
	{
		delete m_UI;
		m_UI = nullptr;
	}
}

void ComponentEngine::UI_Image::SetImagePath(std::string path)
{
	m_ImagePath = path;

	Vector3 _pos = Vector3::Zero;
	_pos.z = 1;

	// TODO : rot는 추후 업데이트

	Vector3 _scale = Vector3::One;

	if (m_UI)
	{
		delete m_UI;
		m_UI = nullptr;
	}
	GRAPHICENGINE::SetUiObject(&m_UI, _pos, _scale, m_ImagePath);
}

DWORD ComponentEngine::UI_Image::GetImageWidth()
{
	return m_UI->GetTextureWidth();
}

DWORD ComponentEngine::UI_Image::GetImageHeight()
{
	return m_UI->GetTextureHeight();
}

void ComponentEngine::UI_Image::OffUI()
{
	m_UI->OffUi();
}

void ComponentEngine::UI_Image::OnUI()
{
	m_UI->OnUi();
}

void ComponentEngine::UI_Image::Awake()
{
	Transform* _trans = m_GameObject->GetComponent<Transform>();

	Vector3 _pos = Vector3::Zero;
	_pos.x = _trans->GetPosition().x;
	_pos.y = _trans->GetPosition().y;
	_pos.z = 1;

	// TODO : rot는 추후 업데이트

	Vector3 _scale = _trans->GetScale();

	/// TODO : 렌더큐에 넣고 초기화하는 순서때문에 여기 또는 다른곳에서 만들어줘야함.
	/*
	if (m_UI)
	{
		delete m_UI;
		m_UI = nullptr;
	}
	GRAPHICENGINE::SetUiObject(&m_UI, _pos, _scale, m_ImagePath);
	*/
	//////////////////////////////////////////////////////////////////////////

	m_UI->UpdateUI(_pos, _scale);

	UI_Button* _button = m_GameObject->GetComponent<UI_Button>();
	if (_button)
	{
		m_UI->SetOverlayColorOnUI(_button->GetNowStateColor());
	}
	else
	{
		m_UI->SetOverlayColorOnUI(m_Color);
	}
}

void ComponentEngine::UI_Image::OnEnable()
{

}

void ComponentEngine::UI_Image::Start()
{

}

void ComponentEngine::UI_Image::Update()
{
	if (m_UI == nullptr)
		return;

	//if (m_GameObject->isActive())
	//{
	//	m_UI->OnUi();
	//}
	//else
	//{
	//	m_UI->OffUi();
	//}

	Transform* _trans = m_GameObject->GetComponent<Transform>();

	Vector3 _pos = Vector3::Zero;
	_pos.x = _trans->GetPosition().x;
	_pos.y = _trans->GetPosition().y;
	_pos.z = 1;

	// TODO : rot는 추후 업데이트

	Vector3 _scale = _trans->GetScale();

	m_UI->UpdateUI(_pos, _scale);

	UI_Button* _button = m_GameObject->GetComponent<UI_Button>();
	if (_button)
	{
		if (_button->IsStateChanged())
		{
			m_UI->SetOverlayColorOnUI(_button->GetNowStateColor());
		}
	}
	else
	{
		m_UI->SetOverlayColorOnUI(m_Color);
	}

	switch (m_FillMethod)
	{
		case ComponentEngine::UI_Image::eFillMethod::Horizontal:
			m_UI->SetUISize(m_FillAmount, m_DefaultAmount);
			break;
		case ComponentEngine::UI_Image::eFillMethod::Vertical:
			m_UI->SetUISize(m_DefaultAmount, m_FillAmount);
			break;
		default:
			break;
	}

}

void ComponentEngine::UI_Image::OnGUI()
{
	/// TODO : 렌더큐에 넣고 초기화하는 순서때문에 여기 또는 다른곳에서 만들어줘야함.
	/// UI 렌더큐 적용 후 DrawUI 비활성화
	m_UI->DrawUI();
}

void ComponentEngine::UI_Image::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"UI_Image");
}

void ComponentEngine::UI_Image::OnQuit()
{

}

void ComponentEngine::UI_Image::OnDisable()
{

}

void ComponentEngine::UI_Image::OnDestroy()
{
	if (m_Material)
	{
		m_Material = nullptr;
	}

	if (m_UI)
	{
		delete m_UI;
		m_UI = nullptr;
	}
}

void ComponentEngine::UI_Image::SetActive(bool value)
{
	if (m_ActiveState != value)
	{
		if (value == true)
		{
			OnUI();
			Start();
		}
		else
		{
			OffUI();
		}
	}
	m_ActiveState = value;
}

void ComponentEngine::UI_Image::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_Image = this;
	}
}

