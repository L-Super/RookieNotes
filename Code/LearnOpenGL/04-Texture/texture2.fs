#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // 把得到的纹理颜色与顶点颜色混合
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}