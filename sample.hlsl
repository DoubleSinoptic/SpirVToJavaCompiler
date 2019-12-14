


struct Particle
{
	int value;
	int basalt[3];
};

RWStructuredBuffer<Particle> Particles : register(u1);

void main()
{
	Particle botas = { 0, {1, 2, 3} };
	int a = 0;
	Particles[a++] = botas;
	botas.value = a + 1;
	Particles[a++] = botas;
	botas.value = a + 1;
	Particles[a++] = botas;
	Particles[Particles[1].value].value = 51;
}