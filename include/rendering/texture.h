#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GLuint id;
    int width;
    int height;
} Texture;

Texture* texture_create(void);
Texture* texture_create_file(const char* path);
Texture* texture_create_data(unsigned char* data, int width, int height, int channels);
void texture_bind(Texture* texture, GLuint slot);
void texture_destroy(Texture* texture);

#ifdef __cplusplus
}
#endif

#endif /* TEXTURE_H */
