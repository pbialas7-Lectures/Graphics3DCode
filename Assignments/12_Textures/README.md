# Textures

In the previous assignment we have implemented a simple material class `KdMaterial` that allows us to set the color of
the object. In this assignment, we will go one step further and add the possibility for this color to be taken from
texture. In this way we will be able to modify the color at the pixel, rather than at the vertex level. You will do it
by modifying the `KdMaterial` class and shaders. We will keep the names because we are still only modifying the diffuse
reflection coefficient of the object.

## Texture coordinates

To add a texture to the object, we need to know the texture coordinates for each vertex. The texture coordinates (UV
map) for the pyramid are provided in the file [uv.svg](uv.svg):
<p align="center"><img alt="UV map" src="uv.svg" width="50%"></p>

As stated before, the vertices are considered equal if they have all attributes equal. This means that two vertices with
different texture coordinates are considered different. So if not for the vertex color attribute, we would have eight
different vertices in the pyramid.

1. So go ahead and remove the vertex color attribute from the vertex buffer. Remember to
   change the arguments of the mesh constructor.

2. Add the texture coordinates read from the UV map to the vertex buffer and at the same time remove the vertices
   that are no longer needed, so that only the eight vertices with different positions or texture coordinates remain.
   Modify the index buffer accordingly. Remember to respect the orientation of the triangles.
   Remember to change the arguments of the mesh constructor and add the
   corresponding `add_attribute` call. Use `AttributeType::TEXCOORD_0` attribute. Make sure that everything works.

3. In vertex shader, add the corresponding input vertex attribute with location defined by
   the `AttributeType::TEXCOORD_0`, that is `layout(location = 3)`. Define the corresponding output variable and
   assign the vertex attribute to it.

4. In fragment shader, add the corresponding input variable. Use this variable to set the RG colors of the fragment.
   This will enable you to check if the texture coordinates are sent correctly to the fragment shader. If everything is OK
   revert to the original color setting code.

## Texture

We will use the texture from the file `multicolor.png` which can be found in the `Models` directory.

<p align="center"><img alt="multicolor texture" src="multicolor.png" width="30%"></p>

To use the texture, we need to load it and send it to the shader.
For loading the image we will use the `stb_image` library.
The library is already included in the project.
To use it please include the `stb/stb_image.h` header file. The image can then be loaded with the `stbi_load` function.


