#version 330 core // Указывает версию OpenGL (3.3) и профиль Core (современный, без устаревших функций)

// Входные данные (атрибуты вершин), которые передаются из C++ кода через VAO/VBO
layout (location = 0) in vec3 aPos;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0f);
}
