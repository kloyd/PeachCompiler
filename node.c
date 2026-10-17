#include "compiler.h"
#include <assert.h>
#include "helpers/vector.h"

struct vector *node_vector = NULL;
struct vector *node_vector_root = NULL;

void node_set_vector(struct vector *vec, struct vector *root_vec) {
    node_vector = vec;
    node_vector_root = root_vec;
}