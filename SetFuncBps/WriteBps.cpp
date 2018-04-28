#include <istream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		cout << "Incorrect number of arguments" << endl;
		return -1;
	}

	//cout << argv[1] << endl;
	ifstream symNameFile;
	symNameFile.open(argv[1], ios::in);
	if(symNameFile.fail())
	{
		cout << "symbol file failed" << endl;
		return -1;
	}

	ofstream bpFile;
	bpFile.open(argv[2], ios::out);
	if(bpFile.fail())
        {
                cout << "bp file failed" << endl;
                return -1;
        }

	string line;
	string bpCommand;
	while(symNameFile.good())
	{
		getline(symNameFile, line);
		if(line != "")
		{
			bpCommand = "break " + line;
			bpFile << bpCommand << '\n';
		}
	}

	symNameFile.close();
	bpFile.close();

	return 0;
}
