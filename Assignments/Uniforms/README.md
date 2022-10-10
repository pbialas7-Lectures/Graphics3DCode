# Uniforms

In this assignment you will learn how to add uniform  interface blocks to shaders and connect them to buffers. As 
usual please start  with duplicating the `Indices` assignment and rename it to `Uniforms`. 

## Uniform interface block

### Modifying pixel color

1. First  please add the
   ```glsl
    layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color; 
   };
    ```  
   interface block to fragment shader. This should change nothing. 

2. Next you have to create a buffer that will back up this interface block. In the `init` method create a buffer and 
   bind it to target `GL_UNIFORM_BUFFER`. Then allocate `8*sizeof(float)` memory for this buffer without loading any 
   data using the `glBufferData` function. Finally, bind this buffer to the interface block using `glBindBufferBase` 
   function. 

3. Now load the data into the buffer. To this end define two variables:
   ```c++
   float strength;
   float color[3];
   ```
   Load them into the buffer using `glBufferSubdata` function. Please respect the `std140` layout rules which can be 
   found on page 138 of 
   [OpenGL 4.5 (Core profile) specification](https://www.khronos.org/registry/OpenGL/specs/gl/glspec45.core.pdf). In 
   this case rules 1 and 3 apply.   

5. Now use the variables  from the interface block to modify the pixel color by multiplying the original color by 
   `strength` and `color`. Modify only the  RGB values and not the alpha. 

### Moving houses :)

Now we will transform the model itself by transforming the coordinates of the vertices in the vertex shader. The 
parameters of this transformation will be passed to vertex shader via  uniform interface block. 

1. Please add interface block 
   ```glsl
   layout(std140, binding=1) uniform Transformations {
    vec2 scale;
    vec2 translation;
    mat2 rotation;
    };
   ```
2. In the `init` function  a new uniform buffer and allocate storage for it. According to `std140` layout `vec2` 
   variables are aligned to 8 bytes (two floats) boundaries (rule 2).  The 2x2 matrix `mat2` is treated as an array 
   of two `vec2` variables  (rule 5). Each element of this array is aligned to a 16 bytes boundary (rule 4). How 
   many bytes you have to allocate?
3. Using `glBoundBufferBase` function bind this buffer to uniform interface block. 

4. Create variables that will hold values to be loaded into the buffer:
   ```c++
   float theta = 1.0*glm::pi<float>()/6.0f;
   auto cs = std::cos(theta);
   auto ss = std::sin(theta);  
   glm::mat2 rot{cs,ss,-ss,cs};
   glm::vec2 trans{0.0,  -0.25};
   glm::vec2 scale{0.5, 0.5};
   ```
   As we are using now `glm` for matrix and vector manipulation you  have to include `glm/glm.hpp` and 
   `glm/gtc/constants.hpp` files. 

5. Load data into the uniform buffer remember the `std140` layout rules. For `mat2` it means that each  column of 
   the matrix is aligned to a four floats boundary and the whole array is padded to a multiple of four floats as well.  
   So a matrix
   ```
   a b
   c d
   ```
   Is stored as 
   ```
   a c x x c d x x 
   ```
   where x denotes a "unused" space. 
   You can access the column of a `glm` matrix by subscripting: `rot[0]` is the fits column and `rot[1]` second. 

6. And finally transform the vertices in the vertex shader:
   ```glsl 
   gl_Position.xy = rotation*(scale*a_vertex_position.xy)+translation;
   gl_Position.zw = a_vertex_position.zw;  
   ```
   It is best to add the transformations one by  one.   Start with translation, then scale and finally rotation. Are the 
   parenthesis around  `scale*a_vertex_position.xy` really needed? Why? 


