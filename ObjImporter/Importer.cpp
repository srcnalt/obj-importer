#include "Importer.h"
#include <string>
#include <vector>

using namespace std;

extern "C" {
	struct Vector3
	{
		float x, y, z;

		inline Vector3(void) {}

		inline Vector3(const float x, const float y, const float z)
		{
			this->x = x;
			this->y = y;
			this->y = z;
		}

		//sum with another Vector3
		inline Vector3 operator + (const Vector3& V) const
		{
			return Vector3(x + V.x, y + V.y, z + V.z);
		}

		//sum with float
		inline Vector3 operator + (const float V) const
		{
			return Vector3(x + V, y + V, z + V);
		}
	};

	struct Vector2
	{
		float x, y;

		inline Vector2(void) {}

		inline Vector2(const float x, const float y)
		{
			this->x = x;
			this->y = y;
		}

		//sum with another Vector2
		inline Vector2 operator + (const Vector2& V) const
		{
			return Vector2(x + V.x, y + V.y);
		}

		//sum with float
		inline Vector2 operator + (const float V) const
		{
			return Vector2(x + V, y + V);
		}
	};

	struct MeshStruct
	{
		string name;
		string fileName;
		vector<Vector3> vertices;
		vector<Vector3>normals;
		vector<Vector3> faces;
		vector<Vector2> uv;
		vector<Vector2> uv1;
		vector<Vector2> uv2;
		vector<int> triangles;
		vector<int> faceVerts;
		vector<int> faceUVs;
	};

	int ImportObj(string path) {

	}
}
