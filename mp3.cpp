#include "lameHelper.h"
#include <string>

using namespace std;

int main()
{
	lameHelper lhHandle;
	char* wav = (char*)"D:\\music\wav\file_example_WAV_10MG.wav";
	char* mp3 = (char*)"D:\\music\wav\file_example_WAV_10MG.mp3";
	lhHandle.encode(wav, mp3);

	return 0;
}