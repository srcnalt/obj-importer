/*
*	ObjLoader by Sercan Altundas 2018
*
*	This small Dll file helps you acquire some of the attributes of an .obj file
*	The information that can be extracted is limited by faces, vertices, UVs and normals of a model.
*	
*	If the .obj file cntains a material the script can also extract .mtl information.
*	For now only one material is taken from the .mtl file and the others are ignore.
*	The information that can be extracted is limited by color and glossiness.
*	These two values can match to Unity standart shaders albedo and smoothness values.
*
*	The Dll does not support all attributes of an .obj or .mtl file.
*	It is best the Dll is used on single mesh model with single texture and material.
*
*/

#include "ObjLoader.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Point3
{
	float x;
	float y;
	float z;

	inline Point3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};

	inline Point3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Point2
{
	float x;
	float y;

	inline Point2()
	{
		x = 0.0f;
		y = 0.0f;
	};

	inline Point2(const float x, const float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Info
{
	string thePath;
	string objName;
	string matName;
	string texName;
} info;

struct Sizes
{
	int vSize;
	int nSize;
	int tSize;
	int uSize;

	inline Sizes()
	{
		vSize = 0;
		nSize = 0;
		tSize = 0;
		uSize = 0;
	}
};

vector<Point3> vertices;
vector<Point3> normals;
vector<Point2> uvs;
vector<int> tris;

Point3* albedo = (Point3*)malloc(sizeof(Point3));
float smoothness;
string texture;

Sizes *sizes = new Sizes();

bool validateObjLine(string str)
{
	return	str.find("f ") == 0 ||
			str.find("v ") == 0 ||
			str.find("vt ") == 0 ||
			str.find("vn ") == 0 ||
			str.find("mtllib ") == 0;
}

bool validateMatLine(string str)
{
	return	str.find("Kd ") == 0 ||
			str.find("Ks ") == 0 ||
			str.find("newmtl ") == 0 ||
			str.find("map_Kd ") == 0;
}

vector<string> split(string str, char delimineter)
{
	vector<string> pieces;
	stringstream stream(str);
	string token;

	while (getline(stream, token, delimineter))
	{
		pieces.push_back(token);
	}

	return pieces;
}

bool CreateMesh(string path)
{
	char space = ' ';
	char slash = '/';

	int f = 0;

	vector<Point3> vx;
	vector<Point2> uv;
	vector<Point3> nr;
	vector<Point3> fc;
	vector<int> fi;
	
	info.thePath = path.substr(0, path.find_last_of("\\/"));
	info.objName = path.substr(path.find_last_of("\\/") + 1);

	string line;
	ifstream file(path);

	if (!file.good())
	{
		return false;
	}

	while (getline(file, line))
	{
		
		if (validateObjLine(line))
		{
			vector<string> pieces = split(line, space);

			if (pieces[0] == "mtllib") {
				info.matName = pieces[1];
				continue;
			}
			
			if (pieces[0] == "v")
			{
				vx.push_back(Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3])));
				continue;
			}

			if (pieces[0] == "vt")
			{
				uv.push_back(Point2(stof(pieces[1]), stof(pieces[2])));
				continue;
			}

			if (pieces[0] == "vn")
			{
				nr.push_back(Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3])));
				continue;
			}

			if (pieces[0] == "f")
			{
				int j = 1;
				fi.clear();

				while (j < pieces.size() && 0 < pieces[j].size())
				{
					Point3 temp;
					vector<string> facePieces = split(pieces[j], slash);
					temp.x = stoi(facePieces[0]);

					if (facePieces.size() > 1)
					{
						temp.y = facePieces[1] != "" ? stoi(facePieces[1]) : 0;
						temp.z = stoi(facePieces[2]);
					}

					fc.push_back(temp);
					fi.push_back(f);

					j++;
					f++;
				}

				j = 1;

				while (j + 2 < pieces.size())
				{
					tris.push_back(fi[0]);
					tris.push_back(fi[j]);
					tris.push_back(fi[j + 1]);

					j++;
				}
			}
		}
	}

	for (int i = 0; i < fc.size(); i++)
	{
		vertices.push_back(vx[(int)fc[i].x - 1]);

		if (fc[i].y >= 1) {
			uvs.push_back(uv[(int)fc[i].y - 1]);
		}

		if (fc[i].z >= 1)
		{
			normals.push_back(nr[(int)fc[i].z - 1]);
		}
	}
}

bool CreateMat()
{
	if (info.matName == "") false;

	char space = ' ';

	string line;
	ifstream file(info.thePath + '/' + info.matName);

	if (!file.good())
	{
		return false;
	}

	while (getline(file, line))
	{
		if (validateMatLine(line))
		{
			vector<string> pieces = split(line, space);

			if (pieces[0] == "newmtl")
			{
				info.matName = pieces[1];
				continue;
			}

			if (pieces[0] == "Kd")
			{
				albedo[0] = Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3]));
				continue;
			}

			if (pieces[0] == "Ks")
			{
				smoothness = (stof(pieces[1]), stof(pieces[2]), stof(pieces[3])) / 3;
				continue;
			}

			if (pieces[0] == "map_Kd")
			{
				texture = info.thePath + '/' + pieces[1];
			}
		}
	}
}

void Initialize()
{
	vertices.clear();
	normals.clear();
	tris.clear();
	uvs.clear();
}

extern "C"
{
	bool ImportObj(const char *path)
	{
		string filePath(path);
		Initialize();
		return CreateMesh(filePath);
	}

	Sizes* GetSizes()
	{
		sizes->vSize = vertices.size();
		sizes->nSize = normals.size();
		sizes->tSize = tris.size();
		sizes->uSize = uvs.size();

		return sizes;
	}

	Point3* GetVertices()
	{
		return &vertices[0];
	}

	Point3* GetNormals()
	{
		return &normals[0];
	}

	Point2* GetUVs()
	{
		return &uvs[0];
	}

	int* GetTris()
	{
		return &tris[0];
	}

	bool ImportMat()
	{
		return CreateMat();
	}

	Point3* GetColor()
	{
		return albedo;
	}

	float GetGloss()
	{
		return smoothness;
	}

	const char* GetTexture()
	{
		return &texture[0];
	}
}