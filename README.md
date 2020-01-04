# dxgi-sandbox
This sandbox is dedicated for my personal study about how the Microsoft DirectX Graphics Infrastructure (DXGI) works.

DXGI is used by the following DirectX versions:
* DirectX 10
* DirectX 11
* DirectX 12

## DXGI Versions

| Version | Header    | Minimum Windows Platform    | Version Improvements |
| ------- | --------- | --------------------------- | -------------------- |
|   1.0	  | dxgi.h    | Windows Vista     		    | The base version. | 
|   1.1	  | dxgi.h    | Windows Vista (SP2) 		| - Synchronized Shared Surfaces Support<br/> - High Color Support<br/> - Maximum Frame Latency Control Support<br/> - Adapter enumeration ordering<br/> - BGRA Support<br/> |
|   1.2	  | dxgi1_2.h | Windows 7 (Platform Update) | - Support for stereo swap chain<br/> - Support for flip-model swap chain<br/> - Optimization for presentations<br/> - Desktop duplication<br/> - Optimized the use of video memory<br/> - Support for 16 bpp formats<br/> - Support for debugging APIs |
|   1.3	  | dxgi1_3.h | Windows 8.1                 | - Support for DXGI adapter memory trim<br/> - Support for multi-plane overlays<br/> - Support for overlapping and scaled swap chains<br/> - Support for selecting backbuffer subregion for swap chain<br/> - Support for lower-latency swap chain presentation |
|   1.4	  | dxgi1_4.h | Windows 10                  | |
|   1.5	  | dxgi1_5.h | Windows 10                  | |
|   1.6   | dxgi1_6.h | Windows 10                  | |

DXGI contains the following components:
* IDXGIAdapter
* IDXGIDecodeSwapChain
* IDXGIDevice
* IDXGIDeviceSubObject
* IDXGIDisplayControl
* IDXGIFactory
* IDXGIFactoryMedia
* IDXGIKeyedMutex
* IDXGIObject
* IDXGIOutput
* IDXGIOutputDuplication
* IDXGIResource
* IDXGISurface
* IDXGISwapChain
* IDXGISwapChainMedia

## Sources

[DXGI Documentation](https://docs.microsoft.com/fi-fi/windows/win32/direct3ddxgi/dx-graphics-dxgi)
