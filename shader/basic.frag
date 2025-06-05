#version 330 core

in vec4 color;
in vec2 TexCoord;

out vec4 outColor;

uniform vec3      iResolution;           // viewport resolution (in pixels)£¬width, height, 1.0
uniform float     iTime;                 // shader playback time (in seconds)


uniform sampler2D outTexture1;
uniform sampler2D outTexture2;

uniform sampler2D texture_diffuses;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{

    
    //outColor = texture(outTexture, TexCoord) * color;
    //vec2 cord2 = vec2(TexCoord.x, 1.0f - TexCoord.y);
    //outColor = mix(texture(outTexture1, TexCoord), texture(outTexture2, TexCoord), 0.5);
    
    outColor = vec4(texture(texture_diffuses, TexCoord));
    
    outColor = vec4(0.1, 0.2,0.3, 1.0);
};