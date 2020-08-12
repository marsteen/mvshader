const char* shader_sphere_vert =

"//\n"
"// Vertex Shader\n"
"//\n"
"\n"
"attribute vec4  aVertAttrib;\n"
"attribute vec2  aTextAttrib;\n"
"varying vec2    vTextVary;\n"
"\n"
"void main()\n"
"{\n"
"    vTextVary = aTextAttrib;\n"
"    gl_Position  = aVertAttrib;\n"
"}\n"
"\n";
