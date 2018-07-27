#include <string>

using namespace std;

#define DLL_API __declspec(dllexport) 

extern "C" {
	struct Sizes;
	struct Point3;
	struct Point2;
	DLL_API Sizes* GetSizes();

	DLL_API Point3* GetVertices();
	DLL_API Point3* GetNormals();
	DLL_API Point2* GetUVs();
	DLL_API int* GetFaces();

	DLL_API void ImportFile(const char *path);
}