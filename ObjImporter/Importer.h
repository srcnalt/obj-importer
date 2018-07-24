#include <string>

using namespace std;

#define DLL_API __declspec(dllexport) 

extern "C" {
	DLL_API int ImportObj(string path);
}