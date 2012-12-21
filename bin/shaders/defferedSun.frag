#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Normal;
uniform sampler2D Depth;

uniform vec3 EyePos;
uniform vec3 EyeFwd;
uniform vec3 EyeRight;
uniform vec3 EyeUp;

uniform vec3 Direction;

uniform mat4 ShadowProjection;
uniform mat4 InnerShadowProjection;

uniform sampler2D Shadowmap;
uniform sampler2D InnerShadowmap;

uniform float NearZ = 0.1;
uniform float FarZ = 100.0;

uniform int Samples = 4;
uniform int Rings = 2;
uniform float SoftSize = 0.001;

float PI = 3.14159265;

float noise(int x, int y)
{
	float noise = 0;
	int hLine = (y - y / 2 * 2);
	noise = (x - x / 2 * 2) == hLine ? 0.0 : 0.5;
	noise += hLine == 0 ? 0.0 : 0.25;
	
	return noise;
}

float shadow(vec3 pos, float bias, sampler2D map)
{
	float noise = noise(int(gl_FragCoord.x),int(gl_FragCoord.y));

	float step = PI*2.0 / float(Samples);
	float shadowValue = 1.0 / float(Samples) / float(Rings);
	float ringcoord = step * noise;
	vec2 shift;
	float shadow = 0;
	for (int r = 1 ; r <= Rings; r++)
	{
		for (int i = 0 ; i < Samples; ++i)
		{	
			ringcoord += step;
			shift = vec2(cos(ringcoord),sin(ringcoord));
			shift *= SoftSize * r;
			shadow += texture2D(map,pos.xy + shift).x + bias > pos.z ? shadowValue : 0.0;
		}
		ringcoord = step * (noise + 0.125 + 1.0 / Rings);
	}
	return shadow;
}
 
void main(void) {
    // Pass through our original color with full opacity.
    // gl_FragColor = vec4(vTexCoord.x,vTexCoord.y,0.0,1.0);
	vec3 N = texture2D(Normal, vTexCoord).rgb * 2 - 1;
	
	float depth = texture2D(Depth, vTexCoord).x;
	depth = 2.0 * depth - 1.0;
	depth = (2.0 * NearZ) / (FarZ + NearZ - depth * (FarZ - NearZ));
	vec2 screenpos = vTexCoord * 2 - 1;
	vec3 GPos = EyePos + (EyeFwd + screenpos.x * EyeRight + screenpos.y * EyeUp) * depth;
	
	float angle = clamp(dot(-Direction, N),0,1);
	
	float shadow = 1;
	
	vec4 LPos = InnerShadowProjection * vec4(GPos,1);
	LPos /= LPos.w;
	if(LPos.x > 0 && LPos.x < 1 &&
		LPos.y > 0 && LPos.y < 1)
	{
		shadow = shadow(LPos.xyz,0.0002/angle,InnerShadowmap);
	}
	else
	{
		LPos = ShadowProjection * vec4(GPos,1);
		LPos /= LPos.w;
		
		if(LPos.x > 0 && LPos.x < 1 &&
		LPos.y > 0 && LPos.y < 1)
		{
			shadow = shadow(LPos.xyz,0.0006/angle,Shadowmap);
		}
	}

	vec3 light = vec3(1.1,1.0,0.9);
	vec3 ambient = vec3(0.1,0.1,0.15);

	vec3 illumination = angle*light*shadow+ambient;
	
	gl_FragColor = vec4(illumination,1);//vec4(illumination,1.0);//texture2D(Depth, vTexCoord);
	//gl_FragColor = vec4(illumination,1.0);
}