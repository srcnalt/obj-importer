#include "ObjLoader.h"
#include "Structures.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

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

struct Info {
	string thePath;
	string objName;
	string matName;
	string texName;
} info;

struct Sizes {
	int vSize;
	int nSize;
	int tSize;
	int uSize;

	inline Sizes() {
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
const char* texture;

Sizes *sizes = new Sizes();

bool validateLine(string str) {
	return	str.find("f ") == 0 || str.find("v ") == 0 || str.find("vt ") == 0 || str.find("vn ") == 0 || str.find("mtllib ") == 0;
}

vector<string> split(string str, char delimineter) {
	vector<string> pieces;
	stringstream stream(str);
	string token;

	while (getline(stream, token, delimineter)) {
		pieces.push_back(token);
	}

	return pieces;
}

void CreateMesh(string path)
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

	while (getline(file, line)) {
		
		if (validateLine(line)) {
			vector<string> pieces = split(line, space);

			if (pieces[0] == "mtllib") {
				info.matName = pieces[1];
				continue;
			}
			
			if (pieces[0] == "v") {
				vx.push_back(Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3])));
				continue;
			}

			if (pieces[0] == "vt") {
				uv.push_back(Point2(stof(pieces[1]), stof(pieces[2])));
				continue;
			}

			if (pieces[0] == "vn") {
				nr.push_back(Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3])));
				continue;
			}

			if (pieces[0] == "f") {
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

				while (j + 2 < pieces.size()) {
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

		if (fc[i].y >= 1) uvs.push_back(uv[(int)fc[i].y - 1]);
		if (fc[i].z >= 1) normals.push_back(nr[(int)fc[i].z - 1]);
	}
}

void CreateMat() {
	char space = ' ';

	string line;
	ifstream file(info.thePath + '/' + info.matName);

	while (getline(file, line)) {
		vector<string> pieces = split(line, space);

		if (pieces.size() < 1) continue;

		if (pieces[0] == "newmtl") {
			info.matName = pieces[1];
			continue;
		}

		if (pieces[0] == "Kd") {
			albedo[0] = Point3(stof(pieces[1]), stof(pieces[2]), stof(pieces[3]));
			continue;
		}

		if (pieces[0] == "Ks") {
			smoothness = (stof(pieces[1]), stof(pieces[2]), stof(pieces[3])) / 3;
			continue;
		}

		if (pieces[0] == "map_Kd") {
			string fullPath = info.thePath + '/' + pieces[1];
			texture = fullPath.c_str();
		}
	}
}

void Initialize() {
	vertices.clear();
	normals.clear();
	tris.clear();
	uvs.clear();
}

extern "C" {
	void ImportObj(const char *path) {
		string filePath(path);
		Initialize();
		CreateMesh(filePath);
	}

	Sizes* GetSizes() {
		sizes->vSize = vertices.size();
		sizes->nSize = normals.size();
		sizes->tSize = tris.size();
		sizes->uSize = uvs.size();

		return sizes;
	}

	Point3* GetVertices() {
		return &vertices[0];
	}

	Point3* GetNormals() {
		return &normals[0];
	}

	Point2* GetUVs() {
		return &uvs[0];
	}

	int* GetTris() {
		return &tris[0];
	}

	void ImportMat() {
		CreateMat();
	}

	Point3* GetColor() {
		return albedo;
	}

	float GetGloss() {
		return smoothness;
	}

	const char* GetTexture() {
		return texture;
	}
}