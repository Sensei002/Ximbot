#pragma once
#include <iostream>
#include <array>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Memory.cpp"
#include "Level.cpp"
#include <chrono>
static const int MAX_PLAYERS = 72;
class Player
{
private:
    int m_entityListIndex;
    float m_lastVisibleTime;
    long m_basePointer = 0;
    bool m_isNPC = false;
    int *m_counter;
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + ((m_entityListIndex + 1) << 5);
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = mem::ReadLong(getUnresolvedBasePointer());
        return m_basePointer;
    }

public:
    Player(int entityListIndex)
    {
        m_entityListIndex = entityListIndex;
        
    }

    Player(int entityListIndex, int *counter) :
        m_entityListIndex(entityListIndex),
        m_counter(counter) {}
        
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
    bool isValid()
    {
        return getBasePointer() > 0 && !isDead();
    }
    std::string getInvalidReason()
    {
        if (getBasePointer() == 0)
            return "Unresolved base pointer";
        else if (isDead())
            return "Player is dead";
        else if (getName().empty())
            return "Name is empty";
        else
            return "Player is valid";
    }
    float getLocationX()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationY()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationZ()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float) + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
   
    int getGlowEnable()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowEnable(int glowEnable)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        mem::WriteInt(ptrLong, glowEnable);
    }
    //==============================================
    bool isKnocked()
    {
        if (m_isNPC) {
            return false;
        }
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::BLEEDOUT_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    int getTeamNumber()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = mem::Read<int>(ptrLong);
        return result;
    }
    template <class TPlayer>
    bool isSameTeam(TPlayer* player, int gameMode)
    {
        if (gameMode == 1) {
            return (getTeamNumber() & 1) == (player->getTeamNumber() & 1);
        }
        return getTeamNumber() & 0 == player->getTeamNumber() & 0;
    }
    int getShieldValue()
    {
        long basePointer = getBasePointer();
        long shieldOffset = basePointer + offsets::CURRENT_SHIELDS;
        int result = mem::ReadInt(shieldOffset);
        return result;
    }
    int getHealthValue()
    {
        long basePointer = getBasePointer();
        long healthOffset = basePointer + offsets::CURRENT_HEALTH;
        int result = mem::ReadInt(healthOffset);
        return result;
    }
    void setCustomGlow(int health, bool isVisible, bool isSameTeam)
    {
        static const int contextId = 1; // Same as glow enable
        long basePointer = getBasePointer();
        int settingIndex = 65;
        std::array<unsigned char, 4> highlightFunctionBits = {
            2,   // InsideFunction
            125, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
            64,  // OutlineRadius: size * 255 / 8
            64   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
        };
        std::array<float, 3> glowColorRGB = { 0, 0, 0 };
        if (isSameTeam) {
            settingIndex = 20;
        } else if (!isVisible) {
            settingIndex = 65;
            glowColorRGB = { 0.5, 0.5, 0.5 }; // grey
        } else if (health >= 225) {
            settingIndex = 66;
            glowColorRGB = { 1, 0, 0 }; // red
        } else if (health >= 200) {
            settingIndex = 67;
            glowColorRGB = { 0.5, 0, 0.5 }; // purple
        } else if (health >= 175) {
            settingIndex = 68;
            glowColorRGB = { 0, 0.5, 1 }; // blue
        } else if (health >= 100) {
            settingIndex = 69;
            glowColorRGB = { 0, 1, 0.5 }; // light green
        } else {
            settingIndex = 70;
            glowColorRGB = { 0, 1, 0 }; // green
        }
        mem::Write<unsigned char>(basePointer + offsets::GLOW_ACTIVE_STATES + contextId, settingIndex);
        if (!isSameTeam) {
            long highlightSettingsPtr = mem::Read<long>(offsets::REGION + offsets::HIGHLIGHT_SETTINGS);
            mem::Write<typeof(highlightFunctionBits)>(
                highlightSettingsPtr + offsets::HIGHLIGHT_TYPE_SIZE * settingIndex + 4, highlightFunctionBits);
            mem::Write<typeof(glowColorRGB)>(
                highlightSettingsPtr + offsets::HIGHLIGHT_TYPE_SIZE * settingIndex + 8, glowColorRGB);
        }
    }
    bool isNPC()
    {
        if (!m_isNPC) {
            long basePointer = getBasePointer();
            m_isNPC = basePointer > 0 && mem::getClassName(basePointer) == "CAI_BaseNPC";
        }
        return m_isNPC;
    }
    //==============================================

    int getGlowThroughWall()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowThroughWall(int glowThroughWall)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        mem::WriteInt(ptrLong, glowThroughWall);
    }
    float getLastVisibleTime()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LAST_VISIBLE_TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    bool isVisible()
    {
        const float lastVisibleTime = getLastVisibleTime();
        const bool isVisible = lastVisibleTime > m_lastVisibleTime;
        m_lastVisibleTime = lastVisibleTime;
        return isVisible;
    }
    void print()
    {
        std::cout << "Player[" + std::to_string(m_entityListIndex) + "]:\n";
        std::cout << "\tUnresolvedBasePointer:\t\t\t" + mem::convertPointerToHexString(getUnresolvedBasePointer()) + "\n";
        std::cout << "\tBasePointer:\t\t\t\t" + mem::convertPointerToHexString(getBasePointer()) + "\n";
        std::cout << "\tIsValid:\t\t\t\t" + std::to_string(isValid()) + "\n";
        std::cout << "\tInvalidReason:\t\t\t\t" + getInvalidReason() + "\n";
        if (!isValid())
        {
            std::cout << "\tLocationOriginX:\t\t\t" + utils::convertNumberToString(getLocationX()) + "\n";
            std::cout << "\tLocationOriginY:\t\t\t" + utils::convertNumberToString(getLocationY()) + "\n";
            std::cout << "\tLocationOriginZ:\t\t\t" + utils::convertNumberToString(getLocationZ()) + "\n";
            std::cout << "\tTeamNumber:\t\t\t\t" + utils::convertNumberToString(getTeamNumber()) + "\n";
            std::cout << "\tGlowEnable:\t\t\t\t" + utils::convertNumberToString(getGlowEnable()) + "\n";
            std::cout << "\tGlowThroughWall:\t\t\t" + utils::convertNumberToString(getGlowThroughWall()) + "\n";
        }
    }
};

