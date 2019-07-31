/* https://github.com/skaslev/catmull-clark/mesh.h
The MIT License

Copyright (c) 2009-2017 Slavomir Kaslev, https://github.com/skaslev/catmull-clark

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef SUBD_MESH_H
#define SUBD_MESH_H

#ifndef HAVE_LIBOSDCPU
/*
 * Mesh construction
 */
struct mesh *mesh_create(void);
void mesh_free(struct mesh *mesh);

void mesh_add_vertex(struct mesh *mesh, const float *v);
void mesh_add_normal(struct mesh *mesh, const float *n);
void mesh_begin_face(struct mesh *mesh);
void mesh_add_index(struct mesh *mesh, int vi, int ni);
void mesh_end_face(struct mesh *mesh);
void mesh_compute_normals(struct mesh *mesh);

/*
 * Vertex buffer access
 */
int mesh_vertex_buffer(const struct mesh *mesh, const float **buf);
int mesh_normal_buffer(const struct mesh *mesh, const float **buf);

/*
 * Face access
 */
int mesh_face_count(const struct mesh *mesh);
int mesh_face_vertex_count(const struct mesh *mesh, int face);
void mesh_face_vertex_index(const struct mesh *mesh, int face, int vert,
                            int *vertex_idx, int *normal_idx);

float *mesh_get_vertex(const struct mesh *mesh, int face, int vert);
float *mesh_get_normal(const struct mesh *mesh, int face, int vert);

#endif
#endif
