#include <iostream>
#include <string>
#include "Compressor.h"
#include "Extractor.h"
using namespace std;

void main()
{
	int choice;
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
				cpr.compress();
				cout << "Compression done!" << endl;
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
				ext.extract();
				cout << "Extraction done!" << endl;
			}
			catch (const char* err)
			{
				cout << err << endl;
			}
		}
	}
	cin.get();
}