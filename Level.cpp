#pragma once
#include <iostream>
#include "Memory.cpp"
#include "Offsets.cpp"

class Level
{
public:
    long getBasePointer()
    {
        long basePointer = offsets::REGION + offsets::LEVEL;
        return basePointer;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        std::string result = mem::ReadString(basePointer);
        return result;
    }
    bool isPlayable()
    {
        if (getName().empty())
            return false;
        if (getName().compare("mp_lobby") == 0)
            return false;
        return true;
    }
    bool isTrainingArea()
    {
        if (getName().compare("mp_rr_canyonlands_staging") == 0)
            return true;
        return false;
    }
    void print()
    {
        std::string str;
        str += "Level:\n";
        str += "\tBasePointer:\t\t\t\t" + mem::convertPointerToHexString(getBasePointer()) + "\n";
        str += "\tName:\t\t\t\t\t" + getName() + "\n";
        std::cout << str;
    }
};

struct GlowMode {
    std::byte bodyStyle, borderStyle, borderWidth, transparency;

    GlowMode() {}

    GlowMode(int x_val, int y_val, int z_val, int transparency_val) :
        bodyStyle(static_cast<std::byte>(x_val)),
        borderStyle(static_cast<std::byte>(y_val)),
        borderWidth(static_cast<std::byte>(z_val)),
        transparency(static_cast<std::byte>(transparency_val)) {}


    GlowMode(std::byte x_val, std::byte y_val, std::byte z_val, std::byte transparency_val) :
        bodyStyle(x_val),
        borderStyle(y_val),
        borderWidth(z_val),
        transparency(transparency_val) {}

    bool isZeroVector() const {
        return bodyStyle == std::byte(0)
            && borderStyle == std::byte(0)
            && borderWidth == std::byte(0)
            && borderWidth == std::byte(0);
    }

    void print() const {
        std::cout
            << "bodyStyle:" << static_cast<int>(bodyStyle)
            << " borderStyle:" << static_cast<int>(borderStyle)
            << " borderWidth:" << static_cast<int>(borderWidth)
            << " transparency:" << static_cast<int>(transparency)
            << "\n";
    }

    bool operator==(const GlowMode& other) const {
        return bodyStyle == other.bodyStyle
            && borderStyle == other.borderStyle
            && borderWidth == other.borderWidth
            && transparency == other.transparency;
    }

    bool operator!=(const GlowMode& other) const {
        return !(*this == other);
    }
};
