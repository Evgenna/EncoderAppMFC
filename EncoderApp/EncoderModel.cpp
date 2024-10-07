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
	size_t mid = _alphabet.length() / 2;
	std::string lowerAlphabet = _alphabet.substr(0, mid);
	std::string upperAlphabet = _alphabet.substr(mid);
	std::string encodedMessage;
	for (char ch : message) {
		if (lowerAlphabet.find(ch) != std::string::npos) { // Проверяем, является ли символ буквой (нижний регистр)
			size_t index = (lowerAlphabet.find(ch) + shift + lowerAlphabet.size()) % lowerAlphabet.size();
			encodedMessage += lowerAlphabet[index];
		}
		else if (upperAlphabet.find(ch) != std::string::npos) { // Проверяем, является ли символ буквой (верхний регистр)
			size_t index = (upperAlphabet.find(ch) + shift + upperAlphabet.size()) % upperAlphabet.size();
			encodedMessage += upperAlphabet[index];
		}
		else if (isdigit(ch)) { // Шифрование цифр
			encodedMessage += char((ch - '0' + shift + 10) % 10 + '0');
		}
		else {
			encodedMessage += ch; // Специальные символы остаются неизменными
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
	size_t keyLength = _key.length();
	size_t keyIndex = 0;

	size_t mid = _alphabet.length() / 2;
	std::string lowerAlphabet = _alphabet.substr(0, mid);
	std::string upperAlphabet = _alphabet.substr(mid);

	for (char c : message) {
		// Определяем, к какому алфавиту принадлежит символ
		std::string alphabet;
		if (lowerAlphabet.find(c) != std::string::npos) {
			alphabet = lowerAlphabet;
		}
		else if (upperAlphabet.find(c) != std::string::npos) {
			alphabet = upperAlphabet;
		}
		else {
			// Небуквенные символы остаются неизменными
			result += c;
			continue;
		}

		// Определяем сдвиг на основе ключа
		char keyChar = _key[keyIndex % keyLength];
		int shift = alphabet.find(tolower(keyChar)); // Получаем сдвиг по ключу

		// Шифруем или расшифровываем в зависимости от параметра
		if (!encrypt) {
			shift = -shift;
		}

		// Применяем сдвиг с учетом границ алфавита
		size_t pos = (alphabet.find(c) + shift + alphabet.size()) % alphabet.size();
		result += alphabet[pos];

		// Увеличиваем индекс ключа только для букв
		keyIndex++;
	}
	return result;
}

VernamEncoder::VernamEncoder(const std::string& key, const std::string& alphabet)
{
	_key = key;
	_alphabet = alphabet;
	for (char& c : _key) {
		c = tolower(c);
	}
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
