#include <iostream>
#pragma once
class IEncoder
{
public:
	virtual ~IEncoder() = default;
	virtual std::string encode(const std::string& message) const = 0;
	virtual std::string decode(const std::string& message) const = 0;

};

class CaesarEncoder : public IEncoder {
public:
	CaesarEncoder(int shift, const std::string& alphabet) : _shift(shift), _alphabet(alphabet) {}

	std::string encode(const std::string& message) const override;
	std::string decode(const std::string& message) const override;


private:
	int _shift;
	std::string _alphabet;
	std::string transform(const std::string& message, int shift) const;
};

class VigenereEncoder : public IEncoder {
public:
	VigenereEncoder(const std::string& key, const std::string& alphabet) : _key(key), _alphabet(alphabet) {}

	std::string encode(const std::string& message) const override;
	std::string decode(const std::string& message) const override;

private:
	std::string _key;
	std::string _alphabet;
	std::string transform(const std::string& message, bool encrypt) const;
};

class VernamEncoder : public IEncoder {
public:
	VernamEncoder(const std::string& key, const std::string& alphabet) : _key(key), _alphabet(alphabet) {}
	std::string encode(const std::string& message) const override;
	std::string decode(const std::string& message) const override;

private:
	std::string _key;
	std::string _alphabet;
	std::string transform(const std::string& message, bool encrypt) const;
};

class EncoderContext {
public:
	void setEncoder(std::unique_ptr<IEncoder> encoder);
	std::string encodeMessage(const std::string& message);
	std::string decodeMessage(const std::string& message);

private:
	std::unique_ptr<IEncoder> _encoder;
};

class EncoderFactory {
public:
	std::unique_ptr<IEncoder> createCaesarEncoder(int shift, const std::string& language);
	std::unique_ptr<IEncoder> createVigenereEncoder(const std::string& key, const std::string& language);
	std::unique_ptr<IEncoder> createVernamEncoder(const std::string& key, const std::string& language);
private:
	std::string GetAlphabet(const std::string& language);
};

