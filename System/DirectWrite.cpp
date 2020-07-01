#include "stdafx.h"
#include "DirectWrite.h"

DirectWrite* DirectWrite::instance = nullptr;

IDWriteFactory* DirectWrite::writeFactory = nullptr;
ID2D1DeviceContext* DirectWrite::deviceContext = nullptr;

ID2D1Bitmap1* DirectWrite::bitmap = nullptr;
IDXGISurface* DirectWrite::surface = nullptr;

ID2D1SolidColorBrush* DirectWrite::brush = nullptr;
IDWriteTextFormat* DirectWrite::format = nullptr;

void DirectWrite::Create()
{
	assert(instance == nullptr);

	instance = new DirectWrite();
}

void DirectWrite::Delete()
{
	assert(instance != nullptr);

	delete instance;
}

DirectWrite::DirectWrite()
{
	HRESULT hr;

	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,//d3d와의 호환
		__uuidof(IDWriteFactory),
		((IUnknown**)&writeFactory)
	);
	assert(SUCCEEDED(hr));

	//D2D1_FACTORY_OPTIONS option;

	//option.debugLevel = D2D1_DEBUG_LEVEL_WARNING;//에러시 경고레벨로만
	hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&factory
	);
	assert(SUCCEEDED(hr));

	IDXGIDevice* dxgiDevice;
	hr = Device->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	hr = factory->CreateDevice
	(
		dxgiDevice,
		&device
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&deviceContext
	);
	assert(SUCCEEDED(hr));

	CreateBackBuffer();
}

DirectWrite::~DirectWrite()
{
	DeleteBackBuffer();
}

DirectWrite * DirectWrite::GetInstance()
{
	return instance;
}

void DirectWrite::CreateBackBuffer()
{
	HRESULT hr;

	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	assert(SUCCEEDED(hr));

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = NULL;

	hr = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
	assert(SUCCEEDED(hr));

	deviceContext->SetTarget(bitmap);

	hr = deviceContext->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 1), &brush);
	assert(SUCCEEDED(hr));

	writeFactory->CreateTextFormat
	(
		L"돋움체",
		NULL,
		DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		15.0f,
		L"ko",
		&format
	);
}

void DirectWrite::DeleteBackBuffer()
{
	deviceContext->SetTarget(NULL);

	SAFE_RELEASE(brush);
	SAFE_RELEASE(format);

	SAFE_RELEASE(bitmap);
	SAFE_RELEASE(surface);
}

void DirectWrite::RenderText(wstring & text, RECT & rect)
{
	D2D1_RECT_F tRect;

	tRect.left = static_cast<float>(rect.left);
	tRect.top = static_cast<float>(rect.top);
	tRect.right = static_cast<float>(rect.right);
	tRect.bottom = static_cast<float>(rect.bottom);

	DirectWrite::GetDC()->DrawTextW(text.c_str(),text.length(),format,&tRect,brush);
}
