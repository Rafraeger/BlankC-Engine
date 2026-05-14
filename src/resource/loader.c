#include "resource/loader.h"
#include "core/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* ---------- asset_path helper ---------- */
static char s_project_root[4096] = {0};

static void get_exe_dir(char* buf, size_t buf_size) {
#ifdef _WIN32
    wchar_t wpath[MAX_PATH];
    GetModuleFileNameW(NULL, wpath, MAX_PATH);
    /* Convert wide to ASCII */
    int len = WideCharToMultiByte(CP_UTF8, 0, wpath, -1, buf, (int)buf_size, NULL, NULL);
    if (len == 0) {
        /* Fallback: ASCII codepage */
        WideCharToMultiByte(CP_ACP, 0, wpath, -1, buf, (int)buf_size, NULL, NULL);
    }
    /* Strip exe name to get directory — find last separator of either kind */
    char* last_back = strrchr(buf, '\\');
    char* last_fwd = strrchr(buf, '/');
    char* last = last_back;
    if (!last || (last_fwd && last_fwd > last)) last = last_fwd;
    if (last) *last = '\0';
#else
    /* Unix: readlink /proc/self/exe or use argv[0] */
    ssize_t len = readlink("/proc/self/exe", buf, buf_size - 1);
    if (len > 0) {
        buf[len] = '\0';
        char* last = strrchr(buf, '/');
        if (last) *(last + 1) = '\0'; /* keep trailing slash, truncate after */
    }
#endif
}

static void strip_filename(char* path) {
    char* last = strrchr(path, '\\');
    if (last) { *last = '\0'; return; }
    last = strrchr(path, '/');
    if (last) *last = '\0';
}

char* asset_path(const char* rel_path) {
    if (!rel_path) return NULL;

    /* If path is absolute, return a copy */
    if (rel_path[0] == '/' ||
        (rel_path[0] && rel_path[1] == ':')) {
        return strdup(rel_path);
    }

    /* Cache project root on first call */
    if (!s_project_root[0]) {
        char exe_dir[4096];
        get_exe_dir(exe_dir, sizeof(exe_dir));
        log_info("asset_path: exe_dir = %s", exe_dir);

        /* Walk up directories looking for assets/ folder */
        char search[4096];
        strcpy(search, exe_dir);
        for (int i = 0; i < 20; i++) {
            snprintf(search, sizeof(search), "%s/assets", exe_dir[0] ? exe_dir : ".");
#ifdef _WIN32
            if (_access(search, 0) == 0) {
#else
            if (access(search, F_OK) == 0) {
#endif
                strcpy(s_project_root, exe_dir);
                log_info("asset_path: found project root = %s", s_project_root);
                break;
            }
            /* Try parent */
            strip_filename(exe_dir);
        }
        /* Fallback: current directory */
        if (!s_project_root[0])
            strcpy(s_project_root, ".");
    }

   /* Build full path: project_root + separator + rel_path */
    size_t full_len = strlen(s_project_root) + 1 + strlen(rel_path) + 1;
    char* full = malloc(full_len);
    if (!full) return NULL;
    log_info("asset_path: %s -> %s/%s", rel_path, s_project_root, rel_path);
#ifdef _WIN32
    snprintf(full, full_len, "%s\\%s", s_project_root, rel_path);
    /* Normalize: convert all / to \ on Windows */
    for (char* p = full; *p; p++) if (*p == '/') *p = '\\';
#else
    snprintf(full, full_len, "%s/%s", s_project_root, rel_path);
#endif
    return full;
}

void asset_path_free(char* path) {
    free(path);
}

TextureData texture_load(const char* path) {
    if (!path) return (TextureData){NULL, 0, 0, 0};
    TextureData tex = {NULL, 0, 0, 0};
    tex.data = stbi_load(path, &tex.width, &tex.height, &tex.channels, 0);
    if (!tex.data) {
        return tex;
    }
    return tex;
}

void texture_data_free(TextureData* data) {
    if (!data) return;
    if (data->data) {
        stbi_image_free(data->data);
        data->data = NULL;
    }
}

char* file_read_text(const char* path, long* out_size) {
    if (!path) return NULL;
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buf = malloc(size + 1);
    if (!buf) { fclose(f); return NULL; }
    size_t read_bytes = fread(buf, 1, size, f);
    buf[read_bytes] = '\0';
    fclose(f);

    if (out_size) *out_size = (long)read_bytes;
    return buf;
}

void file_text_free(char* text) {
    if (text) free(text);
}
