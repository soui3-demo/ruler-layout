#include "stdafx.h"
#include "CRulerBase.h"

CRulerBase::CRulerBase()
{
	//
}

CRulerBase::~CRulerBase()
{
	//
}

void CRulerBase::SetDrawBorder(bool bDraw)
{
	m_bDrawBorder = bDraw;
}

void CRulerBase::SetLeft(int& nLeft)
{
	m_nLeft = nLeft;
}

int CRulerBase::GetLeft()
{
	return m_nLeft;
}

void CRulerBase::SetTop(int& nTop)
{
	m_nTop = nTop;
}

int CRulerBase::GetTop()
{
	return m_nTop;
}

void CRulerBase::SetRight(int& nRight)
{
	m_nRight = nRight;
}

int CRulerBase::GetRight()
{
	return m_nRight;
}

void CRulerBase::SetBottom(int& nBottom)
{
	m_nBottom = nBottom;
}

int CRulerBase::GetBottom()
{
	return m_nBottom;
}

void CRulerBase::SetWidth(int& nWidth)
{
	m_nWidth = nWidth;
}

int CRulerBase::GetWidth()
{
	return m_nWidth;
}

void CRulerBase::SetHeight(int& nHeight)
{
	m_nHeight = nHeight;
}

int CRulerBase::GetHeight()
{
	return m_nHeight;
}