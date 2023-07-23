#version 330


out vec4 FragColor;
in vec2 texCoord;
uniform vec2 u_resolution;

uniform int u_totalTriangles;

// Absolutley useless code
uniform vec3 spherePosition[3];
uniform vec3 sphereColor[3];
uniform float sphereSize[3];
uniform float sphereEmmision[3];

uniform float u_cameraPitch;
uniform float u_cameraYaw;


uniform vec3 u_cameraPos;

uniform int u_maxBounces;
uniform int u_currentTriangle;
uniform int u_raysPerPixel;


uniform float u_time;


float random(inout int state) {
    state = state * 747796405 + 2891336453;
    int result = ((state >> ((state>>28)+4))^state) * 277803737;

    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}




float RandomValueNormalDistribution(inout int state) {
    float t = 2 * 3.1415926*random(state);
    float r = sqrt(-2 * log(random(state)));
    return r*cos(t);
}

vec3 RandomDirection(inout int state) {
    float x = RandomValueNormalDistribution(state);
    float y = RandomValueNormalDistribution(state);
    float z = RandomValueNormalDistribution(state);
    return normalize(vec3(x,y,z));
}
vec3 RandomHemisphereDirection(vec3 n, inout int state) {
    vec3 dir = RandomDirection(state);
    return dir*sign(dot(n,dir));
}









// Fix Texcoord

vec2 TexCoord = texCoord.yx;


mat2x2 rotate(float angle) {
    return mat2x2(cos(angle), -sin(angle),sin(angle),cos(angle));
}


struct Ray {
    vec3 Origin;
    vec3 Direction;
};

struct Material {
    vec3 color;
    float emmision;
    float roughness;
};

struct HitInfo {
    bool didHit;
    float dist;
    vec3 hitPos;
    vec3 normal;
    Material material;
};

HitInfo RaySphere(Ray ray, vec3 spherePosition, float Radius) {


    vec3 localPosition = ray.Origin-spherePosition; // Sphere position relative to camera

    float a = dot(localPosition, ray.Direction); // Some fancy math
    float b = dot(localPosition,localPosition) - Radius * Radius; // Even more
    float c = a*a - b; // Even more

    HitInfo hitInfo;
    hitInfo.didHit = false;

    if (c >= 0.0) {
        c = sqrt(c);
        float dist = -a-c; // what? but it works
        if (dist >= 0) {
            hitInfo.didHit = true;
            hitInfo.dist = dist;
            hitInfo.hitPos = ray.Origin+ray.Direction*dist;
            hitInfo.normal = normalize(hitInfo.hitPos-spherePosition);
            
            return hitInfo;
        }
       
    }



    
    return hitInfo;

}

HitInfo RayIntersection(Ray ray) {
     HitInfo nearestHit;
     nearestHit.dist = 2147483647;
     nearestHit.didHit = false;
     nearestHit.material.color = vec3(0.0);
     for (int i = 0; i<3; i++) {
         HitInfo hitInfo = RaySphere(ray, spherePosition[i],sphereSize[i]);
         if (hitInfo.didHit && hitInfo.dist < nearestHit.dist) {
            nearestHit = hitInfo;
            nearestHit.material = Material(sphereColor[i],sphereEmmision[i],0.0);
         }

     }
     return nearestHit;
}

vec3 Trace(Ray ray, inout int rngState) {
    vec3 incomingLight = vec3(0);
    vec3 color = vec3(1.0);
    for (int i = 0; i<=u_maxBounces;i++) {
        HitInfo hitInfo = RayIntersection(ray);
        if (hitInfo.didHit) {
            ray.Origin = hitInfo.hitPos;
            ray.Direction = RandomHemisphereDirection(hitInfo.normal, rngState);

            Material material = hitInfo.material;
            vec3 emmitedLight = material.emmision * material.color;
            incomingLight += emmitedLight * color;
            color *= hitInfo.material.color;
        }
        else {
            break;
        }
    };
    return incomingLight;
}

void main() {
    vec2 uv = (TexCoord-0.5)*u_resolution/u_resolution.y;

    Ray ray;
    ray.Origin = u_cameraPos;
    ray.Direction = normalize(vec3(1,uv));
    ray.Direction.zx *= rotate(u_cameraPitch);
    ray.Direction.xy *= rotate(u_cameraYaw);
    
    

    vec2 numPixels = vec2(u_resolution);
    vec2 pixelCoord = vec2(TexCoord) * numPixels;
    int pixelIndex = int(pixelCoord.y * numPixels.x + pixelCoord.x);

    int rngState = pixelIndex;
    vec3 color = vec3(0.0);
    for (int i =0; i < u_raysPerPixel;i++) {

       color += Trace(ray, rngState);
    }
    color /= u_raysPerPixel;

    FragColor = vec4(vec3(color),1.0);

}