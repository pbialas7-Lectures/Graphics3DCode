# Indices 

####  5 points

This exercise is intended to familiarize you with how the index cache works. As you have probably noticed, some vertices in the vertex buffer are repeated. In the case of large meshes, each vertex may be repeated several times, which leads to waste of resources and, consequently, slower application performance. To remedy this, you can use an index cache. The idea is to load only non-repeating vertices into the vertex buffer. Then, to define triangles, we use the _indexes_ of the vertices that I pass in the index buffer.


1. First, we will not remove the vertices, we will only add an index buffer. To do this, you need to declare a new vector `vector<std::GLushort>` which will contain the indexes. Since we are not changing anything in the vertex buffer for now, we fill it with indexes from `0` to `number of vertices -1`.

1. Then, using the `glGenBuffers` command, we create a new index buffer in the same way as we created the vertex buffer. Then we bind this buffer with the `glBindBuffer` command, giving `GL_ELEMENT_ARRAY_BUFFER` as the first argument. Then, using the `glBufferData` command, we copy the data from the index vector to it and unbind the buffer.

1. Now we bind this buffer again, but we do it between calls to `glBindVertexArray(vao_)` and `glBindVertexArray(0)`. This way it will be remembered in the Vertex Array Object (VAO) `vao_`.

1. Now in the `frame` function we replace the drawing function with `glDrawElements`, of course changing the arguments accordingly.

1. If our house is displayed, please remove the repeated vertices from the vertex buffer and modify the index buffer accordingly so that the __same__ house still appears on the screen.

1. How many bytes did we save? How many more bytes could we save? Please save the answer in the README.md file in the assignment folder.