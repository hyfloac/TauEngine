float saturate(float value) { return clamp(value, 0.0, 1.0); }
vec2 saturate(vec2 value) { return vec2(saturate(value.x), saturate(value.y)); }
vec3 saturate(vec3 value) { return vec3(saturate(value.x), saturate(value.y), saturate(value.z)); }
vec4 saturate(vec4 value) { return vec4(saturate(value.xy), saturate(value.zw)); }

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453); }

vec3 grain(vec2 texCoord) { return vec3(rand(texCoord)); }
vec4 grain4(vec2 texCoord) { return vec4(grain(texCoord), 1.0); }

float invert(float color) { return 1.0 - color; }
vec2 invert(vec2 color) { return 1.0 - color; }
vec3 invert(vec3 color) { return 1.0 - color; }
vec4 invert(vec4 color) { return 1.0 - color; }
vec4 invertColor(vec4 color) { return vec4(vec3(1.0 - color), 1.0); }
