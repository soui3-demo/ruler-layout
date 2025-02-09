// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"
using namespace SOUI;


#include "CGdiDc.h"
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include <sstream>
#include <string>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

enum EcPosType
{
	Null = -1,
	TopLeft,
	TopCenter,
	TopRight,
	RightCenter,
	BottomRight,
	BottomCenter,
	BottomLeft,
	LeftCenter,
	SelectRect,
};

enum RulerAddType
{
	RulerTypeNull = 0,
	RulerTypeRectangle,
	RulerTypeEllipse,
	RulerTypeBarCode,
	RulerTypeDrugCode,
	RulerTypeQRCode,
	RulerTypeText,
	RulerTypePicture,
	RulerTypePDF,
};