
cbuffer Handler : register(c0)
{
	float ab0;
	float ab1;
};

static float2 poissonDisk[5] =
{
	float2(1.0, 2.0),
	float2(2.0, 4.0),
	float2(3.0, 6.0),
	float2(4.0, 8.0),
	float2(5.0, 10.0)
};

struct Particle
{
	int value;
	int arr[3];
};

RWStructuredBuffer<Particle> Particles : register(u1);
RWStructuredBuffer<float>	 floats : register(u2);
void main()
{
	Particle part = { 0, {1, 2, 3} };
	int a = 0;
	Particles[a++] = part;
	part.value = a + 1;
	Particles[a++] = part;
	part.value = a + 1;
	Particles[a++] = part;
	Particles[Particles[1].value].value = 51;

	floats[0] = poissonDisk[0].x + poissonDisk[1].y + ab0;
	floats[1] = poissonDisk[0].y + poissonDisk[1].x + ab1;
}