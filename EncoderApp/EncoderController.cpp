#include "pch.h"
#include "EncoderController.h"
#include "afxwin.h"
#include <iostream>
#include <string>
#include <stdexcept>

EncoderController::EncoderController(EncoderContext* ctx, CEncoderAppDlg* dlg) : context(ctx), view(dlg) {}

std::string EncoderController::GetLanguageId(std::string& language)
{
    if (language == "Русский язык") return "ru";
    if (language == "Английский язык") return "en";
    throw std::invalid_argument("Выбран неподдерживаемый язык");
}

void EncoderController::SetEncoderContext(std::string& cipher, std::string& language)
{
    EncoderFactory factory;
    std::string languageId = GetLanguageId(language);
    if (cipher == "Шифр Цезаря") {
        int inputKey;
        try {
            inputKey = stoi(view->GetInputKey());
        }
        catch (std::exception ex) {
            throw std::invalid_argument("Ключ должен быть целым числом");
        }
        context->setEncoder(factory.createCaesarEncoder(inputKey, languageId));
        return;
    }
    if (cipher == "Шифр Вернама") {
        std::string inputKey = view->GetInputKey();
        context->setEncoder(factory.createVernamEncoder(inputKey, languageId));
        return;
    }
    if (cipher == "Шифр Виженера") {
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

