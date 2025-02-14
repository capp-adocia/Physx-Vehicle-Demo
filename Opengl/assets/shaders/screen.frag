#version 460 core
out vec4 FragColor;

in vec2 uv;
uniform sampler2D screenTexture;

vec3 colorInvert(vec3 color)
{
    vec3 invertColor = vec3(1.0) - color;
    return invertColor;
}

vec3 gray(vec3 color)
{
    float avg = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
    return vec3(avg, avg, avg);
}

void main()
{
//    vec3 color = colorInvert(texture(screenTexture, uv).rgb);
    vec3 color = texture(screenTexture, uv).rgb;

    FragColor = vec4(color, 1.0);
}