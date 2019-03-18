#include "ShaderStuff.h"

AnimatedShader::AnimatedShader()
{
	//Zero memory 
	myVertexShader = nullptr;
	myPixelShader = nullptr;
	myLayout = nullptr;
	myConstantBuffer = nullptr;
	myConstantLightBuffer = nullptr;
	mySampler = nullptr;
}

//Set up the shaders
bool AnimatedShader::Initialize(ID3D11Device *myDevice)
{
	bool result;

	result = CompileShaders(myDevice, (WCHAR*)L"VertexShader.vs", (WCHAR*)L"PixelShader.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

//Cleans up all objects
void AnimatedShader::Shutdown()
{
	DestroyShaders();
}

//Called every frame, updates the shader information and then draws them
bool AnimatedShader::Render(ID3D11DeviceContext *myDeviceContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, ID3D11ShaderResourceView *normalTexture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, std::vector<float4x4> binds, XMFLOAT4 myPos[], XMFLOAT4 myCol[], XMFLOAT4 spotCol, XMFLOAT4 spotDir, XMFLOAT4 spotPos, XMFLOAT4 spotEx)
{
	bool result;

	//Update information
	result = UpdateShaderBuffers(myDeviceContext, world, view, projection, texture, lightDir, dirColor, binds, myPos, myCol, spotCol, spotDir, spotPos, spotEx);
	if (!result)
	{
		return false;
	}

	//Set the shaders and draw 
	DrawShaders(myDeviceContext, indicies);

	return true;
}

//Loads in the shaders from a file, also initializes the constant buffers and the sampler 
bool AnimatedShader::CompileShaders(ID3D11Device *myDevice, WCHAR *vertexFile, WCHAR* pixelFile)
{
	HRESULT result;
	ID3D10Blob *vertexShaderBuffer, *pixelShaderBuffer, *errors;
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc, lightBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	errors = 0;

	//Compile the vertex shader from the file
	result = D3DCompileFromFile(vertexFile, NULL, NULL, "Main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errors);

	if (FAILED(result))
	{
		//Process the errors if there were any, if there were no errors and this still failed, that means the shader file wasn't found
		if (errors)
		{
			ProcessShaderErrors(errors);
		}

		return false;
	}

	//Compile the vertex shader from the file
	result = D3DCompileFromFile(pixelFile, NULL, NULL, "Main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errors);

	if (FAILED(result))
	{
		//Process the errors if there were any, if there were no errors and this still failed, that means the shader file wasn't found
		if (errors)
		{
			ProcessShaderErrors(errors);
		}

		return false;
	}

	//Creat the vertex shader 
	result = myDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create the pixel shader 
	result = myDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize input layout data 
	D3D11_INPUT_ELEMENT_DESC theLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"JOINTS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//For getting the array size of the input layout
	numElements = sizeof(theLayout) / sizeof(theLayout[0]);

	//Create the input layout
	result = myDevice->CreateInputLayout(theLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myLayout);
	if (FAILED(result))
	{
		return false;
	}

	//Don't need the vertex and shader buffer holders anymore 
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	//Set the constant buffer's data
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	//Create the constant buffer 
	result = myDevice->CreateBuffer(&constantBufferDesc, NULL, &myConstantBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set the constant light buffer's data
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(ConstantLightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//Create the constant light buffer 
	result = myDevice->CreateBuffer(&lightBufferDesc, NULL, &myConstantLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the sampler state's data
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the sampler state
	result = myDevice->CreateSamplerState(&samplerDesc, &mySampler);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

//Cleans up all object pointers
void AnimatedShader::DestroyShaders()
{
	if (myConstantBuffer)
	{
		myConstantBuffer->Release();
		myConstantBuffer = nullptr;
	}

	if (myLayout)
	{
		myLayout->Release();
		myLayout = nullptr;
	}

	if (myVertexShader)
	{
		myVertexShader->Release();
		myVertexShader = nullptr;
	}

	if (myPixelShader)
	{
		myPixelShader->Release();
		myPixelShader = nullptr;
	}

	if (mySampler)
	{
		mySampler->Release();
		mySampler = nullptr;
	}

	if (myConstantLightBuffer)
	{
		myConstantLightBuffer->Release();
		myConstantLightBuffer = nullptr;
	}
}

//Writes any shader errors to a text doc 
void AnimatedShader::ProcessShaderErrors(ID3D10Blob *errors)
{
	char *errorMessages;
	unsigned int bufferSize;
	ofstream fout;

	//Store the error messages and the amount of error messages 
	errorMessages = (char*)(errors->GetBufferPointer());
	bufferSize = errors->GetBufferSize();
	fout.open("ShaderErrors.txt");

	//Write each error message to the file
	for (unsigned int i = 0; i < bufferSize; i++)
	{
		fout << errorMessages[i];
	}
	fout.close();

	//Clean up the error object
	errors->Release();
	errorMessages = nullptr;
}

//Updates and sets the constant buffer data 
bool AnimatedShader::UpdateShaderBuffers(ID3D11DeviceContext *myDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, std::vector<float4x4> binds, XMFLOAT4 myPos[], XMFLOAT4 myCol[], XMFLOAT4 spotCol, XMFLOAT4 spotDir, XMFLOAT4 spotPos, XMFLOAT4 spotEx)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped;
	ConstantBuffer *constData;
	ConstantLightBuffer *constLightData;

	//Transpose the matricies 
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	//Map the subresource to the constant buffer. This means that as the subresource data changes, so will the constant buffer 
	result = myDeviceContext->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result))
	{
		return false;
	}

	//Use a constant buffer data holder to change the constant buffer 
	constData = (ConstantBuffer*)mapped.pData;
	constData->world = world;
	constData->view = view;
	constData->projection = projection;

	for (int i = 0; i < binds.size(); i++)
	{
		constData->bindPoses[i] = binds[i];
	}


	//Unmap the buffer and subresource
	myDeviceContext->Unmap(myConstantBuffer, 0);

	//Set the constant buffer 
	myDeviceContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);

	//Set the texture 
	myDeviceContext->PSSetShaderResources(0, 1, &texture);

	//Map the subresource to the constant light buffer.
	result = myDeviceContext->Map(myConstantLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result))
	{
		return false;
	}

	//Use a constant light buffer data holder to change the constant light buffer 
	constLightData = (ConstantLightBuffer*)mapped.pData;
	constLightData->directionColor = dirColor;
	constLightData->lightDirection = lightDir;
	constLightData->padding = 0.0f;
	constLightData->positions[0] = myPos[0];
	constLightData->positions[1] = myPos[1];
	constLightData->colors[0] = myCol[0];
	constLightData->colors[1] = myCol[1];
	constLightData->spotLightColor = spotCol;
	constLightData->spotLightDirection = spotDir;
	constLightData->spotLightPosition = spotPos;
	constLightData->spotLightExtra = spotEx;

	//Unmap the buffer and subresource
	myDeviceContext->Unmap(myConstantLightBuffer, 0);

	//Set the constant light buffer 
	myDeviceContext->PSSetConstantBuffers(0, 1, &myConstantLightBuffer);

	return true;
}

//Sets all the shader data 
void AnimatedShader::DrawShaders(ID3D11DeviceContext *myDeviceContext, int indicies)
{
	//Set the input layout
	myDeviceContext->IASetInputLayout(myLayout);

	//Set the shaders 
	myDeviceContext->VSSetShader(myVertexShader, NULL, 0);
	myDeviceContext->PSSetShader(myPixelShader, NULL, 0);

	//Set the sampler 
	myDeviceContext->PSSetSamplers(0, 1, &mySampler);

	//Draw with the shaders 
	myDeviceContext->DrawIndexed(indicies, 0, 0);
}