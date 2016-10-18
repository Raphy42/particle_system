/*
** java.util.Random
** (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1)
*/

float frand(long *seed)
{
   // return (((*seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1)) / (float)0xFFFFFFFF);
   return 1.f;
}

__kernel void particle_init(__global float4 *pos)
{
    long seed = get_global_id(0);
    pos[seed] = (float4){
        frand(&seed),
        frand(&seed),
        frand(&seed),
        frand(&seed)
    };
}