class RemotePlayers
{
private:
    Level *m_level;
    int *m_counter;
    std::vector<Player> m_players;
    std::vector<Player> m_trainingAreaPlayers;
    std::chrono::steady_clock::time_point m_lastUpdated = {};

    void findTrainingAreaPlayers(std::vector<Player>& result)
    {
        result.clear();
        for (int i = 0; i < 20000; i++) {
            Player player(i, m_counter);
            if (player.isNPC()) {
                result.emplace_back(std::move(player));
            }
        }
    }

public:
    RemotePlayers(Level *level, int *counter) {
        m_level = level;
        m_counter = counter;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            m_players.emplace_back(i, m_counter);
        }
    }

    void markForPointerResolution() {
        auto& players = getPlayers();
        for (auto& player : players) {
            player.markForPointerResolution();
        }
    }

    std::vector<Player>& getPlayers() {
        if (m_level->isTrainingArea()) {
            auto now = std::chrono::steady_clock::now();
            if (now - m_lastUpdated > std::chrono::seconds(1) || std::any_of(
                m_trainingAreaPlayers.begin(),
                m_trainingAreaPlayers.end(),
                [] (auto& player) { return !player.isValid(); })) {
                findTrainingAreaPlayers(m_trainingAreaPlayers);
                m_lastUpdated = now;
                // printf("found %lu training area players\n\n", m_trainingAreaPlayers.size());
            }
            return m_trainingAreaPlayers;
        } else {
            m_trainingAreaPlayers.clear();
            return m_players;
        }
    }
};
