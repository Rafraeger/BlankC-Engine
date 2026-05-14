#include "rendering/texture.h"
#include "resource/loader.h"
#include <stdlib.h>
#include <string.h>

Texture* texture_create(void) {
    Texture* tex = malloc(sizeof(Texture));
    glGenTextures(1, &tex->id);
    tex->width = 0;
    tex->height = 0;
    return tex;
}

Texture* texture_create_file(const char* path) {
    if (!path) return NULL;
    TextureData data = texture_load(path);
    if (!data.data) return NULL;

    Texture* tex = texture_create();
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (data.channels == 4) format = GL_RGBA;
    else if (data.channels == 2) format = GL_RG;
    else if (data.channels == 1) format = GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, format, data.width, data.height, 0, format, GL_UNSIGNED_BYTE, data.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    tex->width = data.width;
    tex->height = data.height;

    texture_data_free(&data);
    return tex;
}

Texture* texture_create_data(unsigned char* data, int width, int height, int channels) {
    if (!data || width <= 0 || height <= 0) return NULL;
    Texture* tex = texture_create();
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;
    else if (channels == 2) format = GL_RG;
    else if (channels == 1) format = GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    tex->width = width;
    tex->height = height;
    return tex;
}

void texture_bind(Texture* texture, GLuint slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_destroy(Texture* texture) {
    if (texture) {
        glDeleteTextures(1, &texture->id);
        free(texture);
    }
}
