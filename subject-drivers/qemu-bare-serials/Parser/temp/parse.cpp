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
					if (wordCount>= 5 && wordCount<=(sLength-2))
					{
						parsedFile << line << " ";
					}
					
				}
				wordCount++;

			}
			parsedFile << '\n';
			lineCount++;
		}
		else lineCount = 0;
	
	}

	
}
