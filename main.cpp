#include <iostream>
#include <string>
#include <time.h>
#include "Compressor.h"
#include "Extractor.h"
using namespace std;

void main()
{
	int choice;
	clock_t start, finish;
	while (1)
	{
		cout << "Compress file (0) or extrat file(1): ";
		cin >> choice;
		if (choice == 0)
		{
			string fileName, cprFileName;
			cout << "Input file's name: ";
			cin.ignore();
			getline(cin, fileName);
			cout << "Input compress file's name: ";
			getline(cin, cprFileName);
			Compressor cpr(fileName, cprFileName);
			try {
				start = clock();
				cpr.compress();
				finish = clock();
				cout << "Compression done! Time: " << finish - start << endl;
			}
			catch (const char* err)
			{
				cout << err << endl;
			}
		}
		else
		{
			string fileName;
			cout << "Input file's name: ";
			cin.ignore();
			getline(cin, fileName);
			Extractor ext(fileName);
			try {
				start = clock();
				ext.extract();
				finish = clock();
				cout << "Extraction done! Time: " << finish - start << endl;
			}
			catch (const char* err)
			{
				cout << err << endl;
			}
		}
	}
	cin.get();
}