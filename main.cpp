#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <future>


using namespace std;
namespace fs = boost::filesystem;

string EXPLORER_LOG_PATH = "log.txt";

//Подсчет размера директории
double getDirSize(string dirAddress, double size) {
    fs::path dirPath = dirAddress;
    fs::path textDir = dirPath.filename();
    cout << "Finding size of " << textDir << ". Please, wait...\n";
    if(fs::exists(dirPath)){
      fs::directory_iterator end_itr;
      for(fs::directory_iterator dirIte(dirPath); dirIte != end_itr; ++dirIte) {
        fs::path filePath(dirIte->path());
        try {
          if(!fs::is_directory(dirIte->status()))
          {
            size = size + fs::file_size(filePath);
          }
          else {
            getDirSize(filePath.string(), size);
          }
        }
        catch(exception& e){
          cout << e.what() << endl;
        }
        //Проверка на то, является ли файл, размер которого мы хотим узнать, директорией
      }
    }
    return size;
}

string getDirName(string dirAddress) {
    cout << "Finding name of file or directory. Please, wait...\n" << endl;
    fs::path dirPath = dirAddress;
    fs::path textDir = dirPath.filename();
    return textDir.string();
}

string getDirData(string dirAddress) {
    cout << "Finding latest modification data. Please, wait...\n" << endl;
    fs::path dirPath = dirAddress;
    std::time_t ftime = fs::last_write_time(dirPath);
    return asctime(gmtime(&ftime));
}

void displayDir(string dirAddress) {
    //Проверка на существование директории
    if(!fs::exists(dirAddress)) {
      throw runtime_error("ERROR: We don't have such directory");
    }
    //Проверка на то, является ли файл директорией
    if(!fs::is_directory(dirAddress)) {
      throw runtime_error("ERROR: Type of file - not directory");
    }

    std::future<string> futureDirName = std::async (getDirName,dirAddress);
    std::future<string> futureDirData = std::async (getDirData,dirAddress);
    std::future<double> futureDirSize = std::async (getDirSize,dirAddress, 0);

    string dirName = futureDirName.get();
    string dirData = futureDirData.get();
    double dirSize = futureDirSize.get();

    //Запись данных о директории в файл
    ofstream fout(EXPLORER_LOG_PATH);
    fout << "Directory name: " << dirName << endl;
    fout << "Latest modification: " << dirData << endl;
    fout << "Directory size: " << dirSize/1000 << " Kb" << endl;
    fout.close();

}


int main(int argc, char* argv[])
{
  printf("%d \n", argc);
  string ad;
  ad = argv[1];

  try {
    displayDir(ad);
  }
  catch(const exception& e) {
    cout << e.what() << endl;
  }
}
