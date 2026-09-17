#pragma once
#include "shared.h"

class ConfigManager {
public:
    ConfigManager() {
        // Some default values
        m_currentConfig.maxDelta = 127;
    };
    inline void UpdateConfig(const ConfigPayload& newConfig) {
        m_currentConfig = newConfig;
    };
    inline const ConfigPayload& GetConfig() {
        return m_currentConfig;
    };
private:
    ConfigPayload m_currentConfig{};
};