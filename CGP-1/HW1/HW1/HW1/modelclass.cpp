////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(int type, ID3D11Device* device)
{
	bool result = false;


	// Initialize the vertex and index buffers.
	switch (type)
	{
	case 0:
		result = InitializeBuffersHexa(device);
		break;
	case 1:
		result = InitializeBuffersSqu(device);
		break;
	case 2:
		result = InitializeBuffersPent(device);
		break;
	default:
		break;
	}
	
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffersHexa(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = 12;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[0].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-0.75f, -0.3f, 0.0f);  // Under left
	vertices[1].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(-0.75f, 0.3f, 0.0f);  // Up left.
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
	
	vertices[3].position = D3DXVECTOR3(0.0f, -1.0f, 0.0f);  // Bottom middle.
	vertices[3].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(0.75f, -0.3f, 0.0f);  // Under right.
	vertices[4].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[5].position = D3DXVECTOR3(0.75f, 0.3f, 0.0f);  // Up right.
	vertices[5].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	//?????? ??????
	// Load the index array with data.
	// Create the triangle in the clockwise order (counterclockwise: back face culling).:?????? ???? ?????? ????.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;
	indices[6] = 0;
	indices[7] = 4;
	indices[8] = 3;
	indices[9] = 0;
	indices[10] = 5;
	indices[11] = 4;
	//?????? ??????
	// Set up the description of the static vertex buffer.//Desc:description????, ??????????
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;//?? ??
	vertexBufferDesc.MiscFlags = 0;//??????
	vertexBufferDesc.StructureByteStride = 0;//0???? ????

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;//????????????:GPU?????? ????????(????????????????)
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.?????? ?????? ?????? ????, ???? ????????????
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;//?????? ??????
	vertices = 0;

	delete[] indices;//????????
	indices = 0;

	return true;
}

bool ModelClass::InitializeBuffersSqu(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);  // Top left.
	vertices[1].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Top right.
	vertices[2].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[3].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//?????? ??????
	// Load the index array with data.
	// Create the triangle in the clockwise order (counterclockwise: back face culling).:?????? ???? ?????? ????.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle->left.
	indices[2] = 2;  // Bottom->Top right.
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	//?????? ??????
	// Set up the description of the static vertex buffer.//Desc:description????, ??????????
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;//?? ??
    vertexBufferDesc.MiscFlags = 0;//??????
	vertexBufferDesc.StructureByteStride = 0;//0???? ????

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;//????????????:GPU?????? ????????(????????????????)
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.?????? ?????? ?????? ????, ???? ????????????
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;//?????? ??????
	vertices = 0;

	delete [] indices;//????????
	indices = 0;

	return true;
}

bool ModelClass::InitializeBuffersPent(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 5;

	// Set the number of indices in the index array.
	m_indexCount = 9;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle
	vertices[0].color = D3DXVECTOR4(0.01f, 0.01f, 0.01f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 0.25f, 0.0f);  // Up left.
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(-0.75f, -1.0f, 0.0f);  // Under left.
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(0.75f, -1.0f, 0.0f);  // Under right.
	vertices[3].color = D3DXVECTOR4(0.5f, 1.0f, 1.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, 0.25f, 0.0f);  // Up right.
	vertices[4].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//?????? ??????
	// Load the index array with data.
	// Create the triangle in the clockwise order (counterclockwise: back face culling).:?????? ???? ?????? ????.
	indices[0] = 0;  
	indices[1] = 2;  
	indices[2] = 1;  
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;
	indices[6] = 0;
	indices[7] = 4;
	indices[8] = 3;
	//?????? ??????
	// Set up the description of the static vertex buffer.//Desc:description????, ??????????
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;//?? ??
	vertexBufferDesc.MiscFlags = 0;//??????
	vertexBufferDesc.StructureByteStride = 0;//0???? ????

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;//????????????:GPU?????? ????????(????????????????)
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.?????? ?????? ?????? ????, ???? ????????????
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;//?????? ??????
	vertices = 0;

	delete[] indices;//????????
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// This sets the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Once the GPU has an active vertex buffer, it can then use the shader to render that buffer. 
// This function also defines how those buffers should be drawn such as triangles, lines, fans, 
// and etc using the IASetPrimitiveTopology DirectX function.
//GPU?? ???? ?????????? ????????!!
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);//4.2 1:24:??????
	//?? ??????
    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);//?????????? ?? ??????
	//????????
    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//?????? ?????????????????? ?????? ??????!
	return;
}