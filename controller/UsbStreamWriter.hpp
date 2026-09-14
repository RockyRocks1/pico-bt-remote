#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "shared/shared.h"

class UsbStreamWriter {
public:
	bool Open(const std::string& portName, uint32_t baudRate);
	bool Write(const UsbPacket& packet);
	inline HANDLE GetHandle() const noexcept {
		return m_portHandle;
	}
private:
	HANDLE m_portHandle;
	std::string m_portName;
	uint32_t m_baudRate = 0;
};