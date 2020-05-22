// researchTest1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;




int CountWords(const char* str)
{
	if (str == NULL)
		return 0;  // return 0 if string is empty

	bool inSpaces = true;
	int numWords = 0;

	while (*str != '\0')
	{
		if (std::isspace(*str))
		{
			inSpaces = true;
		}
		else if (inSpaces)
		{
			numWords++;
			inSpaces = false;
		}

		++str;
	}

	return numWords;
}

int main(int agrc, char* argv[])
{
	if (agrc != 3)
	{
		cout << "Incorrect number of arguments" << endl;

	}
	ifstream gdbTextFile;
	gdbTextFile.open(argv[1], ios::in);
	if (gdbTextFile.fail())
	{
		cout << "Could not open gdb.txt" << endl;
		return -1;
	}
	ofstream parsedFile;
	parsedFile.open(argv[2], ios::out);
	if (parsedFile.fail())
	{
		cout << "Parsed file failed" << endl;
		return -1;
	}

	string line;
	
	int lineCount = 0;
	while (gdbTextFile.good())
	{
		
		getline(gdbTextFile, line);
		if (line != "")
		{
			int sLength = CountWords(line.c_str());
			stringstream temp(line);
			int wordCount=1;

			while (getline(temp, line, ' '))
			{
				
				if (lineCount==0)
				{
					if ((wordCount>= 6 && wordCount<= sLength-2) || wordCount==sLength)
					{
						parsedFile << line << " ";
						if (wordCount == sLength)
							parsedFile << '\n';
					}
					
				}
				else if (lineCount == 1)
				{
					parsedFile << line << " ";
					if (wordCount == sLength)
						parsedFile << '\n';
				}
				else
				{
					parsedFile << line << " ";
				}

				wordCount++;

			}
			
			lineCount++;
		}
		else lineCount = 0; parsedFile << '\n';
	
	}

	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
