#pragma once
#include <map>
#include <string>
#include <vector>

#include "Geometory.h"

using VecInt = std::vector<int>;

#define lpImageManager ImageManager::getInstance()

// ‰æ‘œ‚ÌÛ°ÄŞ‚â‰æ‘œÊİÄŞÙ‚Ìæ“¾“™‚ğ–Ú“I‚Æ‚µ‚½¸×½
// ¼İ¸ŞÙÄİ²İ½Àİ½
class ImageManager
{
public:
	static ImageManager& getInstance(void)
	{
		static ImageManager sInstance_;
		return sInstance_;
	}
	// ‰æ‘œ’P”­Û°ÄŞ
	void Load(std::string name);
	// ‰æ‘œ•ªŠ„Û°ÄŞ
	void Load(std::string name, Vector2I divcnt, Vector2I divsize);

	// ‰æ‘œÊİÄŞÙæ“¾
	// param@ name: ‰æ‘œÌ§²Ù–¼
	const VecInt& GetHandle(std::string name)
	{
		return handles_[name];
	}

private:
	ImageManager() = default;
	// ºËß°‹Ö~
	void operator=(const ImageManager&) = delete;
	// Û°ÄŞ‚É‰æ‘œÊİÄŞÙ‚ğŠi”[‚·‚é•Ï”
	// ‰æ‘œÌ§²Ù–¼‚Åæ“¾‚µ‚â‚·‚¢‚æ‚¤‚Éstd::map<std::string>‚É‚µ‚Ä‚¢‚é
	std::map<std::string, VecInt> handles_;
};

