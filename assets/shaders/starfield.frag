// Started as Star Nest by Pablo Román Andrioli
// Modifications by Beibei Wang and Huw Bowles.
// This content is under the MIT License.

#version 110

#define iterations 17
#define formuparam 0.53

#define volsteps 22
#define stepsize 0.050

#define zoom   0.800
#define tile   0.850
#define speed  0.02

#define brightness 0.0015
#define darkmatter 0.300
#define distfading 0.760
#define saturation 0.800

uniform vec2 iResolution;
uniform vec2 iOffset;
uniform float iZoom;
uniform float iTime;

void main()
{
	//get coords and direction
	vec2 uv=gl_TexCoord[0].xy-.5;
	uv.y*=iResolution.y/iResolution.x;
	vec3 dir=vec3(uv*zoom,1.);
	float time=(iTime-3311.)*speed;
	vec3 from=vec3(1.1,.25,0.5);
	vec3 forward = vec3(0.,0.,1.0);
	
	// pan (dodgy)
	from += vec3(iOffset.xy, 0.);
	
	//zoom
	float zooom = time * 0.1 + iZoom;
	from += forward* zooom;
	float sampleShift = mod( zooom, stepsize );
	float zoffset = -sampleShift;
	sampleShift /= stepsize; // make from 0 to 1
	
	//volumetric rendering
	float s=0.1;
	vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		vec3 p=from+(s+zoffset)*dir;// + vec3(0.,0.,zoffset);
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) { 
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			//p=abs(p)/max(dot(p,p),0.005)-formuparam; // another interesting way to reduce noise
            float D = abs(length(p)-pa); // absolute sum of average change
            a += i > 7 ? min( 12., D) : D;
			pa=length(p);
		}
		//float dm=max(0.,darkmatter-a*a*.001); //dark matter
		a*=a*a; // add contrast
		//if (r>3) fade*=1.-dm; // dark matter, don't render near
		// brightens stuff up a bit
		float s1 = s+zoffset;
		// need closed form expression for this, now that we shift samples
		float fade = pow(distfading,max(0.,float(r)-sampleShift));
		v+=fade;
        
		// fade out samples as they approach the camera
		if( r == 0 )
			fade *= 1. - sampleShift;
		// fade in samples as they approach from the distance
		if( r == volsteps-1 )
			fade *= sampleShift;
		v+=vec3(2.*s1,4.*s1*s1,16.*s1*s1*s1*s1)*a*brightness*fade; // coloring based on distance
		s+=stepsize;
	}
	v=mix(vec3(length(v)),v,saturation); //color adjust
	gl_FragColor = vec4(v*.01,1.);	
}
