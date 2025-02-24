#include "hfpch.h"

namespace HFR {

	Shader::Shader(const std::string& FolderPath, const std::string& VertexFilePath, const std::string& FragmentFilePath) {
		vertexShader = Util::readAsString(FolderPath + VertexFilePath, HFR_BINARY_READ);
		fragmentShader = Util::readAsString(FolderPath + FragmentFilePath, HFR_BINARY_READ);
		vertexName = VertexFilePath;
		fragmentName = FragmentFilePath;

		vertexID = 0;
		fragmentID = 0;
		programID = 0;
	}

	Shader::~Shader() {
		close();
	}

	void Shader::create() {
		programID = glCreateProgram();
		vertexID = glCreateShader(GL_VERTEX_SHADER);
		fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		Util::compileShader(vertexID, vertexShader, vertexName);
		Util::compileShader(fragmentID, fragmentShader, fragmentName);

		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);

		Util::linkAndValidateProgram(programID);
	}

	void Shader::bind() const{
		glUseProgram(programID);
	}

	void Shader::unbind() const{
		glUseProgram(0);
	}

	unsigned int Shader::getUniformLocation(const char* name) const{
		return glGetUniformLocation(programID, name);
	}

	unsigned int Shader::getAttributeLocation(const char* name) const{
		return glGetAttribLocation(programID, name);
	}

	void Shader::setUniform(const char* name, Mat4f& data) const{
		FloatBuffer* matbuffer = new FloatBuffer(16);
		data.store(matbuffer);
		glUniformMatrix4fv(getUniformLocation(name), 1, false, matbuffer->data);
		delete matbuffer;
	}

	void Shader::setUniform(const char* name, const Vec2f& data) const{
		glUniform2f(getUniformLocation(name), data.x, data.y);
	}

	void Shader::setUniform(const char* name, const Vec3f& data) const{
		glUniform3f(getUniformLocation(name), data.x, data.y, data.z);
	}

	void Shader::setUniform(const char* name, const Vec4f& data) const{
		glUniform4f(getUniformLocation(name), data.x, data.y, data.z, data.w);
	}

	void Shader::setUniform(const char* name, const bool& data) const{
		glUniform1f(getUniformLocation(name), data ? (GLfloat)1 : (GLfloat)0);
	}

	void Shader::setUniform(const char* name, const float& data) const{
		glUniform1f(getUniformLocation(name), data);
	}

	void Shader::setUniform(const char* name, const int& data) const{
		glUniform1i(getUniformLocation(name), data);
	}

	void Shader::bindLightMaps(const Material& material) const{

		//colors
		setUniform("material.diffuseColor", material.diffuseColor);
		setUniform("material.ambientColor", material.ambientColor);
		setUniform("material.specularColor", material.specularColor);
		setUniform("material.specularExponent", material.specularExponent);
		setUniform("material.emissionColor", material.emissionColor);
		//shader.setUniform("material.ambientIntensity", ambientIntensity);

		//maps
		if (material.diffuseMap.image.hasData()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material.diffuseMap.textureID);
			setUniform("material.diffuseMap", (int)0);
			setUniform("material.useDiffuseMap", true);
		}
		else
			setUniform("material.useDiffuseMap", false);

		if (material.ambientMap.image.hasData()) {
			glActiveTexture(GL_TEXTURE1);
			setUniform("material.ambientMap", (int)1);
			setUniform("material.useAmbientMap", true);
			glBindTexture(GL_TEXTURE_2D, material.ambientMap.textureID);
		}
		else
			setUniform("material.useAmbientMap", false);

		if (material.specularMap.image.hasData()) {
			glActiveTexture(GL_TEXTURE2);
			setUniform("material.specularMap", (int)2);
			setUniform("material.useSpecularMap", true);
			glBindTexture(GL_TEXTURE_2D, material.specularMap.textureID);
		}
		else
			setUniform("material.useSpecularMap", false);

		if (material.emissionMap.image.hasData()) {
			glActiveTexture(GL_TEXTURE3);
			setUniform("material.emissionMap", (int)3);
			setUniform("material.useEmissionMap", true);
			glBindTexture(GL_TEXTURE_2D, material.emissionMap.textureID);
		}
		else
			setUniform("material.useEmissionMap", false);

	}

	void Shader::close() {
		glDetachShader(programID, vertexID);
		glDetachShader(programID, fragmentID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteShader(programID);
	}

}