1. In the `init` method of the `SimpleShapeApplication` class load the image using the following code

   ```c++
   stbi_set_flip_vertically_on_load(true);
   GLint width, height, channels;
   auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
   auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
   if (!img) {
       SPDLOG_ERROR("Could not read image from file `{}'", texture_file);
   } else {
       SPDLOG_INFO("Loaded a {}x{} texture with {} channels", width, height, channels);
   }
   ```
   If everything is correct, you should see the info message. If loading fails, there is no point in continuing, so
   you may exit the program in the error branch.

2. Create the texture using the `glGenTextures` function, storing its handle in a `GLuint tex_handle` variable, then
   bind it using `glBindTexture` and load the image using `glTexImage2D` function. Set the
   interpolation (filtering) methods that do not use mipmapping using the `glTexParameteri` function, e.g. set both
   `GL_TEXTURE_MIN_FILTER` and `GL_TEXTURE_MAG_FILTER` to `GL_LINEAR`. This is necessary: the default value of
   `GL_TEXTURE_MIN_FILTER` uses mipmaps, and as we do not create them, the texture would be incomplete and the sampler
   would return black.

   The format of the image data passed to `glTexImage2D` must match the number of channels reported by `stbi_load`:
   `GL_RGB` for three channels and `GL_RGBA` for four. The `multicolor.png` image has three channels, but other images
   may have four. After the call to `glTexImage2D`, OpenGL has its own copy of the image, so free the memory allocated by
   `stbi_load` using `stbi_image_free(img)`.

   By default OpenGL expects each row of the image data to start at an address that is a multiple of four bytes. This
   holds for `multicolor.png` (1024 pixels × 3 bytes), but if you use an image with three channels whose width is not
   a multiple of four, call `glPixelStorei(GL_UNPACK_ALIGNMENT, 1)` before `glTexImage2D`, otherwise the texture will
   be skewed.

Now we have to modify the fragment shader to enable it to read the color from the texture. That requires a _sampler_ which
is defined as a uniform variable

```glsl
uniform sampler2D map_Kd;
```

1. Please add this line to the fragment shader. This is a uniform variable, not an interface block. To assign a value
   to it, we must first get its location using the `glGetUniformLocation` function. Add a static field `map_Kd_location_` of
   type `GLint` to the `KdMaterial` class
   ```c++
   inline static GLint map_Kd_location_ = -1;
   ```
   (the `inline` keyword lets you initialize a static field in the class body; without it, you would have to define
   it separately in `KdMaterial.cpp`) and set its value in the `KdMaterial::init` function
   ```c++
   OGL_CALL(map_Kd_location_ = glGetUniformLocation(program(), "map_Kd"));
   if (map_Kd_location_ == -1) {
       SPDLOG_WARN("Cannot find map_Kd uniform");
   }
   ```
2. Similarly, as with vertex colors, we have to somehow transmit to the fragment shader the information that the texture
   will be bound to the sampler, and we want to use it. We will do it using the material uniform buffer. Please add the
   ```glsl
   bool use_map_Kd;
   ```
   field to the `KdMaterial` interface block in fragment shader, after `use_vertex_colors`. Then in the `bind` method of
   the `KdMaterial` class load zero into this field. The bool variable in an interface block takes as much space as int
   or float, so `use_map_Kd` starts at byte 20 (`Kd` takes bytes 0-15 and `use_vertex_colors` bytes 16-19), and the
   buffer of `2*sizeof(glm::vec4)` bytes created in the previous assignment is still large enough. You can check the
   offsets using `uniform_info(program(), "KdMaterial")` as described in the `Uniforms` assignment.

3. In the fragment shader add code that, depending on the value of the `use_map_Kd` variable, multiplies the color
   calculated so far using `Kd`, and vertex colors if present, by the value obtained from the sampler:
   ```glsl
   vec4 texture_color = texture(map_Kd, vertex_texcoord_0);
   ```
   In this example `vertex_texcoord_0` are the vertex texture coordinates.
   Because the value of the `use_map_Kd` variable is set to zero (`false`), you should not see any difference.

4. Now we have to connect the sampler to the created texture. In the `KdMaterial` class add the field `texture_`
   of type GLuint. The `texture_` field will contain the handle to the texture that we have
   just created.

5. Give this field the default value zero, `GLuint texture_ = 0;`, so the existing constructors do not have to set it.
   Then add a new three parameter constructor that sets this field:
   ```c++
   KdMaterial(const glm::vec4 &Kd, bool use_vertex_colors, GLuint texture) :
           Kd_(Kd), use_vertex_colors_(use_vertex_colors), texture_(texture) {}
   ```

6. In the `bind` method add the code that checks if the `texture_` field is greater than zero. If so please load one
   into the `use_map_Kd` field of the material uniform buffer. Then assign the texture unit zero to the sampler
   ```c++
   OGL_CALL(glUniform1i(map_Kd_location_, 0));
   ```
   (`glUniform1i` sets the uniform in the currently used program, so this has to come after the `glUseProgram` call),
   set the active texture unit to zero using
   the `glActiveTexture` function and bind `texture_` using the `glBindTexture` function.
   Samplers are initialized to texture unit zero, so it would work without the `glUniform1i` call, but it is better to
   be explicit, as we will use more texture units later.

   If `texture_` is equal to zero then just load zero into `use_map_Kd` field of the material uniform buffer.

   In the `unbind` method check if
   the value of the `texture_` field is greater than zero and if so unbind the texture.

7. In the `init` method of the `SimpleShapeApplication` add a single submesh encompassing all the indices and add a
   material with texture. Set the `Kd` to white.
   ```c++
   pyramid->add_submesh(0, indices.size(), new xe::KdMaterial({1.f, 1.f, 1.0f, 1.0f}, false, tex_handle));
   ```
   The material does not take ownership of the texture, it only uses its handle.

## Gamma correction

The texture that we have loaded is (probably) in the sRGB color space. This means that the color values are not linear.
At this moment, this is not a problem because we are not doing any calculations on the color. We are just sending it to
the screen where it is expected to be in sRGB color space.

1. But if we want to do any calculations on the color, we have to convert it to the linear color space. This can be done
   automatically by OpenGL, by changing the internal format in the `glTexImage2D` call to `GL_SRGB8` (or
   `GL_SRGB8_ALPHA8` for images with four channels; `GL_SRGB8` has no alpha channel). Please do it and
   notice that the colors have changed. This is because we are sending the linear values to the screen without any gamma
   correction.
   <p align="center"><img alt="linear RGB" src="linearRGB.png" width="50%"></p>

2. We will add gamma correction directly in the shader. In fragment shader please add the function definition
   ```glsl
   vec3 srgb_gamma_correction(vec3 color) {
      color = clamp(color, 0.0, 1.0);
      color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
      return color;
   }
   ```
   and then modify the code that calculates the color of the fragment to
   ```glsl
   vFragColor.a = color.a;
   vFragColor.rgb = srgb_gamma_correction(color.rgb);
   ```
   with `color` being the final color calculated in the shader. The colors should change back to the original ones.
   <p align="center"><img alt="sRGB" src="sRGB.png" width="50%"></p>
