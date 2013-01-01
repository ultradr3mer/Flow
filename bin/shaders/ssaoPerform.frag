#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Normal;
uniform sampler2D Diffuse;
uniform vec2 RenderSize;

uniform float Size = 0.03;
uniform float Falloff = 0.000002;
uniform float FalloffStrength = 0.07;
uniform float Strength = 2.0;
uniform int Samples = 8;
uniform float DistanceScaling = 30.0;
uniform float Inertia = 0.5;

vec3 sphere[64] = {
vec3(-0.065134, -0.027501, -0.997497),
vec3(0.284917, -0.736596, -0.613392),
vec3(-0.977572, 0.124288, 0.170019),
vec3(0.757427, -0.580219, -0.299417),
vec3(-0.016289, -0.763434, 0.645680),
vec3(0.479536, -0.587557, -0.651784),
vec3(-0.903781, -0.077046, 0.421003),
vec3(0.915886, 0.086487, -0.392010),
vec3(-0.379665, 0.433646, -0.817194),
vec3(0.357376, 0.612152, -0.705374),
vec3(-0.200729, 0.071282, 0.977050),
vec3(0.647494, 0.019002, -0.761834),
vec3(-0.135289, 0.130479, -0.982177),
vec3(-0.899824, -0.431633, 0.063326),
vec3(-0.765379, -0.610550, 0.203528),
vec3(-0.386866, -0.636189, -0.667531),
vec3(-0.595616, 0.797217, -0.098422),
vec3(-0.361654, -0.290429, -0.885922),
vec3(0.267418, -0.779365, 0.566637),
vec3(-0.320393, 0.946450, 0.039766),
vec3(-0.096262, -0.652159, 0.751945),
vec3(0.366712, -0.184770, 0.911801),
vec3(0.624093, 0.777376, 0.078707),
vec3(0.091790, 0.992886, -0.075839),
vec3(0.173094, 0.667210, 0.724479),
vec3(0.460127, -0.689531, 0.559313),
vec3(0.113034, -0.000217, 0.993591),
vec3(-0.760539, 0.609797, 0.222999),
vec3(-0.258754, 0.845234, -0.467574),
vec3(0.724804, 0.108939, 0.680288),
vec3(0.809226, 0.532463, -0.248268),
vec3(0.877364, 0.323458, 0.354411),
vec3(0.162898, -0.091173, -0.982421),
vec3(-0.128159, 0.884686, -0.448225),
vec3(-0.355576, -0.917962, 0.175817),
vec3(-0.108548, -0.729584, 0.675222),
vec3(0.276698, 0.960485, -0.030122),
vec3(-0.856049, 0.171906, 0.487472),
vec3(0.946043, -0.312943, -0.084078),
vec3(0.678131, 0.548761, 0.488876),
vec3(-0.736203, 0.647119, 0.198096),
vec3(-0.683739, -0.558199, 0.470016),
vec3(-0.637098, 0.757058, 0.144810),
vec3(0.111726, 0.708422, -0.696890),
vec3(0.322506, -0.934656, -0.149693),
vec3(0.997438, -0.062817, 0.034211),
vec3(-0.488286, 0.713071, 0.503098),
vec3(-0.412233, -0.625915, -0.662038),
vec3(0.921239, 0.388659, -0.016205),
vec3(-0.916307, -0.027682, 0.399518),
vec3(0.673913, -0.220919, -0.705008),
vec3(0.576954, -0.391473, -0.716849),
vec3(-0.301851, 0.871812, 0.385785),
vec3(0.894016, 0.423273, -0.146886),
vec3(-0.146438, -0.328095, 0.933226),
vec3(0.516582, -0.502150, -0.693533),
vec3(-0.666064, -0.377419, 0.643361),
vec3(0.344370, 0.707356, -0.617298),
vec3(-0.763698, 0.119656, 0.634388),
vec3(-0.724642, -0.017859, -0.688894),
vec3(-0.734486, 0.495179, 0.464034),
vec3(-0.815153, -0.375747, -0.440840),
vec3(0.034232, -0.930581, 0.364483),
vec3(-0.885337, 0.138540, 0.443831)
};

vec4 readSample(in vec2 coord)
{
	vec4 sample = texture(Normal,coord);
	sample.xyz = sample.xyz * 2 - 1;
	sample.a *= 2;
	return sample;
}

int noise()
{
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	int noise = 0;
	int hLine = (y - y / 2 * 2);
	noise = (x - x / 2 * 2) == hLine ? 0 : Samples * 2;
	noise += hLine == 0 ? 0 : Samples;

	return noise;
}

void main(void) {
	vec3 outVal = vec3(0,0,0);
	
	vec2 rastersize = 1.0 / textureSize(Normal,0);
	float aspect = RenderSize.x/RenderSize.y;

	//grab curent pixel and convert to screenspace
	vec4 curPixel = readSample(vTexCoord);
	vec3 screenspaceCoords = vec3(vTexCoord,curPixel.a);
	
	//early out sky
	if(curPixel.a > 1.999){
		gl_FragColor = vec4(0,0,0,1);
		return;
	}
	
	//vector randomisation grab random 8 of 32 vectors
	int stride = noise();
	
	vec3 ray, sampleCoords;
	vec4 sample;
	float depthDifference, normalDifference;
	float ao = 0.0;
	for (int i = 0 ; i < Samples; ++i)
	{	
		// grab ray vector, multiply by size uniform
		// scale down by size
		ray = sphere[stride+i] * Size;
		
		// if the ray is outside the hemisphere then change direction
		ray = sign(dot(ray,curPixel.rgb) )*ray;
		
		// compensate aspect
		ray.x /= aspect;
		
		//get screenspace coords /convert them
		sampleCoords = screenspaceCoords + ray;
		
		// get the depth of the sample fragment and get differece
		sample = readSample(sampleCoords.xy);
		
		if(sample.a <  1.999)
		{
			// calculate the difference between the normals as a weight
			normalDifference = (1-dot(sample.xyz,curPixel.xyz));
			
			depthDifference = sampleCoords.z-sample.a;
			//depthDifference *= DistanceScaling;
			
			
			// the falloff equation, starts at falloff and is kind of 1/x^2 falling
			ao += step(Falloff,depthDifference)*(1.0-smoothstep(Falloff,FalloffStrength,depthDifference))*normalDifference;
		}
	}
	ao *= Strength/Samples;
	ao = 1.0 -ao;
	
	ao = ao * (1-Inertia) + texture2D(Diffuse,vTexCoord).x * Inertia;
	
	gl_FragColor = vec4(curPixel.rgb * 0.5 + 0.5, ao);
	//gl_FragColor = vec4(ao,ao,ao, 1.0);
}