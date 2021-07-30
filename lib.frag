#version 100
precision mediump float;

uniform float iTime;

float hue2rgb(float f1, float f2, float hue) {
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    float res;
    if ((6.0 * hue) < 1.0)
        res = f1 + (f2 - f1) * 6.0 * hue;
    else if ((2.0 * hue) < 1.0)
        res = f2;
    else if ((3.0 * hue) < 2.0)
        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
    else
        res = f1;
    return res;
}

vec3 hsl2rgb(vec3 hsl) {
    vec3 rgb;

    if (hsl.y == 0.0) {
        rgb = vec3(hsl.z); // Luminance
    } else {
        float f2;

        if (hsl.z < 0.5)
            f2 = hsl.z * (1.0 + hsl.y);
        else
            f2 = hsl.z + hsl.y - hsl.y * hsl.z;

        float f1 = 2.0 * hsl.z - f2;

        rgb.r = hue2rgb(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = hue2rgb(f1, f2, hsl.x);
        rgb.b = hue2rgb(f1, f2, hsl.x - (1.0/3.0));
    }
    return rgb;
}

vec3 hsl2rgb(float h, float s, float l) {
    return hsl2rgb(vec3(h, s, l));
}

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    // This function gets 'called' for each pixel in the image
    // set constants using const. Almost everything is a float,
    // don't forget to add the .0 at the end, else stuff breaks
    const float x_scale = 6.0;
    const float y_scale = 12.0;
    const float time_scaler = 20.0;

    const float width = 3.0;

    // use gl_FragCoord to access the current coordinate
    float hue_1 = (gl_FragCoord.y + iTime * time_scaler) / x_scale;
    float hue_2 = gl_FragCoord.x / y_scale;
    vec3 rgb = hsl2rgb(mod(hue_1 + hue_2, 12.0) / 12.0, 1.0, 0.5);
    // gl_FragColor is a special variable in a fragment shader
    // it is responsible for setting the color of the pixel
    float col = rand(vec2(0.0, floor(gl_FragCoord.x / width)));
    float d = mod(iTime / 3.0, 10.0) - 2.5 + col;
    float col2 = float(min(gl_FragCoord.y/10.0 + d, 1.0 - (gl_FragCoord.y/10.0 + d)) + 0.3 > 0.3);
    float col3 = float(col < col2);
    float holes = float(mod(gl_FragCoord.x, width * 2.0) > width);
    gl_FragColor = vec4(col3 * holes * vec3(0.0, 0.8, 1.0), 1.0);
    // gl_FragColor = vec4(1.);
}
