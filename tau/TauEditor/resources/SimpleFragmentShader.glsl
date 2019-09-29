#version 330 core

in vec3 color;
in vec2 fTexCoord;
in vec3 fNormal;

layout(location = 0) out vec4 out_Color;

uniform sampler2D textureSampler;
uniform sampler2D textureOverlay;

void main(void)
{
    // if(gl_FragCoord.x < 400 && gl_FragCoord.y < 300)
    // {
    //     // out_Color = vec4(color, 1.0);
    //     // out_Color = color;
    //     vec4 texColor = texture(textureSampler, fTexCoord);
    //     vec4 overlayColor = texture(textureOverlay, fTexCoord);
    //     // if(texColor.w < 0.5) { discard; }
    //     // out_Color = mix(texColor, vec4(color, 1.0), 0.7);
    // 	out_Color = mix(texColor, overlayColor, 0.3);
    // }
    // else
    // {
    //     if(gl_FragCoord.x > 400 && gl_FragCoord.y > 300)
    //     {
    //         vec4 texColor = texture(textureSampler, fTexCoord);
    //         vec4 overlayColor = texture(textureOverlay, fTexCoord);
    //         // if(texColor.w < 0.5) { discard; }
    //         // out_Color = mix(texColor, vec4(color, 1.0), 0.7);
    //         out_Color = mix(texColor, overlayColor, 0.3);
    //         out_Color = mix(vec4(fNormal, 1.0), out_Color, 0.5);
    //     }
    //     else
    //     {
    //         if(gl_FragCoord.x < 400 && gl_FragCoord.y > 300)
    //         {
    //             discard;
    //         }
    //         else
    //         {
    //             out_Color = vec4(fNormal, 1.0);   
    //         }
    //     }
    // }

    // float x = gl_FragCoord.x;
    // float y = gl_FragCoord.y;

    // if((x + y) / 1.3333333333 > 400)
    // {
    //     vec4 texColor = texture(textureSampler, fTexCoord);
    //     vec4 overlayColor = texture(textureOverlay, fTexCoord);
    //     out_Color = mix(texColor, overlayColor, 0.3);
    // }
    // else
    // {
    //     if((y - x) / 2 < 30)
    //     {
    //         vec4 texColor = texture(textureSampler, fTexCoord);
    //         vec4 overlayColor = texture(textureOverlay, fTexCoord);
    //         out_Color = mix(texColor, overlayColor, 0.3);
    //         out_Color = mix(vec4(fNormal, 1.0), out_Color, 0.3);
    //     }
    //     else
    //     {
    //         out_Color = vec4(fNormal, 1.0);  
    //     }
    // }
    
    vec4 texColor = texture(textureSampler, fTexCoord);
    vec4 overlayColor = texture(textureOverlay, fTexCoord);
    out_Color = mix(texColor, overlayColor, 0.3);
}
