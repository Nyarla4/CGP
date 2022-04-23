////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, const WCHAR* filename)
{
	HRESULT result;

	// Load texture data from a file by using DirectXTex lib.
	ScratchImage image;		// image format used by DirectX shader resource
	result = LoadFromDDSFile(filename, DDS_FLAGS_NONE, NULL, image);
	if (FAILED(result))
	{
			return false;
	}
	// Create shader resource view from loaded data.
	result = CreateShaderResourceView(device, image.GetImages(), 
		image.GetImageCount(), image.GetMetadata(), &m_texture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}