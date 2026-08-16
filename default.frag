#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform float time;
uniform sampler2D tex0;

void main()
{
    // --- 1. ПСИХОДЕДЕЛИЧЕСКОЕ ИСКАЖЕНИЕ ПРОСТРАНСТВА (ВОЛНЫ) ---
    // Создаем волнистые UV-координаты, зависящие от времени и позиции
    vec2 uv = texCoord;
    uv.x += sin(uv.y * 10.0 + time * 2.5) * 0.03;
    uv.y += cos(uv.x * 8.0 + time * 2.0) * 0.03;
    
    // Эффект «дыхания» или пульсации экрана по спирали
    float dist = distance(uv, vec2(0.5));
    uv += (uv - 0.5) * sin(dist * 12.0 - time * 3.0) * 0.04;

    // --- 2. ХРОМАТИЧЕСКАЯ АБЕРРАЦИЯ (РАССЛОЕНИЕ ЦВЕТОВ) ---
    // Читаем текстуру с небольшим сдвигом для каждого цветового канала
    float shift = 0.015 * sin(time * 1.5);
    float r = texture(tex0, uv + vec2(shift, 0.0)).r;
    float g = texture(tex0, uv).g;
    float b = texture(tex0, uv - vec2(shift, 0.0)).b;
    vec3 baseColor = vec3(r, g, b);

    // --- 3. КИСЛОТНЫЙ ЦВЕТОВОЙ СДВИГ ---
    // Генерируем безумные, переливающиеся неоновые цвета
    vec3 trippyColor = vec3(
        sin(time * 1.5 + uv.x * 5.0) * 0.5 + 0.5,
        cos(time * 1.2 + uv.y * 5.0) * 0.5 + 0.5,
        sin(time * 1.8 + (uv.x + uv.y) * 3.0) * 0.5 + 0.5
    );

    // --- 4. ФИНАЛЬНОЕ СМЕШИВАНИЕ ---
    // Смешиваем искаженную текстуру с кислотными цветами.
    // Режим "Overlay/Multiply" (умножение) здесь выглядит жестче и наркоманские, чем обычный mix.
    vec3 finalRGB = baseColor * (trippyColor * 1.5);
    
    // Добавим немного пульсации яркости в такт "дыханию"
    finalRGB += sin(time * 4.0) * 0.05;

    FragColor = texture(tex0, texCoord);//vec4(finalRGB, 1.0);
}
