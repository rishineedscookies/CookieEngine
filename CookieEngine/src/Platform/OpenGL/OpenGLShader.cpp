#include "ckpch.h"
#include "OpenGLShader.h"

#include <fstream>

namespace Cookie {

	static GLenum ShaderTypeFromSource(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string shaderStr = ReadFile(path);
		auto shaderSources = Preproccess(shaderStr);
		Compile(shaderSources);
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = (lastDot == std::string::npos ? path.size() : lastDot) - lastSlash;
		m_Name = path.substr(lastSlash, count);
		 
	}

	OpenGLShader::OpenGLShader(const std::string& path, const std::string& name)
	{
		std::string shaderStr = ReadFile(path);
		auto shaderSources = Preproccess(shaderStr);
		Compile(shaderSources);
		m_Name = name;
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
		m_Name = name;
	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformBool(const std::string& name, const bool value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1fv(location, 1, &(value));
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const mathfu::vec2& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1, &(value[0]));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const mathfu::vec3& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, &(value[0]));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const mathfu::vec4& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(location, 1, &(value[0]));
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const mathfu::mat2& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, &(value[0]));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const mathfu::mat3& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &(value[0]));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const mathfu::mat4& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0]));
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			CK_CORE_ERROR("Could not open file '{0}'", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::Preproccess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			CK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			CK_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromSource(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? -1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> sources)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIds;
		int idIndex = 0;
		for (auto& kv : sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				CK_CORE_ERROR("{0}", infoLog.data());
				CK_CORE_ASSERT(false, "Shader compilation failure!");
				return;
			}

			glAttachShader(program, shader);
			glShaderIds[idIndex] = shader;
			idIndex++;
		}
		
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIds)
				glDeleteShader(id);

			CK_CORE_ERROR("{0}", infoLog.data());
			CK_CORE_ASSERT(false, "Shader linking failure!")
				return;
		}

		// Always detach shaders after a successful link.

		for (auto id : glShaderIds)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

}