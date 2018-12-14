#version 330 core

out vec4 FragColor;

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define SPHERE 0
#define PLANE 1
#define NO_INTERSECT 2
#define DISPLACEMENT_FACTOR 0.1

// Data structure for raymarching results
struct PrimitiveDist {
    float dist;
    int primitive; // Can be SPHERE, PLANE, or NO_INTERSECT
};

// Helper function for tri3.
float tri(in float x) {
	return abs(fract(x)-.5);
}

// Triangle noise. Use it as a sample displacement map for task 7.
vec3 tri3(in vec3 p) {
    return vec3(tri(p.z+tri(p.y*1.)), 
                tri(p.z+tri(p.x*1.)), 
                tri(p.y+tri(p.x*1.)));
}

// TODO [Task 8] Make a displacement map
// You can check out tri3 above and the functions in the handout as inspiration
float calcDisplacement(in vec3 p) {
    return pow(sin(p.x) + cos(p.y), 2.0);
}

// TODO [Task 6] Implement triplanar texture mapping
// If you want, you can play around with the textures in iChannels 0 and 1
// The textures should show no distortion
vec3 texCube( sampler2D sam, in vec3 p, in vec3 n )
{
    vec4 xtex = texture(sam, p.yz);
    vec4 ytex = texture(sam, p.xz);
    vec4 ztex = texture(sam, p.xy);
    return xtex.rgb * abs(n.x) +  ytex.rgb * abs(n.y) + ztex.rgb * abs(n.z);
}

// Signed distance to the twisted sphere.
float sdTwistedSphere(vec3 p) {
    vec3 spherePosition = vec3(0.0, 0.25, 0.0);
    float radius = 1.5;
    float primitive = length(p - spherePosition) - radius;
    return primitive + calcDisplacement(p);
}

float sdFloor(vec3 p) {
    return p.y;
}

PrimitiveDist map(vec3 p) {
    // TODO [Task 3] Implement distance map
    float closest = min(sdFloor(p), sdTwistedSphere(p));
    float planeDist = sdFloor(p);
    float sphereDist = sdTwistedSphere(p);
    if (planeDist < sphereDist) {
    	return PrimitiveDist(planeDist, PLANE);
    } else {
        return PrimitiveDist(sphereDist, SPHERE); 
    }
}

// TODO [Task 4] Calculate surface normals
const float epsilon = 0.001;
vec2 e = vec2(epsilon, 0.0); // For swizzling
vec3 calcNormal(vec3 p) {
    vec3 normal = vec3(map(p + e.xyy).dist - map(p - e.xyy).dist, 
                       map(p + e.yxy).dist - map(p - e.yxy).dist, 
                       map(p + e.yyx).dist - map(p - e.yyx).dist);
    return normalize(normal);
}

float shadow(vec3 ro, vec3 rd, float k) {
    float marchDist = 0.001;
    float boundingVolume = 25.0;
    float darkness = 1.0;
    float threshold = 0.001;
    
    for(int i = 0; i < 30; i++) {
        if(marchDist > boundingVolume) continue;
        float h = map(ro + rd * marchDist).dist;
        // TODO [Task 7] Modify the loop to implement soft shadows
        //if (h < threshold) {
            darkness = min(darkness, k*h/marchDist);
        //    break;
        //}
        marchDist += h * 0.7;
    }
    
    return darkness;
}


PrimitiveDist raymarch(vec3 ro, vec3 rd) {
    
    // TODO [Task 2] Implement ray marching algorithm
    // Fill in parameters
    float marchDist = 0.001;
    float boundingDist = 50.0;
    float threshold = 0.001;
    
    // Fill in the iteration count
    for (int i = 0; i < 1000; i++) {
        // Fill in loop body
        PrimitiveDist currentDist = map(ro + marchDist * rd);
        marchDist += 0.1 * currentDist.dist;
        
        if (currentDist.dist < threshold) {
            return PrimitiveDist(marchDist, currentDist.primitive);
        }
        if (marchDist > boundingDist) {
        	break;
        }
    }    
    return PrimitiveDist(-1.0, NO_INTERSECT);
}

vec3 render(vec3 ro, vec3 rd, float t, int which) {
    
    // Col is the final color of the current pixel.
    vec3 col = vec3(0.);
    vec3 pos = ro + rd * t;
    // Light vector
    vec3 lig = normalize(vec3(1.0,0.6,0.5));
    
    // Normal vector
    vec3 nor = calcNormal(pos);
    
    // Ambient
    float ambient = 0.1;
    // Diffuse
    float diffuse = clamp(dot(nor, lig), 0.0, 1.0);
    // Specular
    float shineness = 32.0; 
    float specular = pow(clamp(dot(rd, reflect(lig, nor)), 0.0, 1.0), 32.0);
    
    float darkness = shadow(pos, lig, 18.0);
    // Applying the phong lighting model to the pixel.
    col += vec3(((ambient + diffuse + specular) * darkness));
    
    // TODO [Task 5] Assign different intersected objects with different materials
    // Make things pretty!
    vec3 material = vec3(0.0);
    if (which == PLANE) {
        material = texCube(iChannel0, pos, nor);
    } else if (which == SPHERE) {
        material = texCube(iChannel1, pos, nor);
    } else {
        material = vec3(0.5);
    }
    
    // Blend the material color with the original color.
    col = mix(col, material, 0.4);
    
    return col;
}

void main()
{
    vec4 fragCoord = gl_FragCoord;
   
    vec3 rayOrigin = vec3(6.0 * sin(iTime * .3), 4.8, 6.0 * cos(iTime * .3));
    
    float focalLength = 2.0;
    
    // The target we are looking at
    vec3 target = vec3(0.0);
    // Look vector
    vec3 look = normalize(rayOrigin - target);
    // Up vector
    vec3 up = vec3(0.0, 1.0, 0.0);
    
    // Set up camera matrix
    vec3 cameraForward = -look;
    vec3 cameraRight = normalize(cross(cameraForward, up));
    vec3 cameraUp = normalize(cross(cameraRight, cameraForward)); 
  
    // TODO [Task 1] Construct the ray direction vector
    vec2 uv = vec2(fragCoord.x/iResolution.x*2.0 - 1.0, -1.0 + fragCoord.y/iResolution.y*2.0);
    uv.x = uv.x * iResolution.x/iResolution.y;
    vec3 rayDirection = vec3(uv.x, uv.y, focalLength);
    rayDirection = cameraRight * rayDirection.x + cameraUp * rayDirection.y + cameraForward * rayDirection.z;
    rayDirection = normalize(rayDirection);
    
      
    PrimitiveDist rayMarchResult = raymarch(rayOrigin, rayDirection);
    vec3 col = vec3(0.0);
    if (rayMarchResult.primitive != NO_INTERSECT) {
      col = render(rayOrigin, rayDirection, rayMarchResult.dist, rayMarchResult.primitive);
    }
    
    fragColor = vec4(col, 1.0);
}