#include<GL/glew.h>
#include "Model3D.h"
using namespace std;


const void Model3D::draw() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambiRGBA);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRGBA);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specRGBA); 

	glMaterialfv(GL_FRONT, GL_EMISSION, emisRGBA);

	glMaterialf(GL_FRONT, GL_SHININESS, 64);


	glBindBuffer(GL_ARRAY_BUFFER, buffers->buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers->buffers[1]);
	glNormalPointer(GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers->buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);


	glDrawArrays(GL_TRIANGLES, 0, buffers->size);

	glBindTexture(GL_TEXTURE_2D, texture->texture); 
}

Model3D::Model3D() {
}

Model3D::~Model3D() {
}


ModelBuffers::~ModelBuffers() {
	glDeleteBuffers(3, buffers);
}

int ModelBuffers::parse(const char* filename) {
	string line;
	ifstream file;
	file.open(filename);

	if (file.is_open()) {
		getline(file, line);
		int count = stoi(line);
		float *vertexB = new float[count * 3];
		float *normalB = new float[count * 3];
		float *texturB = new float[count * 2];
		int t = 0;
		for (int i = 0; getline(file, line); i++) {

			const string delimiter = " ";

			size_t pos = 0;
			std::string token;

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			vertexB[i * 3] = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			vertexB[i * 3 + 1] = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			vertexB[i * 3 + 2] = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			normalB[i * 3] = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			normalB[i * 3 + 1] = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			normalB[i * 3 + 2] = stof(token);
			line.erase(0, pos + delimiter.length());


			pos = line.find(delimiter);
			token = line.substr(0, pos);
			texturB[t++] = stof(token);
			line.erase(0, pos + delimiter.length());

			texturB[t++] = stof(line);
			
			size++;
		}
		file.close();

		glGenBuffers(3, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, vertexB, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, normalB, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 2, texturB, GL_STATIC_DRAW);
		
		delete[] vertexB;
	}
	else fprintf(stderr, "%s: %s\n", strerror(errno), filename);


	file.close();

	return 0;
}



int Texture::parse(const char* filename) {

	unsigned int t, tw, th;
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)filename);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	return 0;
}

