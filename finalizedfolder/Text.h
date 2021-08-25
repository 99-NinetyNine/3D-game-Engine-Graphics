#pragma once
#include "dwrite.h"
#include "d2d1.h"
class Text
{
private:
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1SolidColorBrush* m_pBlackBrush;

private:
	const wchar_t* wszText;
	UINT32 cTextLength;
public:
	void	CreateDeviceIndependentResources();

};

