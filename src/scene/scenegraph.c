#include "scene/scenegraph.h"
#include <string.h>
#include <stdlib.h>

static void scenegraph_destroy_nodes(SceneNode* node);
static void scenegraph_update_nodes(SceneNode* node);
static SceneNode* scenegraph_find_node(SceneNode* node, const char* name);

SceneGraph* scenegraph_create(void) {
    SceneGraph* graph = malloc(sizeof(SceneGraph));
    graph->root = malloc(sizeof(SceneNode));
    graph->root->transform = transform_create();
    graph->root->children = calloc(MAX_CHILDREN, sizeof(SceneNode*));
    graph->root->child_count = 0;
    graph->root->parent = NULL;
    graph->root->name = strdup("root");
    graph->root->user_data = NULL;
    graph->root->texture = NULL;
    graph->root->is_interactive = 0;
    graph->root->on_interact = NULL;
    return graph;
}

void scenegraph_destroy(SceneGraph* graph) {
    if (!graph) return;
    /* Recursive destroy */
    for (int i = 0; i < graph->root->child_count; i++) {
        scenegraph_destroy_nodes(graph->root->children[i]);
    }
    free(graph->root->children);
    transform_destroy(graph->root->transform);
    free((char*)graph->root->name);
    free(graph->root);
    free(graph);
}

static void scenegraph_destroy_nodes(SceneNode* node) {
    for (int i = 0; i < node->child_count; i++) {
        scenegraph_destroy_nodes(node->children[i]);
    }
    free(node->children);
    transform_destroy(node->transform);
    free((char*)node->name);
    free(node);
}

SceneNode* scenegraph_root(SceneGraph* graph) {
    return graph->root;
}

SceneNode* scenegraph_add_node(SceneGraph* graph, SceneNode* parent, const char* name) {
    (void)graph;
    if (parent->child_count >= MAX_CHILDREN) return NULL;

    SceneNode* node = malloc(sizeof(SceneNode));
    node->transform = transform_create();
    node->children = calloc(MAX_CHILDREN, sizeof(SceneNode*));
    node->child_count = 0;
    node->parent = parent;
    node->name = strdup(name);
    node->user_data = NULL;
    node->texture = NULL;
    node->is_interactive = 0;
    node->on_interact = NULL;

    parent->children[parent->child_count++] = node;
    return node;
}

void scenegraph_remove_node(SceneGraph* graph, SceneNode* node) {
    if (!graph || !node || !node->parent) return;
    SceneNode* parent = node->parent;
    for (int i = 0; i < parent->child_count; i++) {
        if (parent->children[i] == node) {
            /* Shift remaining children down */
            for (int j = i; j < parent->child_count - 1; j++) {
                parent->children[j] = parent->children[j + 1];
            }
            parent->children[--parent->child_count] = NULL;
            break;
        }
    }
    /* Recursively destroy the removed node and its subtree */
    scenegraph_destroy_nodes(node);
}

void scenegraph_update(SceneGraph* graph) {
    transform_update(graph->root->transform);
    scenegraph_update_nodes(graph->root);
}

static void scenegraph_update_nodes(SceneNode* node) {
    for (int i = 0; i < node->child_count; i++) {
        SceneNode* child = node->children[i];
        child->transform->parent = node->transform;
        transform_update(child->transform);
        scenegraph_update_nodes(child);
    }
}

SceneNode* scenegraph_find(SceneGraph* graph, const char* name) {
    return scenegraph_find_node(graph->root, name);
}

static SceneNode* scenegraph_find_node(SceneNode* node, const char* name) {
    if (strcmp(node->name, name) == 0) return node;
    for (int i = 0; i < node->child_count; i++) {
        SceneNode* found = scenegraph_find_node(node->children[i], name);
        if (found) return found;
    }
    return NULL;
}
