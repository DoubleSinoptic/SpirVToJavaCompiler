struct Particle
{
	float3 Position;
	float3 Velocity;
};

cbuffer Handler : register(c0)
{
	int GroupDim;
	uint MaxParticles;
	float DeltaTime;
	Particle smallParts[4];
	float six[8];
};


RWStructuredBuffer<Particle> Particles : register(u1);

const uint sampleCount = 16;
static float2 poissonDisk[sampleCount] =
{
	float2(0.2770745f, 0.6951455f),
	float2(0.1874257f, -0.02561589f),
	float2(-0.3381929f, 0.8713168f),
	float2(0.5867746f, 0.1087471f),
	float2(-0.3078699f, 0.188545f),
	float2(0.7993396f, 0.4595091f),
	float2(-0.09242552f, 0.5260149f),
	float2(0.3657553f, -0.5329605f),
	float2(-0.3829718f, -0.2476171f),
	float2(-0.01085108f, -0.6966301f),
	float2(0.8404155f, -0.3543923f),
	float2(-0.5186161f, -0.7624033f),
	float2(-0.8135794f, 0.2328489f),
	float2(-0.784665f, -0.2434929f),
	float2(0.9920505f, 0.0855163f),
	float2(-0.687256f, 0.6711345f)
};



#define THREAD_GROUP_X 32
#define THREAD_GROUP_Y 24
#define THREAD_GROUP_TOTAL 768

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void main(uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
	uint index = groupID.x * THREAD_GROUP_TOTAL + groupID.y * GroupDim * THREAD_GROUP_TOTAL + groupIndex;



	Particle particle = Particles[index];
	float3 pxe = {0.0, 0.0 , 0.0 };


	float3 position = particle.Position;
	float3x3 sfe = {0.0, 0.0 , 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0 , 0.0};
	position = mul(position, sfe);
	float3 velocity = particle.Velocity;
	float xAbs = 1.052f;
	// payload

	particle.Position =  position + velocity * DeltaTime * xAbs + poissonDisk[3].x;
	particle.Velocity = pxe + velocity;

	Particles[index] = particle;
}