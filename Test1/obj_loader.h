#ifndef obj_loader_h
#define obj_loader_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"

#include <iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
struct vertex
{
	glm::vec3 position;//位置
	glm::vec2 textCoords;//纹理坐标
	glm::vec3 normal;//法向量
};
struct CombinedIndex
{
	GLuint pos_index;
	GLuint textC_index;
	GLuint nor_index;
};
class Mesh 
{
public:
	void draw(Shader& shader)
	{
		shader.use();
		glBindVertexArray(this->VAOId);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, this->textureId);
		//glUniform1i(glGetUniformLocation(shader.ID, "tex"), 0);

		glDrawArrays(GL_TRIANGLES, 0, this->verData.size());

		glBindVertexArray(0);
		glUseProgram(0);
	}
	Mesh(){}
	Mesh(const vector<vertex>& vertData, GLint textureId)
	{
		this->verData = verData;
		this->textureId = textureId;
		this->setupMesh();
	}
	Mesh(const vector<vertex>&verData)
	{
		this->verData = verData;
		this->setupMesh();
	}
	~Mesh(){}
	void de_allocate()
	{
		glDeleteVertexArrays(1,&this->VAOId);
		glDeleteBuffers(1, &this->VBOId);
	}
private:
	vector<vertex> verData;
	GLuint VAOId, VBOId;// lightVAOId;
	GLint textureId;
	void setupMesh()
	{
		glGenVertexArrays(1, &this->VAOId);
		glGenBuffers(1, &this->VBOId);

		glBindVertexArray(this->VAOId);
		glBindBuffer(GL_ARRAY_BUFFER,this->VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*this->verData.size(), &this->verData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//glGenVertexArrays(1, &this->lightVAOId);
		//glBindVertexArray(this->lightVAOId);
		//glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		//glEnableVertexAttribArray(0);
	}
};
class Loader
{
public:
	static bool loadOBJ(const string path, vector<vertex>&vertData)
	{
		vector<CombinedIndex> vertComIndices;
		vector<glm::vec3> vert_temp;
		vector<glm::vec2> textC_temp;
		vector<glm::vec3> norm_temp;

		ifstream file(path);
		if (!file)
		{
			cerr << "Error:Failed to open the obj file at" << path << "." << endl;
			return false;
		}
		else
		{
			string line;
			while (getline(file, line))
			{
				if (line.substr(0, 2) == "vt")//顶点纹理
				{
					istringstream str(line.substr(2));
					glm::vec2 vt;
					str >> vt.x;
					str >> vt.y;
					//cout << "vt " << vt.x << " " << vt.y << endl;
					//vt.y =  - vt.y;
					textC_temp.push_back(vt);
				}
				else if (line.substr(0, 2) == "vn")//顶点法向量
				{
					istringstream str(line.substr(2));
					glm::vec3 vn;
					str >> vn.x;
					str >> vn.y;
					str >> vn.z;
					//cout << "vn " << vn.x << " " << vn.y << " " << vn.z << endl;
					norm_temp.push_back(vn);
				}
				else if (line.substr(0, 1) == "v")//顶点位置
				{
					istringstream str(line.substr(1));
					glm::vec3 v;
					str >> v.x;
					str >> v.y;
					str >> v.z;
					//cout << "v " << v.x << " " << v.y << " " << v.z << endl;
					vert_temp.push_back(v);
				}
				else if (line.substr(0, 1) == "f")//面
				{
					istringstream str(line.substr(2));
					string vtx;
					while (str >> vtx)
					{
						CombinedIndex vertComIdx;
						replace(vtx.begin(),vtx.end(), '/', ' ');
						istringstream ivtx(vtx);
						if (vtx.find("  ") != string::npos)
						{
							cerr << "Error:No texture data found in the file:" << path << endl;
							return false;
						}
						else
						{
							ivtx >> vertComIdx.pos_index >> vertComIdx.textC_index >> vertComIdx.nor_index;
							vertComIdx.pos_index--;
							vertComIdx.textC_index--;
							vertComIdx.nor_index--;
							vertComIndices.push_back(vertComIdx);
						}
					}
				}
			}
			for (vector<GLuint>::size_type i = 0; i < vertComIndices.size(); i++)
			{
				vertex v;
				CombinedIndex comIdx = vertComIndices[i];

				v.position = vert_temp[comIdx.pos_index];
				v.textCoords = textC_temp[comIdx.textC_index];
				v.normal = norm_temp[comIdx.nor_index];
				vertData.push_back(v);
			}
			return true;
		}
	}
};
#endif