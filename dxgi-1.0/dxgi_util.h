#pragma once

#include <dxgi.h>
#include <string>

#define CHECK_BIT(x, bit) (x & bit) != 0

// a utility to convert DXGI_MODE_ROTATION into a descriptive string.
inline const char* rotationString(DXGI_MODE_ROTATION rotation) {
	switch (rotation) {
	case DXGI_MODE_ROTATION_IDENTITY:
		return "identity";
	case DXGI_MODE_ROTATION_ROTATE180:
		return "rotate-180";
	case DXGI_MODE_ROTATION_ROTATE270:
		return "rotate-270";
	case DXGI_MODE_ROTATION_ROTATE90:
		return "rotate-90";
	case DXGI_MODE_ROTATION_UNSPECIFIED:
		return "unspecified";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_MODE_SCALING.
inline const char* scalingString(DXGI_MODE_SCALING mode) {
	switch (mode) {
	case DXGI_MODE_SCALING_CENTERED:
		return "centered";
	case DXGI_MODE_SCALING_STRETCHED:
		return "stretched";
	case DXGI_MODE_SCALING_UNSPECIFIED:
		return "unspecified";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_MODE_SCANLINE_ORDER.
inline const char* scanlineOrderingString(DXGI_MODE_SCANLINE_ORDER mode) {
	switch (mode) {
	case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:
		return "lower-field-first";
	case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:
		return "progressive";
	case DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED:
		return "unspecified";
	case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:
		return "upper-field-first";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_RESIDENCY.
inline const char* residencyString(DXGI_RESIDENCY residency) {
	switch (residency) {
	case DXGI_RESIDENCY_EVICTED_TO_DISK:
		return "evicted-to-disk";
	case DXGI_RESIDENCY_FULLY_RESIDENT:
		return "fully-resident";
	case DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY:
		return "in-shared-memory";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_SWAP_EFFECT.
inline const char* swapEffectString(DXGI_SWAP_EFFECT effect) {
	switch (effect) {
	case DXGI_SWAP_EFFECT_DISCARD:
		return "discard";
	case DXGI_SWAP_EFFECT_FLIP_DISCARD:
		return "flip-discard";
	case DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL:
		return "flip-sequential";
	case DXGI_SWAP_EFFECT_SEQUENTIAL:
		return "sequential";
	default:
		return "unknown";
	}
}

// a utility to get string presentation of DXGI_USAGE.
inline std::string usageString(DXGI_USAGE usage) {
	std::string result;
	if (CHECK_BIT(usage, DXGI_USAGE_BACK_BUFFER)) {
		result += "[back-buffer]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_DISCARD_ON_PRESENT)) {
		result += "[discard-on-present]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_READ_ONLY)) {
		result += "[read-only]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_RENDER_TARGET_OUTPUT)) {
		result += "[render-target-output]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_SHADER_INPUT)) {
		result += "[shared-input]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_SHARED)) {
		result += "[shared]";
	}
	if (CHECK_BIT(usage, DXGI_USAGE_UNORDERED_ACCESS)) {
		result += "[unordered-access]";
	}
	return result;
}