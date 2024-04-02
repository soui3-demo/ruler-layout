#pragma once
class CRulerBase
{
public:
	CRulerBase();
	virtual ~CRulerBase();

public:
	void SetDrawBorder(bool bDraw);
	void SetLeft(int& nLeft);
	int GetLeft();
	void SetTop(int& nTop);
	int GetTop();
	void SetRight(int& nRight);
	int GetRight();
	void SetBottom(int& nBottom);
	int	GetBottom();
	void SetWidth(int& nWidth);
	int GetWidth();
	void SetHeight(int& nHeight);
	int GetHeight();

protected:
	bool	m_bDrawBorder;
	int		m_nLeft;
	int		m_nTop;
	int		m_nRight;
	int		m_nBottom;
	int		m_nWidth;
	int		m_nHeight;
};

