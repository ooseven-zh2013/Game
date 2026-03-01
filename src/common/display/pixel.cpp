#include "common/display/pixel.h"

using namespace display;

Pixel::Pixel(Color background) : m_background(background) {}
Color Pixel::GetBackground() const { return m_background; }

void Pixel::SetBackground(Color background) { m_background = background; }

char Pixel::getCh() const { return m_character; }