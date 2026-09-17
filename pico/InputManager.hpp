#pragma once
#include "shared.h"
#include "ConfigManager.hpp"
#include <array>
#include <optional>

template <typename T, size_t queueCapacity>
class FixedQueue {
public:
	FixedQueue() = default;
	bool PushBack(const T& element) {
		if (m_currentSize == queueCapacity)
			return false;
		m_queue[m_tailIndex] = element;
		m_tailIndex = (m_tailIndex + 1) % queueCapacity;
		m_currentSize++;

		return true;
	}
	std::optional<T> PopBack() {
		if (m_currentSize == 0)
			return std::nullopt;

		m_tailIndex = (m_tailIndex == 0) ? queueCapacity - 1 : m_tailIndex - 1;
		T element = m_queue[m_tailIndex];
		m_currentSize--;

		return element;
	}
	bool PushFront(const T& element) {
		if (m_currentSize == queueCapacity)
			return false;

		m_headIndex = (m_headIndex == 0) ? queueCapacity - 1 : m_headIndex - 1;
		m_queue[m_headIndex] = element;
		m_currentSize++;

		return true;
	}
	std::optional<T> PopFront() {
		if (m_currentSize == 0)
			return std::nullopt;
		T element = m_queue[m_headIndex];
		m_headIndex = (m_headIndex + 1) % queueCapacity;
		m_currentSize--;

		return element;
	}
	
	
	inline size_t GetSize() const noexcept {
		return m_currentSize;
	}
	inline size_t GetCapacity() const noexcept {
		return queueCapacity;
	}
	inline bool IsEmpty() const noexcept {
		return m_currentSize == 0;
	}
private:
	std::array<UsbPacket, queueCapacity> m_queue;
	size_t m_headIndex = 0;
	size_t m_tailIndex = 0;
	size_t m_currentSize = 0;
};


class InputManager {
public:
	InputManager(ConfigManager* configManager): m_configManager(configManager) {};
	std::optional<UsbPacket> GetNextPacket();
	inline bool IsPacketQueueEmpty() const noexcept { return m_packetQueue.IsEmpty(); }
	void ProcessUsbPacket(UsbPacket& packet);
private:
	FixedQueue<UsbPacket, 1024> m_packetQueue;
	ConfigManager* m_configManager;

	void ProcessMousePayload(MousePayload& mouse);
	void ProcessKeyboardPayload(KeyboardPayload& keyboard);
	void ProcessConfigPayload(ConfigPayload& config);
};