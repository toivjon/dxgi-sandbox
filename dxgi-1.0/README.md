# dxgi-1.0
This sandbox is dedicated for my personal study about how Microsoft DirectX Graphics Infrastructure (DXGI) 1.0 works.

## Interfaces

![alt text](https://github.com/toivjon/dxgi-sandbox/blob/master/dxgi-1.0/images/dxgi-interfaces-1.0.svg "DXGI 1.0 Interfaces")

| Interface Name         | Description |
| ---------------------- | ----------- |
| IUnknown               | The root interface for all COM interfaces. |
| IDXGIObject            | The base interface for most DXGI objects. |
| IDXGIFactory           | A factory object used to build DXGI objects. |
| IDXGIAdapter 		     | Represents a display subsystem containing GPUs, DACs and video memory. |
| IDXGIDevice 		     | Represents an abstraction of an DXGI object that produce image data. |
| IDXGIDeviceSubObject 	 | An abstraction for all objects that are tied to a DXGI device. |
| IDXGIOutput            | Represents an adapter output such as monitor or television. |
| IDXGISurface           | Represents an 2D image-data section in memory. |
| IDXGISwapChain         | Represents a swap chain for presenting surfaces to an output. |
| IDXGIResource          | Represents a shareable resource within a memory. |