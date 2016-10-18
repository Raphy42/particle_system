/*
** java.util.Random
** (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1)
*/
#define MOVE_SPEED  0.1f

float				frand(uint *seed)
{
	uint const			salt = 209863451;
	uint const			t = *seed ^ (*seed << 11);

	*seed = salt ^ (salt >> 19) ^ (t ^ (t >> 8));
	return (*seed / (float)0xFFFFFFFF);
}

__kernel void particle_init(__global float4 *pos)
{
    uint			seed;
    uint const		id = get_global_id(0);

    seed = 0xdeadbeef + (id | (id << 16)) * id;
    pos[id] = (float4){
    	frand(&seed) * 2.f - 1.f,
    	frand(&seed) * 2.f - 1.f,
    	frand(&seed) * 2.f - 1.f,
    	1.f
    };
}

__kernel void particle(__global float4 *pos, __global float4 *cursor)
{
    uint const id = get_global_id(0);
    float x_distance = cursor->x - pos[id][0];
    float y_distance = cursor->y - pos[id][1];
    float z_distance = cursor->z - pos[id][2];
    float distance = sqrt(x_distance * x_distance + y_distance * y_distance);

    if (distance > 1.0f)
    {
        pos[id][0] += x_distance * 0.5f;
        pos[id][1] += y_distance * 0.5f;
        pos[id][2] += z_distance * 0.5f;
    }
}