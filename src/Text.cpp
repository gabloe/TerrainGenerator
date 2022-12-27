#include <Text.hpp>

#include <Asset.hpp>
#include <Logger.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <filesystem>
#include <stdexcept>

using namespace resources;

struct Font::Data {
  FT_Library ft;
  FT_Face face;
};

Font::Font(std::string fontName, uint8_t fontSize)
    : fontData{std::make_unique<Font::Data>()} {
  if (FT_Init_FreeType(&fontData->ft)) {
    logging::Logger::LogError("FREETYPE: Could not init FreeType Library");
    throw new std::runtime_error{"FREETYPE: Could not init FreeType Library"};
  }

  // Windows uses a wchar_t which we don't want to use because we don't
  // support internationalization yet.
  char sep = (char)std::filesystem::path::preferred_separator;
  auto fontPath = asset::Asset::Instance().FONT_DIR + sep + fontName;

  if (FT_New_Face(fontData->ft, fontPath.c_str(), 0, &fontData->face)) {
    logging::Logger::LogError("FREETYPE: Failed to load font");
    throw new std::runtime_error{"FREETYPE: Failed to load font"};
  }
}

Font::~Font() {
  FT_Done_Face(fontData->face);
  FT_Done_FreeType(fontData->ft);
}
