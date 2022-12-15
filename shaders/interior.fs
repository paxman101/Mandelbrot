#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler1D tex;
uniform vec2 center;
uniform float scale;
uniform float eps;
uniform int iter;

void main()
{
    vec2 z, c;
    c.x = 1.77777 * (TexCoord.x - 0.5) * scale - center.x;
    c.y = (TexCoord.y - 0.5) * scale - center.y;
    z = c;
    vec2 der = vec2(1.0f, 0.0f);
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    int i;
    float x, y, derx, dery;
    for (i = 0; i < iter; i++) {
        x = (z.x * z.x - z.y * z.y) + c.x;
        y = (z.y * z.x + z.x * z.y) + c.y;
        float r2 = x * x + y * y;
        derx = der.x * der.x - der.y * der.y;
        dery = der.y * der.x + der.y * der.x;
        if((derx * derx) + (dery * dery) < eps*eps){
            color = texture(tex, (float(i) / 100.0f));
            break;
        }
        if (r2 > (100)) {
            color = texture(tex, (float(i) / 100.0f));
            break;
        }
        der.x = derx * 2 * (z.x * z.x - z.y * z.y);
        der.y = dery * 2 * (z.y * z.x + z.x * z.y);
        z.x = x;
        z.y = y;
    }
    FragColor = color;
}