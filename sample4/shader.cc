#include <shader.h>

Shader::Shader(const char *vertex_shader, const char *frag_shader)
{
   std::string vertextCode;
   std::string fragCode;
   std::ifstream vShaderFile;
   std::ifstream fShaderFile;
   // 保证ifstream对象可以抛出异常：
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

   try
   {
      vShaderFile.open(vertex_shader);
      fShaderFile.open(frag_shader);
      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      vShaderFile.close();
      fShaderFile.close();
      vertextCode = vShaderStream.str();
      fragCode = fShaderStream.str();
   }
   catch (std::ifstream::failure e)
   {
      std::cerr << e.what() << '\n';
   }
   const char *vShaderCode = vertextCode.c_str();
   const char *fShaderCode = fragCode.c_str();
   GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vShader, 1, &vShaderCode, NULL);
   glCompileShader(vShader);

   int success;
   char infoLog[512];
   glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(vShader, 512, NULL, infoLog);
      std::cout << "error: vtx shader compile failed:" << infoLog << std::endl;
   }

   GLuint fShader;
   fShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fShader, 1, &fShaderCode, NULL);
   glCompileShader(fShader);
   glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fShader, 512, NULL, infoLog);
      std::cout << "error: frag shader compile failed:" << infoLog << std::endl;
   }

   mProgram = glCreateProgram();
   glAttachShader(mProgram, vShader);
   glAttachShader(mProgram, fShader);
   glLinkProgram(mProgram);

   glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
      std::cout << "link error:" << infoLog << std::endl;
   }
   glDeleteShader(vShader);
   glDeleteShader(fShader);
}

void Shader::use()
{
   glUseProgram(mProgram);
}

void Shader::setBool(const std::string &name, bool value) const
{
   glUniform1i(glGetUniformLocation(mProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
   glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
   glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

Shader::~Shader()
{
   glDeleteProgram(mProgram);
}