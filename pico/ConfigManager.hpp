#pragma once
#include "shared/shared.h"

class ConfigManager {
public:
    ConfigManager() {};
    inline void UpdateConfig(const ConfigPayload& newConfig) {
        m_currentConfig = newConfig;
    };
    inline const ConfigPayload& GetConfig() {
        return m_currentConfig;
    };
private:
    ConfigPayload m_currentConfig{};
};