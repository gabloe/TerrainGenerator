#pragma once

// https://learnopengl.com/In-Practice/Text-Rendering

#include <glm/vec2.hpp>

#include <memory>
#include <string>

namespace resources {

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;         // Size of glyph
  glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Offset to advance to next glyph
};

class Font {
 private:
  // PIMPL idiom: https://en.wikipedia.org/wiki/Opaque_pointer
  // Using this to make sure we don't include external libraries unless we
  // absolutely have to.
  struct Data;
  std::unique_ptr<Data> fontData;

 public:
  /// @brief Create a new font using the font type and font size.
  /// @param fontType The font type, i.e arial.ttf
  /// @param fontSize The font size.
  Font(std::string fontType, uint8_t fontSize);

  ~Font();
};
}  // namespace resources