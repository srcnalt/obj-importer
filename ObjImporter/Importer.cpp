#include "Importer.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <algorithm>

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

struct Sizes {
	int vSize;
	int nSize;
	int fSize;
	int uSize;

	inline Sizes() {
		vSize = 0;
		nSize = 0;
		fSize = 0;
		uSize = 0;
	}
};

string thePath;
string objName;
string matName;
string texName;
vector<Point3> vertices;
vector<Point3> normals;
vector<Point2> uvs;
vector<int> triangles;
vector<int> faceIndexes;
vector<Point3> faces;

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
	
	thePath = path.substr(0, path.find_last_of("\\/"));
	objName = path.substr(path.find_last_of("\\/") + 1);

	string line;
	ifstream file(path);

	while (getline(file, line)) {
		
		if (validateLine(line)) {
			vector<string> pieces = split(line, space);

			if (pieces[0] == "mtllib") {
				matName = pieces[1];
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
				faceIndexes.clear();
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
					faces.push_back(temp);
					faceIndexes.push_back(f);

					j++;
					f++;
				}

				j = 1;

				while (j + 2 < pieces.size()) {
					triangles.push_back(faceIndexes[0]);
					triangles.push_back(faceIndexes[j]);
					triangles.push_back(faceIndexes[j + 1]);
					j++;
				}
			}
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		vertices.push_back(vx[(int)faces[i].x - 1]);

		if (faces[i].y >= 1) uvs.push_back(uv[(int)faces[i].y - 1]);
		if (faces[i].z >= 1) normals.push_back(nr[(int)faces[i].z - 1]);
	}
}

extern "C" {
	void ImportFile(const char *path)
	{
		string filePath(path);

		CreateMesh(filePath);

		//return mesh;
	}

	Sizes* GetSizes() {
		sizes->vSize = vertices.size();
		sizes->nSize = normals.size();
		sizes->fSize = faces.size();
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

	int* GetFaces() {
		return &triangles[0];
	}
}
