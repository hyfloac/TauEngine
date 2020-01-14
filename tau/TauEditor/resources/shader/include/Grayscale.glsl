vec3 grayscale(vec3 color) { return vec3((color.r + color.g + color.b) / 3.0); }
vec4 grayscale(vec4 color) { return vec4(grayscale(color.rgb), 1.0); }
vec3 grayscaleWeighted(vec3 color) { return vec3(color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722); }
vec4 grayscaleWeighted(vec4 color) { return vec4(grayscaleWeighted(color.rgb), 1.0); }
