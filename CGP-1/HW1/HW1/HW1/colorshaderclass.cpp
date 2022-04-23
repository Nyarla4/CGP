////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "colorshaderclass.h"


ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}//셰이더들, 레이아웃, 버퍼 초기화


ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}


ColorShaderClass::~ColorShaderClass()
{
}


bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, (WCHAR*)L"\color.vs", (WCHAR*)L"\color.ps");
	if(!result)//셰이더를 찾아서 초기화한다!!
	{
		return false;
	}

	return true;
}


void ColorShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

// This first sets the parameters inside the shader, and then draws the triangle 
// using the HLSL shader.
bool ColorShaderClass::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int indexCount,
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, int lightmode)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if(!result)//각 매트릭스(행렬 아마)를 넘겨준다! 매 프레임마다!
	{
		return false;
	}

	ID3D10Blob* pixelShaderBuffer; pixelShaderBuffer = 0;
	ID3D10Blob* errorMessage; errorMessage = 0;
	if (lightmode == 0)
	{
		D3DX11CompileFromFile((WCHAR*)L"\color.ps", NULL, NULL, "ColorPixelShader", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	}
	if (lightmode == 1)
	{
		D3DX11CompileFromFile((WCHAR*)L"\color.ps", NULL, NULL, "ColorPixelShader_", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	}
	device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

// This loads the shader files and makes it usable to DirectX and the GPU. 
// This also sets up the layout and how the vertex buffer data is going to 
// look on the graphics pipeline in the GPU. The layout will need the match 
// the VertexType in the modelclass.h file as well as the one defined in the 
// color.vs file.
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, 
	WCHAR* vsFilename, WCHAR* psFilename)//셰이더 초기화!
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = {};
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//4.6 00:26:50 이전↑
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", //컬러 버텍스 셰이더라는 것을 쓸것이다 선언
		/*규칙!*/D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))//파일이름, 버텍스 셰이더 등등으로 파일을 컴파일
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))//여기서 할건 컬러 픽셀 셰이더, 셰이더 모델은 5.0
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), //컴파일한걸 가지고
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);//만들어낸다!!
	if(FAILED(result))
	{
		return false;
	}

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), //위에서는 버텍스 셰이더
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);//여기서는 픽셀 셰이더(만드는거)
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description that will be processed by the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	// The AlignedByteOffset indicates how the data is spaced in the buffer: 12 bytes for 
	// position and the next 16 bytes for color. Use D3D11_APPEND_ALIGNED_ELEMENT which figures
	// out the spacing.//레이아웃 부분
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//보내주는 타입(플로트, 32비트×3(xyz))
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//버텍스 데이터이다 이것은
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//이건 플로트, 32비트×4(rgba)
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;//정확한 규격 사이즈
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	//4.6 00:38:00근처
	// Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);//이제 GPU로 보낼 준비!
	//이렇게 보내겠다 하고 레이아웃 설정했다는 뜻
	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, 
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// The buffer is updated each frame.동적으로 해뒀다!
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);	//C버퍼로 해뒀다
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수사이즈 버퍼를 쓸것이다(다이나믹:바뀐다, 콘스탄트:안바뀐다)
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Writing data by CPU
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);//크리에이트 버퍼!
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void ColorShaderClass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// The sets up the global variables in the shader. The transfom matrices are sent into the 
// vertex shader during the Render function call.
bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, //이제 데이터를 보낸다!
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.행렬 순서 정리
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))//Map:매트릭스 버퍼로 넘겨준다!(매핑)
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.로컬로 다 얻어온다
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//이걸로 보내준다!!라기보다는 연결해준다
	return true;
}

// First, this sets the input layout to be active in the input assembler. This lets the GPU know
// the format of the data in the vertex buffer. 
// Second, this sets the vertex and pixel shaders to render the vertex buffer by calling the 
// DrawIndexed DirectX 11 function using the D3D device context.
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{//넘겨준 정보를 화면으로!!
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);//레이아웃 활성화
    // Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);//버텍스 셰이더와
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);//픽셀 셰이더 실행!

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);//이제 그린다!!
	//4.6 00:44:30근처
	return;
}