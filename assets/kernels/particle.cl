#define MOVE_SPEED                                  0.1f
#define LINEAR_CONVERSION(n, x1, x2, y1, y2)        (((float)((n - x1) * (y2 - y1)) / (x2 - x1)) + y1)

__kernel void particle_init_cube(__global float4 *pos)
{
    const uint id = get_global_id(0);
    const uint size = (uint)cbrt((float)get_global_size(0));

//http://stackoverflow.com/questions/11316490/convert-a-1d-array-index-to-a-3d-array-index
    const int step_x = id % size;
    const int step_y = (id / size) % size;
    const int step_z = id / (size * size);

    pos[id] = (float4){
        LINEAR_CONVERSION(step_x, 0, size, -.5f, .5f),
        LINEAR_CONVERSION(step_y, 0, size, -.5f, .5f),
        LINEAR_CONVERSION(step_z, 0, size, -.5f, .5f),
        1.0f
    };
}

__kernel void particle_init_sphere(__global float4 *pos)
{
    uint const		id = get_global_id(0);
	uint const		global_size = get_global_size(0);
	float		    offset = 2.f / global_size;
	float			y, r, phi, k;

    k = 2.f;
    offset *= k;
	y = ((id * offset * k) - 1.f * k) + (offset / 2.f);
	r = sqrt(k * k - pow(y, 2));
	phi = (id % global_size) * M_PI * (2.f - sqrtf(5.f));
	pos[id] = (float4){
	    cos(phi) * r,
	    y,
	    sin(phi) * r,
	    1.f};
	pos[id] = normalize(pos[id]);
}   

float4 attraction(float4 pos, float4 attractor)
{
    float4 delta = attractor - pos;
    const float damp = 0.5f;
    float dDampedDot = dot(delta, delta) + damp;
    float invDist = 1.0f / sqrt(dDampedDot);
    float invDistCubed = invDist * invDist * invDist;
    return (delta * (invDistCubed * 0.0035f));
}

float4 repulsion(float4 pos, float4 repulsor)
{
    float4 delta = repulsor - pos;
    float targetDistance = sqrt(dot(delta, delta));
    return (delta * (1.0f / (targetDistance * targetDistance * targetDistance)) * -0.000035f);
}

__kernel void particle_update(__global float4 *pos, __global float4 *vel, __global float4 *cursor, __global float *delta_t)
{
    const uint id = get_global_id(0);
    const float bounds = 1.f;
    float4 vVel = vel[id];
    float4 vPos = pos[id];
    float4 destPos = *cursor;

    float weight = destPos.w;
    destPos.w = 1.0f;

    vVel += repulsion(vPos, destPos) * 1 / weight;
    vPos += vVel * *delta_t / 2.f; //DELTA HERE

    if ((vPos.x < -bounds) || (vPos.x > bounds)
        || (vPos.y < -bounds) || (vPos.y > bounds)
        || (vPos.z < -bounds) || (vPos.z > bounds))
        vVel = (-vVel * 0.1f) + attraction(vPos, destPos) * weight;
    else
        pos[id] = vPos;
    pos[id].w = 1.f;
    vel[id] = vVel;
}

__kernel void particle_update_flow(__global float4 *pos, __global float4 *vel, __global float4 *cursor, __global float *delta_t)
{
    float4 a;
    float4 center = *cursor;
    float4 tempCurrPos;
    const uint id = get_global_id(0);
    float4 vPos = pos[id];

    if(center.w < 0.0f){
      // The direction of this vector corresponds to the direction of the gravity.
      // A zero vector will freeze the particles when not interacted with them.
      a = (float4){0,-0.125,0,0};
    } else {
      a = center - vPos - (float4){0,-1,0,0};
      a = normalize(a) * 5 * length(vPos);
    }
    tempCurrPos  = 1.99f * vPos - 0.99f * vPos + a * 0.5f;
    pos[id] = tempCurrPos;
}
