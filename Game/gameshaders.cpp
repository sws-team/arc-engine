#include "levelobject.h"

const std::string ShadersFactory::WAVE_SHADER =
"#ifdef GL_ES\n\
precision lowp float;\n\
#endif\n\
uniform float time;\n\
uniform vec2 mouse;\n\
uniform vec2 resolution;\n\
uniform sampler2D currentTexture;\n\
\n\
#define MAX_ITER 6\n\
\n\
void main( void ) {\n\
	vec2 uv = gl_FragCoord.xy / resolution.xy;\n\
	vec4 texture_color = texture2D(currentTexture, gl_TexCoord[0].xy);\n\
	vec2 sp = uv.xy;\n\
	vec2 p = sp * 15.0 - vec2(20.0);\n\
	vec2 i = p;\n\
	float c = 1.0; // brightness; larger -> darker\n\
	float inten = 0.025; // brightness; larger -> brighter\n\
	float speed = 7.5; // larger -> slower\n\
	float speed2 = 0.5; // larger -> slower\n\
	float freq = 0.5; // ripples\n\
	float xflow = 1.5; // flow speed in x direction\n\
	float yflow = 2.0; // flow speed in y direction\n\
\n\
	for (int n = 0; n < MAX_ITER; n++)\n\
	{\n\
		float t = time * (1.0 - (3.0 / (float(n) + speed)));\n\
		i = p + vec2(cos(t - i.x * freq) + sin(t + i.y * freq) + (time * xflow), sin(t - i.y * freq) + cos(t + i.x * freq) + (time * yflow));\n\
		c += 1.0 / length(vec2(p.x / (sin(i.x + t * speed2) / inten), p.y / (cos(i.y + t * speed2) / inten)));\n\
	}\n\
\n\
	c /= float(MAX_ITER);\n\
	c = 1.5 - sqrt(c);\n\
	gl_FragColor = texture_color + vec4(vec3(c * c * c * c), 0.0);\n\
}";

 /*
 "#version 130\n\
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
 */

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

const std::string ShadersFactory::SMOKE_SHADER =
"uniform vec2 resolution;\n\
uniform float time;\n\
\n\
#ifdef GL_ES\n\
precision mediump float;\n\
#endif\n\
\n\
#define NUM_OCTAVES 16\n\
\n\
mat3 rotX(float a) {\n\
	float c = cos(a);\n\
	float s = sin(a);\n\
	return mat3(\n\
				1, 0, 0,\n\
				0, c, -s,\n\
				0, s, c\n\
				);\n\
}\n\
mat3 rotY(float a) {\n\
	float c = cos(a);\n\
	float s = sin(a);\n\
	return mat3(\n\
				c, 0, -s,\n\
				0, 1, 0,\n\
				s, 0, c\n\
				);\n\
}\n\
\n\
float random(vec2 pos) \n\
{\n\
	return fract(sin(dot(pos.xy, vec2(12.9898, 78.233))) * 43758.5453123);\n\
}\n\
\n\
float noise(vec2 pos) {\n\
	vec2 i = floor(pos);\n\
	vec2 f = fract(pos);\n\
	float a = random(i + vec2(0.0, 0.0));\n\
	float b = random(i + vec2(1.0, 0.0));\n\
	float c = random(i + vec2(0.0, 1.0));\n\
	float d = random(i + vec2(1.0, 1.0));\n\
	vec2 u = f * f * (3.0 - 2.0 * f);\n\
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;\n\
}\n\
\n\
float fbm(vec2 pos) {\n\
	float v = 0.0;\n\
	float a = 0.5;\n\
	vec2 shift = vec2(100.0);\n\
	mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.5));\n\
	for (int i=0; i<NUM_OCTAVES; i++) {\n\
		v += a * noise(pos);\n\
		pos = rot * pos * 2.0 + shift;\n\
		a *= 0.5;\n\
	}\n\
	return v;\n\
}\n\
\n\
void main()\n\
{\n\
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);\n\
	\n\
	float t = 0.0, d;\n\
	\n\
	float time2 = time * 10.0;\n\
	\n\
	vec2 q = vec2(0.0);\n\
	q.x = fbm(p + 0.00 * time2);\n\
	q.y = fbm(p + vec2(1.0));\n\
	vec2 r = vec2(0.0);\n\
	r.x = fbm(p + 1.0 * q + vec2(1.7, 9.2) + 0.15 * time2);\n\
	r.y = fbm(p + 1.0 * q + vec2(8.3, 2.8) + 0.126 * time2);\n\
	float f = fbm(p + r);\n\
	vec3 color = mix(\n\
				vec3(0.101961, 0.619608, 1.666667),\n\
				vec3(0.666667, 0.666667, 1.498039),\n\
				clamp((f * f) * 4.0, 0.0, 1.0)\n\
				);\n\
	\n\
	color = mix(\n\
				color,\n\
				vec3(0, 0, 0.164706),\n\
				clamp(length(q), 0.0, 1.0)\n\
				);\n\
	\n\
	color = mix(\n\
				color,\n\
				vec3(0.666667, 1, 1),\n\
				clamp(length(r.x), 0.0, 1.0)\n\
				);\n\
	\n\
	color = (f *f * f + 0.6 * f * f + 0.5 * f) * color;\n\
	\n\
	gl_FragColor = vec4(color, 0.5);\n\
	\n\
}";
