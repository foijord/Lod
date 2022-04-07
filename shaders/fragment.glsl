#version 460

layout(binding = 0) uniform sampler2D Image;
layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;

void main() {
	FragColor0 = texture(Image, TexCoord);
	FragColor1 = texture(Image, TexCoord);
}
