#include "Color.h"
#include <iostream>

Color::Color() : red(192), green(192), blue(192), alpha(255) {}
Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : red(r), green(g), blue(b), alpha(a) {}


uint32_t Color::getRGBA() const {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t Color::getBGRA() const {
    return (blue << 24) | (green << 16) | (red << 8) | alpha;
}

uint32_t Color::getRGB() const {
    return (red << 16) | (green << 8) | blue;
}

Color Color::fromRGBA(uint32_t rgba) {
    return Color((rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF, (rgba >> 24) & 0xFF);
}

void Color::setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

void Color::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    red = r;
    green = g;
    blue = b;
    alpha = 255; // Default alpha to fully opaque
}

void Color::setBGRA(uint8_t b, uint8_t g, uint8_t r, uint8_t a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

void Color::setColor(uint32_t color) {
    extractColor(color);
}

void Color::extractColor(uint32_t color) {
    if (color & 0xFF000000) { // Alpha channel present (RGBA or BGRA)
        red = (color >> 16) & 0xFF;
        green = (color >> 8) & 0xFF;
        blue = color & 0xFF;
        alpha = (color >> 24) & 0xFF;
    } else { // No alpha channel (RGB format)
        red = (color >> 16) & 0xFF;
        green = (color >> 8) & 0xFF;
        blue = color & 0xFF;
        alpha = 255; // Default alpha to fully opaque
    }
}