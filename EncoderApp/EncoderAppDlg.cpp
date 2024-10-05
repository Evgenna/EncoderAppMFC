
// EncoderAppDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "EncoderApp.h"
#include "EncoderAppDlg.h"
#include "EncoderController.h"
#include "EncoderModel.h"
#include "afxdialogex.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CEncoderAppDlg



CEncoderAppDlg::CEncoderAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENCODERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEncoderAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CIPHER_COMBOBOX, m_cipherCombobox);
	DDX_Control(pDX, IDC_LANGUAGESET_COMBOBOX, m_langCombobox);
	DDX_Control(pDX, IDC_RADIO2, m_encodeRadio);
	DDX_Control(pDX, IDC_RADIO3, m_decodeRadio);
	DDX_Control(pDX, IDC_INPUT_EDIT, m_inputMessage_edit);
	DDX_Control(pDX, IDC_KEY_EDIT, m_inputKey_edit);
	DDX_Control(pDX, IDC_OUTPUT_EDIT, m_outputMessage_edit);
	DDX_Control(pDX, IDC_BUTTON1, m_coderButton);
}

BEGIN_MESSAGE_MAP(CEncoderAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON1, &CEncoderAppDlg::OnProcessingButtonClicked)
	ON_CBN_SELCHANGE(IDC_CIPHER_COMBOBOX, &CEncoderAppDlg::OnSelchangeCipherCombobox)
	ON_CBN_SELCHANGE(IDC_LANGUAGESET_COMBOBOX, &CEncoderAppDlg::OnSelchangeLanguagesetCombobox)
END_MESSAGE_MAP()


// Обработчики сообщений CEncoderAppDlg

BOOL CEncoderAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	m_inputKey_edit.ModifyStyle(0, ES_NUMBER);

	SetToolTip();
	
	SetCipherList(); // Инициализация списка шифров
	SetLanguageList(); // Инициализация списка языков
	m_encodeRadio.SetCheck(1); // Установка значеня 1 для CheckBox по умолчанию

	context = std::make_unique<EncoderContext>(); // Инициализация указателя на контекст модуля шифратора

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CEncoderAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CEncoderAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CEncoderAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEncoderAppDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CDialogEx::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 735;

	lpMMI->ptMaxTrackSize.x = 1200;
	lpMMI->ptMaxTrackSize.y = 1000;
}

// Обработчик нажатия кнопки для шифрования/дешифрования.
// Выполняет проверку заполнения полей и вызывает метод контроллера для обработки.
void CEncoderAppDlg::OnProcessingButtonClicked()
{
	try {
		IsFieldNotEmpty(); // Проверяет заполнение всех необходимых полей

		// Инициализация контроллера и вызов функции обработки
		EncoderController controller(context.get(), this);
		controller.OnEncodeButtonClicked();
	}
	catch (std::exception ex) {
		AfxMessageBox((CString)(ex.what())); // Выводит сообщение об ошибке
		return;
	}
}

// Добавляет доступные шифры в выпадающий список m_cipherCombobox.
// Устанавливает первый шифр (по умолчанию "Шифр Цезаря").
void CEncoderAppDlg::SetCipherList()
{
	m_cipherCombobox.AddString(_T("Шифр Цезаря"));
	m_cipherCombobox.AddString(_T("Шифр Виженера"));
	m_cipherCombobox.AddString(_T("Шифр Вернама"));
	m_cipherCombobox.SetCurSel(0);
}

// Добавляет доступные языки в выпадающий список m_languageCombobox.
// Устанавливает первый язык (по умолчанию "Русский язык").
void CEncoderAppDlg::SetLanguageList()
{
	m_langCombobox.AddString(_T("Английский язык"));
	m_langCombobox.AddString(_T("Русский язык"));
	m_langCombobox.SetCurSel(1);
}

// Проверка на то, что все необходимые поля заполнены.
bool CEncoderAppDlg::IsFieldNotEmpty()
{
	CString message, key;
	m_inputMessage_edit.GetWindowText(message);
	if (message.IsEmpty()) {
		throw std::invalid_argument("Поле с сообщением не должно быть пустым");
	}
	m_inputKey_edit.GetWindowText(key);
	if (key.IsEmpty()) {
		throw std::invalid_argument("Поле с ключем не должно быть пустым");
		return false;
	}
	return true;
}

// Возвращает введенное пользователем сообщение из текстового поля.
const std::string CEncoderAppDlg::GetInputMessage()
{
	CString inputMessage;
	m_inputMessage_edit.GetWindowText(inputMessage);
	return std::string(CT2A(inputMessage));
}

// Возвращает введенный пользователем ключ из текстового поля.
const std::string CEncoderAppDlg::GetInputKey()
{
	CString inputKey;
	m_inputKey_edit.GetWindowText(inputKey);
	return std::string(CT2A(inputKey));
}

