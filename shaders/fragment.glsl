#version 460

layout(binding = 0) uniform sampler2D Image;
layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;

void main() {
	// FragColor1 = texelFetch(Image, (ivec2(gl_FragCoord.xy) / 4) * 4, 0);
	// FragColor1 = texelFetch(Image, ivec2(gl_FragCoord.xy) / 4, 2);

	ivec2 offset = ivec2(0);

	for (int i = 0; i < 5; i++) {
		if (gl_FragCoord.y > offset.y) {
			FragColor1 = texelFetch(Image, ivec2(gl_FragCoord.xy - ivec2(0, offset.y)) * int(pow(2, i)), 0);
			FragColor1 = texelFetch(Image, ivec2(gl_FragCoord.xy - ivec2(0, offset.y)), i);
		}
		offset.y += textureSize(Image, i).y;
	}
}
