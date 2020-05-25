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
	
	//disable pagination so we can run the whole thing without stopping
	bpFile << "set pagination off\n";

	string line;
	string bpCommand;
	int bpCount = 0;
	while(symNameFile.good())
	{
		getline(symNameFile, line);
		if(line != "")
		{
			bpCommand = "break " + line;
			bpFile << bpCommand << '\n';
			bpCount++;
		}
	}

	for(int i = 0; i < bpCount; i++)
	{
		bpFile << "info locals" << '\n'
			<< "info args" << '\n'
			<< '\n' //Python script looks for two new lines in a row to distinguish breakpoint ends.
			<< "shell python breakPoint_parser.py"     << '\n' //run the python script at each breakpoint
			<< "source changeVars.gdb"      << '\n' //source the .gdb script produced from the python script at each breakpoint. This changes the local variables/arguments at each breakpoint
			<< "continue" << '\n';
	}

	//bpFile << "quit" << '\n';

	symNameFile.close();
	bpFile.close();

	return 0;
}
