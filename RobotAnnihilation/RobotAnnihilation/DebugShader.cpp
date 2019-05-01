#include "DebugShader.h"

DebugShader::DebugShader()
{
	myVertexShader = nullptr;
	myPixelShader = nullptr;
	myLayout = nullptr;
	myConstantBuffer = nullptr;
}

bool DebugShader::Initialize(ID3D11Device * myDevice)
{
	bool result;

	result = CompileShaders(myDevice, (WCHAR*)L"DebugVertex.vs", (WCHAR*)L"DebugPixel.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void DebugShader::Shutdown()
{
	DestroyShaders();
}

bool DebugShader::Render(ID3D11DeviceContext * myContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	bool result;

	result = UpdateShaderBuffers(myContext, world, view, projection);
	if (!result)
	{
		return false;
	}

	DrawShaders(myContext);

	return true;
}

bool DebugShader::CompileShaders(ID3D11Device * myDevice, WCHAR * vsFile, WCHAR * psFile)
{
	HRESULT result;
	ID3D10Blob *vertexShaderBuffer, *pixelShaderBuffer, *errors;
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;

	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	errors = 0;

	result = D3DCompileFromFile(vsFile, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errors);

	if (FAILED(result))
	{
		if (errors)
		{
			ProcessShaderErrors(errors);
		}

		return false;
	}

	result = D3DCompileFromFile(psFile, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errors);

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
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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

	return true;
}

void DebugShader::DestroyShaders()
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
}

void DebugShader::ProcessShaderErrors(ID3D10Blob * errors)
{
	char *errorMessages;
	unsigned int bufferSize;
	ofstream fout;

	errorMessages = (char*)(errors->GetBufferPointer());
	bufferSize = errors->GetBufferSize();
	fout.open("ShaderErrors.txt");

	for (unsigned int i = 0; i < bufferSize; i++)
	{
		fout << errorMessages[i];
	}
	fout.close();

	errors->Release();
	errorMessages = nullptr;
}

bool DebugShader::UpdateShaderBuffers(ID3D11DeviceContext * myContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped;
	ConstantBuffer *constData;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	result = myContext->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result))
	{
		return false;
	}

	constData = (ConstantBuffer*)mapped.pData;
	constData->world = world;
	constData->view = view;
	constData->projection = projection;

	myContext->Unmap(myConstantBuffer, 0);

	myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);

	return true;
}

void DebugShader::DrawShaders(ID3D11DeviceContext * myContext)
{
	myContext->IASetInputLayout(myLayout);

	myContext->VSSetShader(myVertexShader, NULL, 0);
	myContext->PSSetShader(myPixelShader, NULL, 0);

	//myContext->Draw(end::debug_renderer::get_line_vert_count(), 0);
	myContext->Draw(debug_renderer::get_line_vert_count(), 0);
	//end::debug_renderer::clear_lines();
	debug_renderer::clear_lines();
}
