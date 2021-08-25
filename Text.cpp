

/*
#include "Text.h"

void Text::CreateDeviceIndependentResources()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&m_pD2DFactory);
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
        );
    }
   wszText= L"Hello World using  DirectWrite!";
    cTextLength = (UINT32)wcslen(wszText);
    if (SUCCEEDED(hr))
    {
        hr =m_pDWriteFactory->CreateTextFormat(
            L"Gabriola",                // Font family name.
            NULL,                       // Font collection (NULL sets it to use the system font collection).
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            72.0f,
            L"en-us",
            &m_pTextFormat
        );
    }
}
*/