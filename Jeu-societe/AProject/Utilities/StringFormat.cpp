#include "StringFormat.hpp"

sf::Font* StringFormat::defaultFont = nullptr;

std::string StringFormat::VFormat(const char* _fmt, va_list _args)
{
	va_list tmp;
	va_copy(tmp, _args);
	int needed = std::vsnprintf(nullptr, 0, _fmt, tmp);
	va_end(tmp);

	if (needed < 0) return std::string();

	std::vector<char> buf(static_cast<size_t>(needed) + 1);
	std::vsnprintf(buf.data(), buf.size(), _fmt, _args);
	return std::string(buf.data(), static_cast<size_t>(needed));
}

std::string StringFormat::Format(const char* _fmt, ...)
{
	va_list args;
	va_start(args, _fmt);
	std::string s = VFormat(_fmt, args);
	va_end(args);
	return s;
}

sf::Font& StringFormat::GetDefaultFont(void)
{
	return *defaultFont;
}

void StringFormat::Load()
{
	defaultFont = new sf::Font();
	defaultFont->loadFromFile("Assets/Font.ttf");
}

void StringFormat::Unload()
{
	delete defaultFont;
	defaultFont = nullptr;
}