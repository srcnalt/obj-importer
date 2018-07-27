struct Point3
{
	float x, y, z = 0;

	inline Point3() {};

	inline Point3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Point2
{
	float x, y = 0;

	inline Point2() {};

	inline Point2(const float x, const float y)
	{
		this->x = x;
		this->y = y;
	}
};