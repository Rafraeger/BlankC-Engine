# Asset Management & Resource Loading

This document explains how BlankC Engine loads and manages assets from disk into computer memory (RAM) and graphics memory (VRAM). All asset loading functions are located under the `resource/` module (`loader.c` and `obj_loader.c`).

## 1. Path Resolution (`asset_path`)

Since C programs can be run from various working directories, relying purely on *relative paths* (like `"assets/texture.png"`) often leads to *File Not Found* errors.

To solve this, BlankC uses the **`asset_path(const char* rel_path)`** function.
- This function dynamically finds the location of your *executable* file.
- It will walk backwards up the folder structure (up to 20 levels) looking for a folder named `assets/`.
- Once found, it concatenates the path to produce an **Absolute Path**.
- On Windows, this function automatically normalizes path separators (changing `/` to `\`) to prevent compatibility issues with third-party libraries.

**Golden Rule:** Every string returned by `asset_path()` is dynamically allocated (`malloc`). You **must** call `free()` after you are done using it to prevent memory leaks.

## 2. Loading Textures

BlankC uses **`stb_image`** to parse image files (PNG, JPG, BMP) into pixel arrays.

1.  **`texture_load()`:** Reads the image file into CPU RAM (`TextureData`).
2.  **`texture_create_file()`:** This is the main *wrapper*. This function calls `texture_load`, then takes the pixel data and sends it to GPU VRAM via `glTexImage2D`. The function then calls `texture_data_free()` to discard the RAM copy, leaving only the GPU reference (as a `GLuint id` inside the `Texture` struct).
3.  **Mipmaps:** The engine automatically generates mipmaps (`glGenerateMipmap`) for every loaded texture, so textures look smooth when viewed from a distance.
4.  **Formats:** Supports RGB textures (3 channels) and RGBA (4 channels, supports transparency).

## 3. Loading 3D Models (OBJ Files)

BlankC has an internal OBJ parser (`obj_loader.c`) designed specifically to read Wavefront `.obj` files.

### OBJ Parser Features:
- Supports extraction of Vertices (`v`), Texture Coordinates (`vt`), Normals (`vn`), and Faces (`f`).
- Currently **only** supports **Triangle** faces. Make sure to check the "Triangulate Faces" option when exporting your model from Blender or other 3D software.

### Loading Memory Management:
Reading large models can potentially cause a *Stack Overflow* if using static arrays. Therefore, the `obj_load` parser uses a **`DynArray`** (Dynamic Array) structure.
- `DynArray` starts allocation on the heap with a small size.
- If capacity is full when reading a new line from the `.obj` file, it automatically doubles its size using `realloc()`.
- After the file is completely read, data from the various `DynArray`s is assembled into a single `Vertex` format ready to be sent to the GPU (`mesh_create`).
- All temporary `DynArray`s (*scratch memory*) are immediately `free()`'d.

## 4. Limitations

- **No Resource Cache/Manager:** If you load `"cube.obj"` 10 times using `obj_load()`, the engine will read the file from disk 10 times and duplicate the data in VRAM 10 times. It is highly recommended that you load models/textures once into a variable, then pass the same pointer (reference) to various `SceneNode`s.
- **MTL Materials:** The current OBJ parser does not read material files (`.mtl`). Texture setup must be done manually in C code by assigning the `Texture*` struct to `SceneNode->texture`.