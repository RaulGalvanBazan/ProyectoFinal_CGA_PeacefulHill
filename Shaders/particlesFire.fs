#version 400

uniform sampler2D ParticleTex;

in float Transp;
in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);
    // Mix with black as it gets older, to simulate a bit of smoke
    FragColor = vec4(mix(vec3(.32, 0.03, 0.03), FragColor.xyz, 0.1 ), FragColor.a);
    FragColor = vec4(mix( vec3(0.4,0,0.6), FragColor.xyz, Transp * 1.8 ), FragColor.a);
    FragColor.a *= Transp;
}
