#pragma once
class CRulerBar : public SWindow
{
	SOUI_CLASS_NAME(CRulerBar, L"rulerbar")
public:
	CRulerBar();
	~CRulerBar();

protected:
	void OnPaint(IRenderTarget* pRT);

protected:
	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
	SOUI_MSG_MAP_END()

	SOUI_ATTRS_BEGIN()
		ATTR_INT(L"bar_position", m_nBarPosition, FALSE)
	SOUI_ATTRS_END()

private:
	int     m_nBarPosition;

	int		m_nPix;
public:
	void SetPix2CM(int nPix);
};

