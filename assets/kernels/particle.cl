#define MOVE_SPEED                                  0.1f
#define LINEAR_CONVERSION(n, x1, x2, y1, y2)        (((float)((n - x1) * (y2 - y1)) / (x2 - x1)) + y1)

//__kernel void particle_init(__global float4 *pos)
//{
//    const uint id = get_global_id(0);
//    const uint size = get_global_size(0);
//
////http://stackoverflow.com/questions/11316490/convert-a-1d-array-index-to-a-3d-array-index
//    const int step_x = id % 100;
//    const int step_y = (id / 100) % 100;
//    const int step_z = id / (100 * 100);
//
//    pos[id] = (float4){
//        LINEAR_CONVERSION(step_x, 0, 100, -2, 2),
//        LINEAR_CONVERSION(step_y, 0, 100, -2, 2),
//        LINEAR_CONVERSION(step_z, 0, 100, -2, 2),
//        1.0f
//    };
//}

__kernel void particle_init(__global float4 *pos)
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
	pos[id] = (float4){cos(phi) * r, y, sin(phi) * r, 1.f};
	pos[id] = normalize(pos[id]);
}

//__kernel void particle_init(__global float4 *pos)
//{
//    const uint id = get_global_id(0);
//    const uint size = get_global_size(0);
//
//    const int step_x = id % 100;
//    const int step_y = (id / 100) % 100;
//    const int step_z = id / (100 * 100);

//    float x = LINEAR_CONVERSION(step_x, 0, 100, -2, 2);
//    float y = LINEAR_CONVERSION(step_y, 0, 100, -2, 2);
//    float z = LINEAR_CONVERSION(step_z, 0, 100, -2, 2);

//    pos[id] = (float4){
//        z,
//        sqrtf(fabs(1 - sin(x) * sin(x))) * z,
//        sin(x) + cos(y),
//        1.0f
//    };
//}

////////////////////////////////////////////////////////////////
__kernel void particle(__global float4 *pos, __global float4 *cursor)
{
    const uint id = get_global_id(0);
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
