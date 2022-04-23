////////////////////////////////////////////////////////////////////////////////
// Filename: VertexColor.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
// These globals can be modified externally from the framework(C++) code.
// Use cbuffer for most globals. 
// Logically organizing these buffers is important for efficient execution of 
// shaders as well as how the graphics card will store the buffers. 
/////////////
cbuffer MatrixBuffer//c버퍼를 쓴다!
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;//여기서 보내고, 이또한 채널
    float4 color : COLOR;//색을 지정했다!
};

struct PixelInputType
{
    float4 position : SV_POSITION;//여기서 받는다, 의 일종이다
    float4 color : COLOR;//컬러는 이름 안바뀌어서 내보낸다!, SV는 다르다
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)//컬러 버텍스 셰이더
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;//아무튼 벡터가 보내질 것이다. 포지션!

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);//행렬 곱!!
    output.position = mul(output.position, viewMatrix);//행렬 변환이라
    output.position = mul(output.position, projectionMatrix);//할수있나?
    
	// Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}