// Возвращает выбранный пользователем язык из выпадающего списка.
std::string CEncoderAppDlg::GetSelectedLanguage()
{
	CString selectedLanguage;
	m_langCombobox.GetWindowText(selectedLanguage);
	return std::string(CT2A(selectedLanguage));
}

// Возвращает выбранный пользователем шифр из выпадающего списка.
const std::string CEncoderAppDlg::GetSelectedCipher()
{
	CString selectedCipher;
	m_cipherCombobox.GetWindowText(selectedCipher);
	return std::string(CT2A(selectedCipher));
}

// Проверяет, выбрано ли действие "Зашифровать" (RadioButton для кодирования).
const bool CEncoderAppDlg::IsEncoderSelected()
{
	return m_encodeRadio.GetCheck() == BST_CHECKED;
}

// Проверяет, выбрано ли действие "Расшифровать" (RadioButton для раскодирования).
const bool CEncoderAppDlg::IsDecoderSelected()
{
	return m_decodeRadio.GetCheck() == BST_CHECKED;
}

// Устанавливает результат шифрования/дешифрования в текстовое поле вывода.
void CEncoderAppDlg::SetOutputMessage(const std::string& outputMessage)
{
	m_outputMessage_edit.SetWindowText(CString(outputMessage.c_str()));
}

void CEncoderAppDlg::SetToolTip() {
	m_toolTip.Create(this);

	m_toolTip.AddTool(GetDlgItem(IDC_INPUT_EDIT), _T("Введите текст сообщения, которое хотите зашифровать (На русском)"));
	m_toolTip.AddTool(GetDlgItem(IDC_KEY_EDIT), _T("Введите ключ для шифрования (Число)"));
	m_toolTip.AddTool(GetDlgItem(IDC_OUTPUT_EDIT), _T("Здесь будет зашифрованное сообщение"));

	m_toolTip.Activate(TRUE);

	m_toolTip.SetDelayTime(TTDT_INITIAL, 1000);
	m_toolTip.SetDelayTime(TTDT_AUTOPOP, 10000);
}

BOOL CEncoderAppDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_toolTip.m_hWnd != NULL)
	{
		m_toolTip.RelayEvent(pMsg); // Передача событий подсказкам
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// Меняет подсказки для поля ввода с ключем в зависимости от выбора алгоритма шифрования.
void CEncoderAppDlg::OnSelchangeCipherCombobox()
{
	int nSel = m_cipherCombobox.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strSelectedText;
		m_cipherCombobox.GetLBText(nSel, strSelectedText);

		if (strSelectedText == "Шифр Цезаря") {
			m_toolTip.UpdateTipText(_T("Введите ключ для шифрования (Число)"), GetDlgItem(IDC_KEY_EDIT));
			m_inputKey_edit.SetWindowText(_T(""));
			m_inputKey_edit.ModifyStyle(0, ES_NUMBER);
			return;
		}
		if (strSelectedText == "Шифр Виженера") {
			m_toolTip.UpdateTipText(_T("Введите ключ для шифрования (Текст)"), GetDlgItem(IDC_KEY_EDIT));
			m_inputKey_edit.SetWindowText(_T(""));
			m_inputKey_edit.ModifyStyle(ES_NUMBER, 0);
			return;
		}
		if (strSelectedText == "Шифр Вернама") {
			m_toolTip.UpdateTipText(_T("Введите ключ для шифрования (Текст)"), GetDlgItem(IDC_KEY_EDIT));
			m_inputKey_edit.SetWindowText(_T(""));
			m_inputKey_edit.ModifyStyle(ES_NUMBER, 0);
			return;
		}
	}
}


void CEncoderAppDlg::OnSelchangeLanguagesetCombobox()
{
	int nSel = m_langCombobox.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strSelectedText;
		m_langCombobox.GetLBText(nSel, strSelectedText);

		if (strSelectedText == "Русский язык") {
			m_toolTip.UpdateTipText(_T("Введите текст сообщения, которое хотите зашифровать (На русском)"), GetDlgItem(IDC_INPUT_EDIT));
			m_inputKey_edit.SetWindowText(_T(""));
			m_inputKey_edit.ModifyStyle(0, ES_NUMBER);
			return;
		}
		if (strSelectedText == "Английский язык") {
			m_toolTip.UpdateTipText(_T("Введите текст сообщения, которое хотите зашифровать (На английском)"), GetDlgItem(IDC_INPUT_EDIT));
			m_inputMessage_edit.SetWindowText(_T(""));
			m_inputMessage_edit.ModifyStyle(ES_NUMBER, 0);
			return;
		}
	}
}
