#include "SkyboxShader.h"

SkyboxShader::SkyboxShader()
{
	myVertexShader = nullptr;
	myPixelShader = nullptr;
	myLayout = nullptr;
	myConstantBuffer = nullptr;
	mySampler = nullptr;
}

bool SkyboxShader::Initialize(ID3D11Device* myDevice)
{
	bool result; 

	result = CompileShaders(myDevice, (WCHAR*)L"Skybox.vs", (WCHAR*)L"Skybox.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void SkyboxShader::Shutdown()
{
	DestroyShaders();
}

bool SkyboxShader::Render(ID3D11DeviceContext *myDeviceContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture)
{
	bool result;

	result = UpdateShaderBuffers(myDeviceContext, world, view, projection, texture);
	if (!result)
	{
		return false;
	}

	DrawShaders(myDeviceContext, indicies);

	return true;
}

bool SkyboxShader::CompileShaders(ID3D11Device *myDevice, WCHAR *vertexFile, WCHAR *pixelFile)
{
	HRESULT result;
	ID3D10Blob *vertexShaderBuffer, *pixelShaderBuffer, *errors;
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	errors = 0;

	result = D3DCompileFromFile(vertexFile, NULL, NULL, "SkyboxVertex", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errors);
	if (FAILED(result))
	{
		if (errors)
		{
			ProcessShaderErrors(errors);
		}
		return false;
	}

	result = D3DCompileFromFile(pixelFile, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errors);

	if (FAILED(result))
	{
		if (errors)
		{
			ProcessShaderErrors(errors);
		}
		return false;
	}

	result = myDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC theLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"JOINTS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	numElements = sizeof(theLayout) / sizeof(theLayout[0]);

	result = myDevice->CreateInputLayout(theLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	result = myDevice->CreateBuffer(&constantBufferDesc, NULL, &myConstantBuffer);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = myDevice->CreateSamplerState(&samplerDesc, &mySampler);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SkyboxShader::DestroyShaders()
{
	if (myVertexShader)
	{
		myVertexShader->Release();
		myVertexShader = nullptr;
	}

	if (myLayout)
	{
		myLayout->Release();
		myLayout = nullptr;
	}

	if (myPixelShader)
	{
		myPixelShader->Release();
		myPixelShader = nullptr;
	}

	if (myConstantBuffer)
	{
		myConstantBuffer->Release();
		myConstantBuffer = nullptr;
	}

	if (mySampler)
	{
		mySampler->Release();
		mySampler = nullptr;
	}
}

void SkyboxShader::ProcessShaderErrors(ID3D10Blob *errors)
{
	char *errorMessages;
	unsigned int bufferSize;
	ofstream fout;

	errorMessages = (char*)(errors->GetBufferPointer());
	bufferSize = (unsigned int)errors->GetBufferSize();
	fout.open("ShaderErrors.txt");

	for (unsigned int i = 0; i < bufferSize; i++)
	{
		fout << errorMessages[i];
	}
	fout.close();

	errors->Release();
	errorMessages = nullptr;
}

bool SkyboxShader::UpdateShaderBuffers(ID3D11DeviceContext *myDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped;
	ConstantBuffer *constData;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	result = myDeviceContext->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result))
	{
		return false;
	}

	constData = (ConstantBuffer*)mapped.pData;
	constData->world = world;
	constData->view = view;
	constData->projection = projection;

	myDeviceContext->Unmap(myConstantBuffer, 0);

	myDeviceContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);

	myDeviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void SkyboxShader::DrawShaders(ID3D11DeviceContext *myDeviceContext, int indicies)
{
	myDeviceContext->IASetInputLayout(myLayout);

	myDeviceContext->VSSetShader(myVertexShader, NULL, 0);
	myDeviceContext->PSSetShader(myPixelShader, NULL, 0);

	myDeviceContext->PSSetSamplers(0, 1, &mySampler);

	myDeviceContext->DrawIndexed(indicies, 0, 0);
}