#include<Header/shaderClass.h>

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    //Create Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Specify the vertexShader source
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader); //Compile it

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Specify the fragmentShader source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader); //Compile it

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader); //Attach the vertexShader to the shaderProgram
    glAttachShader(ID, fragmentShader); //Attach the fragmentShader to the shaderProgram
    glLinkProgram(ID); //Link the shaderProgram, basically telling the program to use this shader program

    glDeleteShader(vertexShader); //We already attach the vertex and fragment shader so we delete it so we don't accidentally modify them
    glDeleteShader(fragmentShader);

}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}