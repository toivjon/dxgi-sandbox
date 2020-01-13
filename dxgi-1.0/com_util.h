#pragma once

#include <wrl/client.h> // ComPtr
#include <comdef.h>		// _com_error
#include <string>

// a utility to easily catch failed HRESULTS.
inline void check_hresult(HRESULT result) {
	if (FAILED(result)) {
		throw _com_error(result);
	}
}

// a utility to count COM object references.
inline UINT countRefs(Microsoft::WRL::ComPtr<IUnknown> object) {
	object->AddRef();
	return object->Release();
}

// a utility to create a new GUID.
inline GUID createGUID() {
	GUID guid;
	check_hresult(CoCreateGuid(&guid));
	return guid;
}

// a utility to convert RECT into a descriptive string.
inline std::string rectString(RECT& rect) {
	std::string str = "{";
	str += "top: " + std::to_string(rect.top);
	str += ", right: " + std::to_string(rect.right);
	str += ", bottom: " + std::to_string(rect.bottom);
	str += ", left: " + std::to_string(rect.left);
	str += "}";
	return str;
}

// a utility to convert DWORD into a boolean string.
inline const char* boolString(DWORD value) {
	return value == 0 ? "false" : "true";
}

// a utility to convert BOOL into a boolean string.
inline const char* boolString(BOOL value) {
	return value == 0 ? "false" : "true";
}