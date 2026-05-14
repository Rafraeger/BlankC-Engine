#ifndef LOADER_H
#define LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* data;
    int width;
    int height;
    int channels;
} TextureData;

TextureData texture_load(const char* path);
void texture_data_free(TextureData* data);

char* file_read_text(const char* path, long* out_size);
void file_text_free(char* text);

/* Resolve relative path (e.g. "assets/...") against project root.
   Automatically detects project root by walking up from exe directory. */
char* asset_path(const char* rel_path);
void asset_path_free(char* path);

#ifdef __cplusplus
}
#endif

#endif /* LOADER_H */
