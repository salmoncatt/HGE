#version 400 core
out vec4 fragColor;

in vec2 passedTextureCoords;


uniform sampler2D textureSampler;
uniform vec4 color;

void main() {
	
	//vec4 alpha = texture(textureSampler, passedTextureCoords);
	fragColor = vec4(0.1, 0.2, 1, texture(textureSampler, passedTextureCoords).r);
	//fragColor *= color;
	//fragColor = vec4(1,1,1,1);
}