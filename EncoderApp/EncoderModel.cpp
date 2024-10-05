#include "pch.h"
#include "EncoderModel.h"

// Кодирование сообщения с помощью Шифра Цезаря. 
std::string CaesarEncoder::encode(const std::string& message) const
{
	return transform(message, _shift);
}

// Расшифровка сообщения с помощью Шифра Цезаря.
std::string CaesarEncoder::decode(const std::string& message) const
{
	return transform(message, -_shift);
}

// Преобразование сообщения с использованием сдвига (Кодирование и декодирование).
std::string CaesarEncoder::transform(const std::string& message, int shift) const
{
	std::string encodedMessage;
	for (char ch : message) {
		if (_alphabet.find(ch) != std::string::npos) {
			size_t index = _alphabet.find(ch);
			index = (index + shift + _alphabet.size()) % _alphabet.size();
			encodedMessage += _alphabet[index];
		}
		else {
			encodedMessage += ch;
		}
	}
	return encodedMessage;
}

// Кодирование сообщения с использованием Шифра Виженера.
std::string VigenereEncoder::encode(const std::string& message) const
{
	return transform(message, true);
}

// Расшифровка сообщения с использованием Шифра Виженера.
std::string VigenereEncoder::decode(const std::string& message) const
{
	return transform(message, false);
}

// Преобразование сообщения с помощью Шифра Виженера.
std::string VigenereEncoder::transform(const std::string& message, bool encrypt) const
{
	std::string result;
	size_t _key_length = _key.length();
	for (size_t i = 0; i < message.length(); ++i) {
		char c = message[i];
		auto it_c = _alphabet.find(c);
		if (it_c != std::string::npos) {
			auto it_k = _alphabet.find(_key[i % _key_length]);

			
			int shift = encrypt ? static_cast<int>(it_k) : -static_cast<int>(it_k);

			
			size_t pos = (it_c + shift + _alphabet.size()) % _alphabet.size();
			result += _alphabet[pos];
		}
		else {
			result += c;
		}
	}
	return result;
}

// Кодирование сообщения с помощью шифра Вернама.
std::string VernamEncoder::encode(const std::string& message) const
{
	return transform(message, true);
}

// Расшифровка сообщения с помощью шифра Вернама.
std::string VernamEncoder::decode(const std::string& message) const
{
	return transform(message, false);
}

// Преобразование сообщения с помощью шифра Вернама.
std::string VernamEncoder::transform(const std::string& message, bool encrypt) const
{
	std::string result;
	size_t alphabetSize = _alphabet.size();

	for (size_t i = 0; i < message.length(); ++i) {
		size_t textIndex = _alphabet.find(message[i]);
		size_t keyIndex = _alphabet.find(_key[i % _key.length()]);

		size_t cipherIndex = (textIndex ^ keyIndex) % alphabetSize;

		result += _alphabet[cipherIndex];
	}

	return result;
}

// Устанавливает текущий шифратор в контексте.
void EncoderContext::setEncoder(std::unique_ptr<IEncoder> encoder)
{
	_encoder = std::move(encoder);
}

// Шифрует сообщение с помощью текущего шифратора в контексте.
std::string EncoderContext::encodeMessage(const std::string& message)
{
	if (_encoder) {
		return _encoder->encode(message);
	}
	throw std::runtime_error("Шифратор не установлен");
}

// Расшифровывает сообщение с помощью текущего шифратора в контексте.
std::string EncoderContext::decodeMessage(const std::string& message)
{
	if (_encoder) {
		return _encoder->decode(message);
	}
	throw std::runtime_error("Шифратор не установлен");
}

// Создает шифратор Цезаря с указанным сдвигом и алфавитом.
std::unique_ptr<IEncoder> EncoderFactory::createCaesarEncoder(int shift, const std::string& language)
{
	return std::make_unique<CaesarEncoder>(shift, GetAlphabet(language));
}

// Создает шифратор Виженера с указанным ключом и алфавитом.
std::unique_ptr<IEncoder> EncoderFactory::createVigenereEncoder(const std::string& key, const std::string& language)
{
	return std::make_unique<VigenereEncoder>(key, GetAlphabet(language));
}

// Создает шифратор Вернама с указанным ключом и алфавитом.
std::unique_ptr<IEncoder> EncoderFactory::createVernamEncoder(const std::string& key, const std::string& language)
{
	
	return std::make_unique<VernamEncoder>(key, GetAlphabet(language));
}

// Возвращает алфавит в зависимости от выбранного языка.
std::string EncoderFactory::GetAlphabet(const std::string& language)
{
	std::string alphabet;
	if (language == "en") {
		alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		return alphabet;
	}
	else if (language == "ru") {
		alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; 
		return alphabet;
	}	
	throw std::invalid_argument("Неподдерживаемый язык");
}
