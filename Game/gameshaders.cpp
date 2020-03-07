#include "levelobject.h"

const std::string ShadersFactory::WAVE_SHADER = "#version 130\n\
uniform sampler2D currentTexture;\n\
uniform float time;\n\
uniform vec2 resolution;\n\
void main()\n\
{\n\
	vec2 uv = gl_FragCoord.xy / resolution.xy;\n\
	vec4 texture_color = texture2D(currentTexture, gl_TexCoord[0].xy);\n\
	vec4 k = vec4(time)*2.8;\n\
	k.xy = uv * 20.0;\n\
	float val1 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));\n\
	float val2 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.2));\n\
	float val3 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));\n\
	vec4 color = vec4 ( pow(min(min(val1,val2),val3), 5.0) * 3.0)+texture_color;\n\
	gl_FragColor = color;\n\
}";

const std::string ShadersFactory::MOVING_SHADER =
"uniform vec2 iResolution;\n\
uniform float iTime;\n\
uniform vec2 iMouse;\n\
uniform float wave_phase;\n\
uniform vec2 wave_amplitude;\n\
void main()\n\
{\n\
		vec4 vertex = gl_Vertex;\n\
		vertex.x += cos(gl_Vertex.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x\n\
		+ sin(gl_Vertex.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;\n\
		vertex.y += sin(gl_Vertex.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y\n\
		+ cos(gl_Vertex.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;\n\
		gl_Position = gl_ModelViewProjectionMatrix * vertex;\n\
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n\
		gl_FrontColor = gl_Color;\n\
}";

