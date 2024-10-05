
// EncoderAppDlg.h: файл заголовка
#include <iostream>
#include "EncoderModel.h"

#pragma once


// Диалоговое окно CEncoderAppDlg
class CEncoderAppDlg : public CDialogEx
{
// Создание
public:
	CEncoderAppDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENCODERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cipherCombobox;
	CComboBox m_langCombobox;
	CButton m_encodeRadio;
	CButton m_decodeRadio;
	CEdit m_inputMessage_edit;
	CEdit m_inputKey_edit;
	CEdit m_outputMessage_edit;
	CButton m_coderButton;

	afx_msg void OnProcessingButtonClicked();

	const std::string GetInputMessage();
	const std::string GetInputKey();
	std::string GetSelectedLanguage();
	const std::string GetSelectedCipher();

	const bool IsEncoderSelected();
	const bool IsDecoderSelected();

	void SetOutputMessage(const std::string& outputMessage);

	std::unique_ptr<EncoderContext> context;

private:
	void SetCipherList();
	void SetLanguageList();

	bool IsFieldNotEmpty();
};
