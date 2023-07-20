#version 330


out vec4 FragColor;
in vec2 texCoord;
uniform vec2 u_resolution;

uniform mat3x3 u_pos1[1];

// triangle degined by vertices v0, v1 and  v2
vec3 triIntersect( in vec3 ro, in vec3 rd, in vec3 v0, in vec3 v1, in vec3 v2 )
{
    vec3 v1v0 = v1 - v0;
    vec3 v2v0 = v2 - v0;
    vec3 rov0 = ro - v0;
    vec3  n = cross( v1v0, v2v0 );
    vec3  q = cross( rov0, rd );
    float d = 1.0/dot( rd, n );
    float u = d*dot( -q, v2v0 );
    float v = d*dot(  q, v1v0 );
    float t = d*dot( -n, rov0 );
    if( u<0.0 || v<0.0 || (u+v)>1.0 ) t = -1.0;
    return vec3( t, u, v );
}









struct Ray {
    vec3 Origin;
    vec3 Direction;
    float minDist;
};








vec3 castRay(Ray ray) {
    ray.minDist = triIntersect(ray.Origin, ray.Direction, u_pos1[0][0], u_pos1[0][1], u_pos1[0][2]).x;

    return vec3(ray.minDist);
}

void main() {
    vec2 uv = vec2(texCoord-0.5)/u_resolution*u_resolution.x;

    Ray ray;
    ray.Origin = vec3(0.0,0.0,0.0);
    ray.Direction = normalize(vec3(1,uv));

    vec3 color = castRay(ray);

    FragColor = vec4(color, 1.0);
}

