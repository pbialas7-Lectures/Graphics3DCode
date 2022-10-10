# Projection - View - Model

The aim of this assignment is to familiarize you with projection and view transforms. 

1.  In vertex shader change the `Transformations` interface block to
    ```c++
    layout(std140, binding=1) uniform Transformations {
        mat4 PVM;
    };
    ``` 
    Use the `PVM` matrix to transform vertex coordinates:
    ```c++
    gl_Position =  PVM * a_vertex_position;
    ``` 
    
2.  Modify the `cpu` code as to load  the `PVM` matrix into the uniform buffer. Start with identity matrix:
    ```c++
    glm::mat4 PVM(1.0f); 
    ```  
    
3. On the `cpu` side compose the `PVM` matrix from Model, View and Projection matrices. Start with identity model matrix. After including the `glm/gtc/matrix_transform.hpp`  header file, you can use the `glm::perspective` function to create the projection matrix and `glm::lookAt` function to create view matrix. 
   Familiarize yourself with parameters of those functions. 