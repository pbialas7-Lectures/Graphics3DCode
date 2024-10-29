# Indices

#### 5 points

This exercise is intended to familiarize you with how the index buffer works.
As you have probably noticed, some
vertices in the vertex buffer are repeated.
In the case of large meshes, each vertex may be repeated several times,
which leads to waste of resources and consequently to slower application performance.
To remedy this, you can use an
index buffer.
The idea is to load only non-repeating vertices into the vertex buffer.
Then, to define triangles, we use the _indexes_ of the vertices that we pass in the index buffer. So instead of
repeating the same vertex several times, we use the same vertex index several times. As an index is an integer, we
save a lot of memory compared to vertex data that consists in our case of six floats.
Please remember that two vertices are considered the same if they have the same coordinates and the same attributes,
so if they differ in color, they are considered different even if they have the same coordinates.

1. First, we will not remove the vertices, we will only add an index buffer. To do this, you need to declare a new
   vector `vector<GLubyte>` which will contain the indexes. Since we are not changing anything in the vertex
   buffer for now, we fill it with integers from `0` to `number of vertices-1`.

1. Then, using the `glCreateBuffers` command, create a new index buffer in the same way as you have created the vertex
   buffer.
   Then, using the `glNamedBufferData` command, copy the data from the index vector to  it. 

1. Now bind this buffer
   using the `glBindBuffer` command  with target argument set to `GL_ELEMENT_ARRAY_BUFFER`  between calls to `glBindVertexArray(vao_)` and `glBindVertexArray(0)`.
   This way it will be remembered in the Vertex Array Object (VAO) `vao_`.

1. Now in the `frame` function we replace the drawing function `glDrawArrays` with `glDrawElements`, of course
   changing the arguments accordingly.

1. If our house is displayed, please remove the repeated vertices from the vertex buffer and modify the index buffer
   accordingly so that the __same__ house still appears on the screen. You should end up with 7 vertices in the vertex
   buffer. 
