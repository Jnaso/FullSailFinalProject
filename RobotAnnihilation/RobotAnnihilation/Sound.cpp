#include "Sound.h"

Sound::Sound(char *filename, int v)
{
	//Initialize pointers  
	myDirectSound = nullptr;
	myPrimary = nullptr;
	mySecondary = nullptr;
	soundFile = filename;
	volume = v;
}

//Initializes the sound buffers and loads the sound from a file
bool Sound::Initialize(HWND window)
{
	bool result;

	//Create Direct sound object and primary buffer
	result = InitializeDirectSound(window);
	if (!result)
	{
		return false;
	}

	//Load in the data into the secondary buffer
	result = LoadWaveFile(soundFile, &mySecondary);
	if (!result)
	{
		return false;
	}
	return true;
}

//Cleans up all objects
void Sound::Shutdown()
{
	ShutdownWaveFile(&mySecondary);
	ShutdownDirectSound();
}

//Create the direct sound object and the primary buffer
bool Sound::InitializeDirectSound(HWND window)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;

	//Create the direct sound object 
	result = DirectSoundCreate8(NULL, &myDirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = myDirectSound->SetCooperativeLevel(window, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	//Set the primary buffer data
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Create the primary buffer
	result = myDirectSound->CreateSoundBuffer(&bufferDesc, &myPrimary, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

//Clean up the direct sound and primary buffer
void Sound::ShutdownDirectSound()
{
	if (myPrimary)
	{
		myPrimary->Release();
		myPrimary = nullptr;
	}

	if (myDirectSound)
	{
		myDirectSound->Release();
		myDirectSound = nullptr;
	}
}

//Load the wave data into the secondary buffer 
bool Sound::LoadWaveFile(const char *filename, IDirectSoundBuffer8 **secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeader wavFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer *tempBuffer;
	unsigned char *waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&wavFileHeader, sizeof(wavFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	if (wavFileHeader.chunkId[0] != 'R' || wavFileHeader.chunkId[1] != 'I' ||
		wavFileHeader.chunkId[2] != 'F' || wavFileHeader.chunkId[3] != 'F')
	{
		return false;
	}

	if (wavFileHeader.format[0] != 'W' || wavFileHeader.format[1] != 'A' ||
		wavFileHeader.format[2] != 'V' || wavFileHeader.format[3] != 'E')
	{
		return false;
	}

	if (wavFileHeader.subChunkId[0] != 'f' || wavFileHeader.subChunkId[1] != 'm' ||
		wavFileHeader.subChunkId[2] != 't' || wavFileHeader.subChunkId[3] != ' ')
	{
		return false;
	}

	if (wavFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	if (wavFileHeader.dataChunkId[0] != 'd' || wavFileHeader.dataChunkId[1] != 'a' ||
		wavFileHeader.dataChunkId[2] != 't' || wavFileHeader.dataChunkId[3] != 'a')
	{
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = wavFileHeader.sampleRate;
	waveFormat.wBitsPerSample = wavFileHeader.bitsPerSample;
	waveFormat.nChannels = wavFileHeader.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = wavFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = myPrimary->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	result = myDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeader), SEEK_SET);

	waveData = new unsigned char[wavFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	count = (unsigned int)fread(waveData, 1, wavFileHeader.dataSize, filePtr);
	if (count != wavFileHeader.dataSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	result = (*secondaryBuffer)->Lock(0, wavFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, wavFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	delete[] waveData;
	waveData = nullptr;

	return true;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8 **secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

bool Sound::PlayWaveFile()
{
	HRESULT result;
	LPDWORD myWord = 0;

	result = mySecondary->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	result = mySecondary->SetVolume(volume);
	if (FAILED(result))
	{
		return false;
	}

	result = mySecondary->Play(0, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

