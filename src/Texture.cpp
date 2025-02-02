#include<Header/Texture.h>

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;

    int widthImg, heightImg, numColorChanel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColorChanel, 0);

    if (!bytes) {
        std::cout << "Failed to load texture: " << image << std::endl;
        return;
    }


    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(type, ID);


    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

   
    GLenum internalFormat = (numColorChanel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(type, 0, internalFormat, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(type);

    stbi_image_free(bytes);
    glBindTexture(type, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit); 
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}
