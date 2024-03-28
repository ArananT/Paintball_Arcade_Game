// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 transformation_matrix;
uniform mat4 view_matrix;
uniform vec2 uScale;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

void main()
{
    // Apply non-uniform scaling directly to the vertex position
    vec4 vertex_pos = vec4(vertex, 0.0, 1.0);
    vec4 scaledVertex = vec4(vertex.x * uScale.x, vertex.y * uScale.y, 0.0, 1.0);
    
    // Apply transformation and view matrices
    gl_Position = view_matrix * transformation_matrix * vertex_pos;
    
    // Pass attributes to fragment shader
    color_interp = vec4(color, 1.0);
    uv_interp = uv;
}
