#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

class Color {
public:
    Color();  // Default constructor
    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 100);  // Constructor with RGB values

    // Getters for color representations
    uint32_t getRGBA() const; // Returns color in RGBA8888 format
    uint32_t getBGRA() const; // Returns color in BGRA8888 format
    uint32_t getRGB() const;  // Returns color in RGB888 format


     // Setters for color values
    void setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 100);  // Set RGBA values directly
    void setRGB(uint8_t r, uint8_t g, uint8_t b);  // Set RGB values, sets alpha to 255
    void setBGRA(uint8_t b, uint8_t g, uint8_t r, uint8_t a);  // Set BGRA values
    void setColor(uint32_t color); // Set color from a packed uint32_t value

    // Utility function for creating Color objects
    static Color fromRGBA(uint32_t rgba); // Create Color object from RGBA8888

private:
    uint8_t red, green, blue, alpha;

    // Helper function to extract color components from packed value
    void extractColor(uint32_t color);
};

#endif // COLOR_H
