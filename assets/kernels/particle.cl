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

__kernel void particle_init_cube_with_color(__global float4 *pos, __global float4 *color)
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
        color[id] = (float4){
            LINEAR_CONVERSION(step_x, 0, size, 0.f, 1.f),
            LINEAR_CONVERSION(step_y, 0, size, 0.f, 1.f),
            LINEAR_CONVERSION(step_z, 0, size, 0.f, 1.f),
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
    const float damp = 100.f;
    float dDampedDot = dot(delta, delta) + damp;
    float invDist = 1.0f / sqrt(dDampedDot);
//    float invDist = dDampedDot;
    float invDistCubed = invDist * invDist;
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

    vVel += attraction(vPos, destPos) * 12;
    vPos += vVel * *delta_t / 2.f; //DELTA HERE
    pos[id] = vPos;
    pos[id].w = 1.f;
    vel[id] = vVel;
}

__kernel void particle_update_flow(__global float4 *pos, __global float4 *vel, __global float4 *cursor, __global float *delta_t)
{
    const uint id = get_global_id(0);
    float4 vPos = pos[id];
    float4 center = *cursor;
    float weight = center.w;
    center.w = 1.0f;
    float4 delta = center - vPos;
    float d = 1 / (dot(delta, delta) + 1000.f);

    vel[id] += 2 * d * normalize(delta);
    vPos += vel[id];
    pos[id] = vPos;
}

float4 get_rainbow_color(int i)
{
    const float		frequency = .8f;
    float4 out;

	if (i == 5000)
		return ((float4){0.f, 0.f, 0.f, 0.f});
	out.x = sin(frequency * i + 0);
	out.y = sin(frequency * i + 2);
	out.z = sin(frequency * i + 4);
	out.w = 1.0f;
	return (out);
}


__kernel void particle_init_mandelbrot(__global float4 *pos, __global float4 *color) {
    const uint id = get_global_id(0);
    const uint size = (uint)sqrt((float)get_global_size(0));
    int nx = id % size;
    int ny = id / size;
    float4 z, c;

    c = pos[id];

    c.x = (((float)nx / (float)size - 0.5f) * 2.2f - 0.7f);
    c.y = ((float)ny / (float)size - 0.5f) * 2.2f - 0.0f;

    int i;
    z = c;
    for(i = 0; i < 50000; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    z.z = 0.f;
    z.w = 1.0f;
    color[id] = normalize(get_rainbow_color(i));
    pos[id] = z;
}
