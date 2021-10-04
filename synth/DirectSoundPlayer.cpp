#include "DirectSoundPlayer.h"
#include "Util.h"
#include <cstdio>

using namespace OmedaPlayer;

DirectSoundPlayer::DirectSoundPlayer(SongPlayer *player)
{
	this->player = player;
	isPlaying = false;

	InitializeCriticalSection(&criticalSection);
	thread = CreateThread(nullptr, 0, threadProc, (LPVOID)this, 0, nullptr);
	SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
}

DirectSoundPlayer::~DirectSoundPlayer()
{
	if (thread)
	{
		TerminateThread(thread, 0);
		DeleteCriticalSection(&criticalSection);
	}
}

void DirectSoundPlayer::StartPlaying()
{
	startTime = timeGetTime();
	EnterCriticalSection(&criticalSection);
	isPlaying = true;
	LeaveCriticalSection(&criticalSection);
}

void DirectSoundPlayer::StopPlaying()
{
	EnterCriticalSection(&criticalSection);
	isPlaying = false;
	LeaveCriticalSection(&criticalSection);
}

bool DirectSoundPlayer::getIsPlaying()
{
	EnterCriticalSection(&criticalSection);
	auto val = isPlaying;
	LeaveCriticalSection(&criticalSection);
	return val;
}

void DirectSoundPlayer::setStartTime(DWORD val) {
	EnterCriticalSection(&criticalSection);
	startTime = val;
	LeaveCriticalSection(&criticalSection);
}

void DirectSoundPlayer::Wait() const
{
	WaitForSingleObject(thread, INFINITE);
}

double DirectSoundPlayer::GetPlayTime() {
	EnterCriticalSection(&criticalSection);
	auto val = (timeGetTime() - startTime) / 1000.0;
	LeaveCriticalSection(&criticalSection);
	return val;
}

void DirectSoundPlayer::writeData(LPDIRECTSOUNDBUFFER buffer, int writePos, int chunkSize) const
{
	short *chunk1Start, *chunk2Start;
	DWORD chunk1Size, chunk2Size;

	buffer->Lock(writePos, chunkSize, (void**)&chunk1Start, &chunk1Size, (void**)&chunk2Start, &chunk2Size, 0);

	auto frame1Count = chunk1Size / sizeof(short) / 2;
	player->Render((short(*)[2])chunk1Start, frame1Count);

	if (chunk2Start != nullptr)
	{
		auto frame2Count = chunk2Size / sizeof(short) / 2;
		player->Render((short(*)[2])chunk2Start, frame2Count);
	}

	buffer->Unlock(chunk1Start, chunk1Size, chunk2Start, chunk2Size);
}

DWORD DirectSoundPlayer::threadProc(LPVOID lpParameter)
{
	auto player = (DirectSoundPlayer*)lpParameter;

	LPDIRECTSOUND8 device;
	DirectSoundCreate8(nullptr, &device, nullptr);
	device->SetCooperativeLevel(GetForegroundWindow(), DSSCL_NORMAL);

	auto channelNumber = 2;
	auto bitsPerSample = 16;
	auto cbSize = 8;

	WAVEFORMATEX waveFormat;
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = channelNumber;
	waveFormat.nSamplesPerSec = OmedaCommon::Util::CurrentSampleRate;
	waveFormat.wBitsPerSample = bitsPerSample;
	waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = cbSize;

	// create primary buffer
	DSBUFFERDESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(DSBUFFERDESC));
	bufferDesc.dwSize = sizeof bufferDesc;
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

	LPDIRECTSOUNDBUFFER primaryBuffer;
	device->CreateSoundBuffer(&bufferDesc, &primaryBuffer, nullptr);
	primaryBuffer->SetFormat(&waveFormat);

	// create secondary buffer
	bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;
	bufferDesc.dwBufferBytes = 2 * waveFormat.nAvgBytesPerSec;
	bufferDesc.lpwfxFormat = &waveFormat;
	LPDIRECTSOUNDBUFFER secondaryBuffer;
	device->CreateSoundBuffer(&bufferDesc, &secondaryBuffer, nullptr);

	// treat buffer as split (two buffers with length of 1 sec)
	auto bufferCenter = waveFormat.nAvgBytesPerSec;
	player->writeData(secondaryBuffer, 0, bufferCenter);

	auto timer = CreateWaitableTimer(nullptr, false, nullptr);
	LARGE_INTEGER dueTime;
	dueTime.QuadPart = 0;
	SetWaitableTimer(timer, &dueTime, 500, nullptr, nullptr, true);

	auto firstBufferFilled = true;
	auto isPlaying = false;

	while (true)
	{
		WaitForSingleObject(timer, INFINITE);

		auto isPlayerPlaying = player->getIsPlaying();
		if (isPlayerPlaying)
		{
			DWORD playCursor;
			DWORD writeCursor;
			secondaryBuffer->GetCurrentPosition(&playCursor, &writeCursor);

			if (firstBufferFilled && playCursor < bufferCenter)
			{
				player->writeData(secondaryBuffer, bufferCenter, bufferCenter);
				firstBufferFilled = false;
			}
			else if (!firstBufferFilled && playCursor >= bufferCenter)
			{
				player->writeData(secondaryBuffer, 0, bufferCenter);
				firstBufferFilled = true;
			}
		}

		if (isPlayerPlaying != isPlaying)
		{
			if (isPlayerPlaying) {
				player->setStartTime(timeGetTime());
				secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
			}
			else secondaryBuffer->Stop();
			isPlaying = isPlayerPlaying;
		}
	}
}
