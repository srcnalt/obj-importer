#include "Importer.h"
#include <string>

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

	int ImportObj(string path) {

	}
}
