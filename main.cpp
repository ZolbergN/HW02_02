#include <boost/filesystem.hpp>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;

string getFileName(string address)
{
	fs::path filePath = address;
	fs::path textFilename = filePath.filename();
	return textFilename.string();
}

double getFileSize(string address)
{
	fs::path filePath = address;
	return fs::file_size(filePath);
}

string getFileDate(string address)
{
	fs::path filePath = address;
	time_t ftime = fs::last_write_time(filePath);
	return asctime(gmtime(&ftime));
}

void Get(string address)
{

	if (fs::is_directory(address))
		throw runtime_error("Type of file - directory");

	if (!fs::exists(address))
		throw runtime_error("File does not find!");

	future<string>futureFileName = async(getFileName, address);
	future<double>futureFileSize = async(getFileSize, address);
	future<string>futureFileDate = async(getFileDate, address);

	string FileName = futureFileName.get();
	double FileSize = futureFileSize.get();
	string FileDate = futureFileDate.get();
	
	cout << "Название файла:\t " << FileName << endl;
	cout << "Размер:\t " << FileSize << " byte" << endl;
	cout << "Modification date:\t " << FileDate << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");
	try
	{
		string add;
		cout << "Укажите путь к файлу" << endl;
		cin >> add;

		Get(add);
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}
