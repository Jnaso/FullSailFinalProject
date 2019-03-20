#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_

/***************************\

		Sound Class:
	 Every sound effect in
	 the game will need an
	 instance of this class.
	 (as of now, only .wav is supported)

	 Josh Naso
	 3/20/19

\***************************/

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class Sound
{
private:
	//Structure for containing all the sound file's data
	struct WaveHeader
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	//Objects for creating and playing the sound files
	IDirectSound8* myDirectSound;
	IDirectSoundBuffer *myPrimary;
	IDirectSoundBuffer8 *mySecondary;
	char* soundFile;

	bool InitializeDirectSound(HWND window);
	void ShutdownDirectSound();
	bool LoadWaveFile(const char *, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

public:
	Sound(char *filename);

	bool Initialize(HWND window);
	void Shutdown();
	bool PlayWaveFile();
};

#endif // !_SOUND_H_
