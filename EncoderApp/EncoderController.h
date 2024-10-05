#include "EncoderModel.h"
#include "EncoderAppDlg.h"

#pragma once
class EncoderController
{
private:
	EncoderContext* context;
	CEncoderAppDlg* view;

	std::string GetLanguageId(std::string& language);
	void SetEncoderContext(std::string& cipher, std::string& language);
	
public:
	EncoderController(EncoderContext* ctx, CEncoderAppDlg* dlg);
	void OnEncodeButtonClicked();
};

