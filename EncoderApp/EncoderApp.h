
// EncoderApp.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CEncoderAppApp:
// Сведения о реализации этого класса: EncoderApp.cpp
//

class CEncoderAppApp : public CWinApp
{
public:
	CEncoderAppApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CEncoderAppApp theApp;
