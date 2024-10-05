#include "pch.h"
#include "EncoderController.h"
#include "afxwin.h"
#include <iostream>
#include <string>
#include <stdexcept>

EncoderController::EncoderController(EncoderContext* ctx, CEncoderAppDlg* dlg) : context(ctx), view(dlg) {}

std::string EncoderController::GetLanguageId(std::string& language)
{
    if (language == "������� ����") return "ru";
    if (language == "���������� ����") return "en";
    throw std::invalid_argument("������ ���������������� ����");
}

void EncoderController::SetEncoderContext(std::string& cipher, std::string& language)
{
    EncoderFactory factory;
    std::string languageId = GetLanguageId(language);
    if (cipher == "���� ������") {
        int inputKey;
        try {
            inputKey = stoi(view->GetInputKey());
        }
        catch (std::exception ex) {
            throw std::invalid_argument("���� ������ ���� ����� ������");
        }
        context->setEncoder(factory.createCaesarEncoder(inputKey, languageId));
        return;
    }
    if (cipher == "���� �������") {
        std::string inputKey = view->GetInputKey();
        context->setEncoder(factory.createVernamEncoder(inputKey, languageId));
        return;
    }
    if (cipher == "���� ��������") {
        std::string inputKey = view->GetInputKey();
        context->setEncoder(factory.createVigenereEncoder(inputKey, languageId));
        return;
    }
    
}

void EncoderController::OnEncodeButtonClicked()
{
    std::string inputMessage = view->GetInputMessage();
    std::string selectedCipher = view->GetSelectedCipher();
    std::string selectedLanguage = view->GetSelectedLanguage();

    SetEncoderContext(selectedCipher, selectedLanguage);

    std::string result;

    if (view->IsEncoderSelected()) result = context->encodeMessage(inputMessage);
    else if (view->IsDecoderSelected()) result = context->decodeMessage(inputMessage);

    view->SetOutputMessage(result);
}

