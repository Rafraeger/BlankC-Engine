#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "scene/transform.h"
#include "rendering/texture.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CHILDREN 128

typedef struct SceneNode SceneNode;

typedef void (*OnInteractCallback)(SceneNode* node, void* engine);

struct SceneNode {
    Transform* transform;
    struct SceneNode** children;
    int child_count;
    struct SceneNode* parent;
    const char* name;
    void* user_data;
    Texture* texture; // New: per-node texture
    
    /* Interaction metadata */
    int is_interactive;
    OnInteractCallback on_interact;
};

typedef struct SceneGraph {
    SceneNode* root;
} SceneGraph;

SceneGraph* scenegraph_create(void);
void scenegraph_destroy(SceneGraph* graph);
SceneNode* scenegraph_root(SceneGraph* graph);
SceneNode* scenegraph_add_node(SceneGraph* graph, SceneNode* parent, const char* name);
void scenegraph_remove_node(SceneGraph* graph, SceneNode* node);
void scenegraph_update(SceneGraph* graph);
SceneNode* scenegraph_find(SceneGraph* graph, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* SCENEGRAPH_H */
