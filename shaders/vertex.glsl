#version 460

layout(location = 0) out vec2 TexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

void main()
{
	float x = float((gl_VertexIndex & 1) << 2);
    float y = float((gl_VertexIndex & 2) << 1);
	TexCoord = vec2(x * 0.5, y * 0.5);
	gl_Position = vec4(x - 1, y - 1, 0, 1);
}
