#define DLL_API __declspec(dllexport) 

extern "C"
{
	struct Sizes;
	struct Point3;
	struct Point2;

	DLL_API Sizes* GetSizes();
	DLL_API Point3* GetVertices();
	DLL_API Point3* GetNormals();
	DLL_API Point2* GetUVs();
	DLL_API int* GetTris();
	DLL_API void ImportObj(const char *path);

	DLL_API void ImportMat();
	DLL_API Point3* GetColor();
	DLL_API float GetGloss();
	DLL_API const char* GetTexture();
}