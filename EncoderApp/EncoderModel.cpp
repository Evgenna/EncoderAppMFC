#include "pch.h"
#include "EncoderModel.h"


// �������� ��������� � ������� ����� ������.
std::string CaesarEncoder::encode(const std::string& message) const
{
	return transform(message, _shift);
}

// �������������� ��������� � ������� ����� ������.
std::string CaesarEncoder::decode(const std::string& message) const
{
	return transform(message, -_shift);
}

// ����������� ��������� � �������������� ������ (�����������, �������������).
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

// �������� ��������� � ������� ����� ��������.
std::string VigenereEncoder::encode(const std::string& message) const
{
	return transform(message, true);
}

// �������������� ��������� � ������� ����� ��������.
std::string VigenereEncoder::decode(const std::string& message) const
{
	return transform(message, false);
}

// ����������� ��������� � ������� ����� ��������.
std::string VigenereEncoder::transform(const std::string& message, bool encrypt) const
{
	std::string result;
	size_t _key_length = _key.length();
	for (size_t i = 0; i < message.length(); ++i) {
		char c = message[i];
		auto it_c = _alphabet.find(c);
		if (it_c != std::string::npos) {
			// ������� ������ �����
			auto it_k = _alphabet.find(_key[i % _key_length]);

			// ���� �������, ��������� ��������, ���� �������������� � ��������
			// �������� it_k � int ��� ���������� ������ � �������������� ����������
			int shift = encrypt ? static_cast<int>(it_k) : -static_cast<int>(it_k);

			// ��������� ����� ������� � ������ ����� ��������
			size_t pos = (it_c + shift + _alphabet.size()) % _alphabet.size();
			result += _alphabet[pos];
		}
		else {
			// ���� ������ �� ������ � ��������, ������ ��������� ��� � ���������
			result += c;
		}
	}
	return result;
}

// �������� ��������� � ������� ����� �������.
std::string VernamEncoder::encode(const std::string& message) const
{
	return transform(message, true);
}

// �������������� ��������� � ������� ����� �������.
std::string VernamEncoder::decode(const std::string& message) const
{
	return transform(message, false);
}

// ����������� ��������� � ������� ����� �������.
std::string VernamEncoder::transform(const std::string& message, bool encrypt) const
{
	std::string result;
	size_t alphabetSize = _alphabet.size();

	for (size_t i = 0; i < message.length(); ++i) {
		// ������� ������ ������� � �������� ��� ������ � �����
		size_t textIndex = _alphabet.find(message[i]);
		size_t keyIndex = _alphabet.find(_key[i % _key.length()]);

		// ��������� �������� XOR ��� ��������� � ����� ��������� �� ������ ������� ��������
		size_t cipherIndex = (textIndex ^ keyIndex) % alphabetSize;

		// ��������� ������������� (��� ��������������) ������ � ���������
		result += _alphabet[cipherIndex];
	}

	return result;
}

// ������������� ������� �������� � ���������.
void EncoderContext::setEncoder(std::unique_ptr<IEncoder> encoder)
{
	_encoder = std::move(encoder);
}

// ������� ��������� � ������� �������� ��������� � ���������.
std::string EncoderContext::encodeMessage(const std::string& message)
{
	if (_encoder) {
		return _encoder->encode(message);
	}
	throw std::runtime_error("�������� �� ����������");
}

// �������������� ��������� � ������� �������� ��������� � ���������.
std::string EncoderContext::decodeMessage(const std::string& message)
{
	if (_encoder) {
		return _encoder->decode(message);
	}
	throw std::runtime_error("�������� �� ����������");
}

// ������� �������� ������ � ��������� ������� � ���������.
std::unique_ptr<IEncoder> EncoderFactory::createCaesarEncoder(int shift, const std::string& language)
{
	return std::make_unique<CaesarEncoder>(shift, GetAlphabet(language));
}

// ������� �������� �������� � ��������� ������ � ���������.
std::unique_ptr<IEncoder> EncoderFactory::createVigenereEncoder(const std::string& key, const std::string& language)
{
	return std::make_unique<VigenereEncoder>(key, GetAlphabet(language));
}

// ������� �������� ������� � ��������� ������ � ���������.
std::unique_ptr<IEncoder> EncoderFactory::createVernamEncoder(const std::string& key, const std::string& language)
{
	
	return std::make_unique<VernamEncoder>(key, GetAlphabet(language));
}

// ���������� ������� � ����������� �� ���������� �����.
std::string EncoderFactory::GetAlphabet(const std::string& language)
{
	std::string alphabet;
	if (language == "en") {
		alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		return alphabet;
	}
	else if (language == "ru") {
		alphabet = "�������������������������������������Ũ��������������������������";
		return alphabet;
	}	
	throw std::invalid_argument("���������������� ����");
}
