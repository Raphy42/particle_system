__kernel void sine_wave(__global float4 *pos, uint width, uint height, float time)
{
    uint x = get_global_id(0);
    uint y = get_global_id(1);

    float u = x / (float) width;
    float v = y / (float) height;
    u = u * 2.0f - 1.0f;
    v = v * 2.0f - 1.0f;

    float freq = 4.0f;
    float w = sin(u * freq + time) * cos(v * freq + time) * 0.5f;

    pos[y * width + x] = (float4)(u, w, v , 1.0f);
}
