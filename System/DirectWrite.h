#pragma once

class DirectWrite
{
private:
	DirectWrite();
	~DirectWrite();

private:
	static DirectWrite* instance;

private://variable
	ID2D1Factory1 * factory;
	static IDWriteFactory* writeFactory;

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;

	static ID2D1Bitmap1* bitmap;
	static IDXGISurface* surface;

	static ID2D1SolidColorBrush* brush;//글자색
	static IDWriteTextFormat* format;//글꼴

public:
	static void Create();
	static void Delete();
	static DirectWrite* GetInstance();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }

	static void CreateBackBuffer();
	static void DeleteBackBuffer();

	static void RenderText(wstring& text, RECT& rect);
};