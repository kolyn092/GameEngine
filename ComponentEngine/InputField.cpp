#include "pch.h"
#include "InputField.h"
#include "Transform.h"
#include "Image.h"
#include "Input.h"
#include "CUtil.h"

using namespace DirectX::SimpleMath;

ComponentEngine::InputField::InputField()
	: Component("InputField"), m_ContentType(eContentType::Standard), m_AlignmentType(eAlignmentType::Left), m_Text(""), m_TextYCheck(""), m_TextSize(10.0f), m_TextPos(Vector2(0.f, 0.f)),
	m_TextColor(Vector4(1, 1, 1, 1)), m_CharacterLimit(0), m_CanInputText(false), m_ActiveState(true)
{
	m_TextSize = 20.0f;
	char _str = 'A';
	char _str1 = 'B';
	m_TextYCheck.push_back(_str);
	m_TextYCheck.push_back(_str1);
}

ComponentEngine::InputField::~InputField()
{

}

void ComponentEngine::InputField::CollisionUpdate()
{
	if (m_GameObject)
	{
		Transform* _trans = m_GameObject->GetComponent<Transform>();
		UI_Image* _image = m_GameObject->GetComponent<UI_Image>();
		if (_image)
		{
			// �簢�� ���� ���
			RECT rc = { static_cast<LONG>(_trans->GetPosition().x),
						static_cast<LONG>(_trans->GetPosition().y),
						static_cast<LONG>(_trans->GetPosition().x + _image->GetImageWidth()),
						static_cast<LONG>(_trans->GetPosition().y + _image->GetImageHeight()) };

			// ���콺 ��ġ
			POINT pt = { static_cast<LONG>(Input::Ins()->GetMousePos().x), static_cast<LONG>(Input::Ins()->GetMousePos().y) };

			// �浹 �˻�
			if (PtInRect(&rc, pt))
			{
				// Ȱ��ȭ
				if (Input::Ins()->GetMouseButtonDown(Input::eMouse::leftButton))
				{
					m_CanInputText = true;
				}
			}
			else
			{
				// �浹���� �ʴ� ���� Ŭ���ϸ� ��Ȱ��ȭ
				if (Input::Ins()->GetMouseButtonDown(Input::eMouse::leftButton))
				{
					m_CanInputText = false;
				}
			}
		}
	}
}

void ComponentEngine::InputField::UpdateTextInput()
{
	// text input Ȱ��ȭ ��
	if (m_CanInputText)
	{
		TCHAR* _ch = StringToTCHAR(m_Text);
		TCHAR* _ch1 = StringToTCHAR(m_TextYCheck);
		Vector2 _textSizeX = DrawText_Debug::GetInstance()->GetSpriteFontSize(1, _ch, m_TextSize);
		Vector2 _textSizeY = DrawText_Debug::GetInstance()->GetSpriteFontSize(1, _ch1, m_TextSize);

		if (_ch)
		{
			delete _ch;
			_ch = nullptr;
		}
		if (_ch1)
		{
			delete _ch;
			_ch1 = nullptr;
		}

		// ���� Ű�� ���ȴ��� Ȯ��
		if (Input::Ins()->GetKeyDown(Input::Ins()->GetNowKey()))
		{
			// �齺���̽��� ����, �̿��� �͵��� push_back
			// Ű�� Input::UpdateKeyState() ���� ���� �͸� ����.
			switch (Input::Ins()->GetNowKey())
			{
				case Keyboard::Keys::Back:
				{
					if (m_Text.size() > 0)
					{
						m_Text.pop_back();
					}
					SettingTextAlignment(Vector2(_textSizeX.x, _textSizeY.y));
				}
				break;
				case Keyboard::Keys::Space:
				{
					if (m_Text.size() < m_CharacterLimit || m_CharacterLimit == 0)
					{
						m_Text.push_back(' ');
					}
				}
				break;
				case Keyboard::Keys::Enter:
				{
					m_CanInputText = false;
				}
				break;
				default:
				{
					UI_Image* _image = m_GameObject->GetComponent<UI_Image>();
					if (_image)
					{
						const int whiteSpaceLength = 50;
						if (_image->GetImageWidth() >= _textSizeX.x + whiteSpaceLength)
						{
							bool _capslockState = GetKeyState(VK_CAPITAL);
							if (_capslockState)
							{
								// �빮��
								m_Text.push_back(Input::Ins()->GetNowKey());
							}
							else
							{
								// �ҹ���
								if (65 <= Input::Ins()->GetNowKey() && Input::Ins()->GetNowKey() <= 90)
								{
									m_Text.push_back(Input::Ins()->GetNowKey() + 32);
								}
								else
								{
									m_Text.push_back(Input::Ins()->GetNowKey());
								}
							}
						}

						SettingTextAlignment(Vector2(_textSizeX.x, _textSizeY.y));
					}
				}
				break;
			}
		}
	}
}

void ComponentEngine::InputField::SettingTextAlignment(Vector2 textLength)
{
	UI_Image* _image = m_GameObject->GetComponent<UI_Image>();

	if (_image)
	{
		// ��� ������ ���
		switch (m_AlignmentType)
		{
			case ComponentEngine::InputField::eAlignmentType::Left:
				break;
			case ComponentEngine::InputField::eAlignmentType::Center:
			{
				// inputfield �̹��� ũ���� �� - ���� �Էµ� �ؽ�Ʈ �̹��� ũ���� ��
				m_TextPos.x = (_image->GetImageWidth() * 0.5f) - (textLength.x * 0.5f);
				m_TextPos.y = (_image->GetImageHeight() * 0.5f) - (textLength.y * 0.5f);
			}
			break;
			case ComponentEngine::InputField::eAlignmentType::Right:
				break;
			default:
				break;
		}
	}
}

void ComponentEngine::InputField::Update()
{
	// input field �浹 ���� ������Ʈ
	CollisionUpdate();

	// �ؽ�Ʈ �Է� ���� ������Ʈ
	UpdateTextInput();
}

void ComponentEngine::InputField::OnGUI()
{
	if (m_GameObject)
	{
		Transform* _trans = m_GameObject->GetComponent<Transform>();
		Vector3 _pos = _trans->GetPosition();
		va_list vl;
		TCHAR* _ch = StringToTCHAR(m_Text);
		va_start(vl, _ch);
		DrawText_Debug::GetInstance()->DrawTextEx(1, static_cast<int>(_pos.x + m_TextPos.x), static_cast<int>(_pos.y + m_TextPos.y), m_TextColor, m_TextSize, _ch, vl);
		va_end(vl);
		delete _ch;
	}
}

void ComponentEngine::InputField::DebugTextRender()
{

}

void ComponentEngine::InputField::OnQuit()
{

}

void ComponentEngine::InputField::OnDisable()
{

}

void ComponentEngine::InputField::OnDestroy()
{

}

void ComponentEngine::InputField::SetActive(bool value)
{

}

void ComponentEngine::InputField::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_InputField = this;
	}
}
