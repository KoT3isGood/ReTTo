#version 330

out vec4 FragColor;
in vec2 texCoord;
void main() {
    FragColor = vec4(texCoord,0.0, 1.0f);
}

