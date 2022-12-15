#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler1D tex;
uniform dvec2 center;
uniform double scale;
uniform int iter;

void main()
{
    dvec2 z, c;
    c.x = 1.33333 * (TexCoord.x - 0.5) * scale - center.x;
    c.y = (TexCoord.y - 0.5) * scale - center.y;

    int i;
    z = c;
    double j, x, y;
    int pow = 1;
    for (i = 0; i < iter; i++) {
        x = (z.x * z.x - z.y * z.y) + c.x;
        y = (z.y * z.x + z.x * z.y) + c.y;
        double r2 = x * x + y * y;

        if (r2 > 100) {
            break;
        }
        z.x = x;
        z.y = y;
        pow = pow * 2;
    }
    if (i < iter) {
        j = i;
        float log_zn = log(float(x*x + y*y)) / 2;
        double nu = log(log_zn / log(2)) / log(2);
        j = i + 1 - nu;
    }
    float waste;
    vec4 color1 = texture(tex, (float(floor(j)) / 100.0f));
    vec4 color2 = texture(tex, (float((floor(j) + 1.0f)) / 100.0f));

    FragColor = mix(color1, color2, modf(float(j), waste));
    //FragColor = texture(tex, TexCoord.x);
}