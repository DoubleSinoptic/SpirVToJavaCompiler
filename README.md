# SpirVToJavaCompiler
 spirv to java code converter. it sounds funny but it's not a joke.

```hlsl

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

```

Convert's to

```java

void initialize() {
	Rsx.setlf(privateStorage, _vO11, Float.intBitsToFloat(0x5));
	Rsx.setlf(privateStorage, _vO15, Float.intBitsToFloat(0x3f800000));
	Rsx.setlf(privateStorage, _vO16, Float.intBitsToFloat(0x40000000));
	Rsx.setlf(privateStorage, _vO17, Float.intBitsToFloat(0x3f800000),Float.intBitsToFloat(0x40000000));
	Rsx.setlf(privateStorage, _vO18, Float.intBitsToFloat(0x40800000));
	Rsx.setlf(privateStorage, _vO19, Float.intBitsToFloat(0x40000000),Float.intBitsToFloat(0x40800000));
	Rsx.setlf(privateStorage, _vO20, Float.intBitsToFloat(0x40400000));
	Rsx.setlf(privateStorage, _vO21, Float.intBitsToFloat(0x40c00000));
	Rsx.setlf(privateStorage, _vO22, Float.intBitsToFloat(0x40400000),Float.intBitsToFloat(0x40c00000));
	Rsx.setlf(privateStorage, _vO23, Float.intBitsToFloat(0x41000000));
	Rsx.setlf(privateStorage, _vO24, Float.intBitsToFloat(0x40800000),Float.intBitsToFloat(0x41000000));
	Rsx.setlf(privateStorage, _vO25, Float.intBitsToFloat(0x40a00000));
	Rsx.setlf(privateStorage, _vO26, Float.intBitsToFloat(0x41200000));
	Rsx.setlf(privateStorage, _vO27, Float.intBitsToFloat(0x40a00000),Float.intBitsToFloat(0x41200000));
	Rsx.setlf(privateStorage, _vO28, Float.intBitsToFloat(0x3f800000),Float.intBitsToFloat(0x40000000),Float.intBitsToFloat(0x40000000),Float.intBitsToFloat(0x40800000),Float.intBitsToFloat(0x40400000),Float.intBitsToFloat(0x40c00000),Float.intBitsToFloat(0x40800000),Float.intBitsToFloat(0x41000000),Float.intBitsToFloat(0x40a00000),Float.intBitsToFloat(0x41200000));
	Rsx.setlf(privateStorage, _vO30, Float.intBitsToFloat(0x3));
	Rsx.setlf(privateStorage, _vO35, Float.intBitsToFloat(0x0));
	Rsx.setlf(privateStorage, _vO36, Float.intBitsToFloat(0x1));
	Rsx.setlf(privateStorage, _vO37, Float.intBitsToFloat(0x2));
	Rsx.setlf(privateStorage, _vO38, Float.intBitsToFloat(0x3));
	Rsx.setlf(privateStorage, _vO102, Float.intBitsToFloat(0x33));
	Rsx.setlf(privateStorage, _vO108, Float.intBitsToFloat(0x0));
	Rsx.setlf(privateStorage, _vO112, Float.intBitsToFloat(0x1));
}
final int _vO11 = 0;
final int _vO15 = 11;
final int _vO16 = 12;
final int _vO17 = 13;
final int _vO18 = 15;
final int _vO19 = 16;
final int _vO20 = 18;
final int _vO21 = 19;
final int _vO22 = 20;
final int _vO23 = 22;
final int _vO24 = 23;
final int _vO25 = 25;
final int _vO26 = 26;
final int _vO27 = 27;
final int _vO28 = 29;
final int _vO30 = 39;
final int _vO35 = 40;
final int _vO36 = 41;
final int _vO37 = 42;
final int _vO38 = 43;
final int _vO102 = 44;
final int _vO108 = 45;
final int _vO112 = 46;
public  Buffer  register0 = null;
public  Buffer  register1 = null;
public  Buffer  register2 = null;
private final float[] privateStorage = Rsx.allocate(47);
private final float[] stackStorage   = Rsx.allocate(128);
private int     sp             = 128;
void function() {
	sp -= 11;
	final int _vO14 = 1;
	Rsx.setcpy(privateStorage,_vO14,privateStorage,_vO28,10);//STORE
	final int _vO139 = register1.offset+0+0;
	final int _vO141 = _vO139+0;
	register1.memory[_vO141] = privateStorage[_vO35];//STORE
	final int _vO143 = _vO139+1;
	final int _vO145 = _vO143+0;
	register1.memory[_vO145] = privateStorage[_vO36];//STORE
	final int _vO147 = _vO143+1;
	register1.memory[_vO147] = privateStorage[_vO37];//STORE
	final int _vO149 = _vO143+2;
	register1.memory[_vO149] = privateStorage[_vO38];//STORE
	final int _vO156 = register1.offset+0+4;
	final int _vO158 = _vO156+0;
	register1.memory[_vO158] = privateStorage[_vO37];//STORE
	final int _vO160 = _vO156+1;
	final int _vO162 = _vO160+0;
	register1.memory[_vO162] = privateStorage[_vO36];//STORE
	final int _vO164 = _vO160+1;
	register1.memory[_vO164] = privateStorage[_vO37];//STORE
	final int _vO166 = _vO160+2;
	register1.memory[_vO166] = privateStorage[_vO38];//STORE
	final int _vO173 = register1.offset+0+8;
	final int _vO175 = _vO173+0;
	register1.memory[_vO175] = privateStorage[_vO38];//STORE
	final int _vO177 = _vO173+1;
	final int _vO179 = _vO177+0;
	register1.memory[_vO179] = privateStorage[_vO36];//STORE
	final int _vO181 = _vO177+1;
	register1.memory[_vO181] = privateStorage[_vO37];//STORE
	final int _vO183 = _vO177+2;
	register1.memory[_vO183] = privateStorage[_vO38];//STORE
	final int _vO184 = register1.offset+0+4+0;
	final int _vO185 = sp + 0;
	Rsx.setcpy(stackStorage, _vO185,register1.memory,_vO184, 1);//LOAD
	final int _vO186 = register1.offset+0+Float.floatToIntBits(stackStorage[_vO185])*4+0;
	register1.memory[_vO186] = privateStorage[_vO102];//STORE
	final int _vO187 = _vO14+0+0;
	final int _vO188 = sp + 1;
	Rsx.setcpy(stackStorage, _vO188,privateStorage,_vO187, 1);//LOAD
	final int _vO189 = _vO14+2+1;
	final int _vO190 = sp + 2;
	Rsx.setcpy(stackStorage, _vO190,privateStorage,_vO189, 1);//LOAD
	final int _vO191 = sp + 3;
	Rsx.addF5float(stackStorage, _vO191,stackStorage, _vO188,stackStorage, _vO190);
	final int _vO192 = register0.offset+0;
	final int _vO193 = sp + 4;
	Rsx.setcpy(stackStorage, _vO193,register0.memory,_vO192, 1);//LOAD
	final int _vO194 = sp + 5;
	Rsx.addF5float(stackStorage, _vO194,stackStorage, _vO191,stackStorage, _vO193);
	final int _vO195 = register2.offset+0+0;
	register2.memory[_vO195] = stackStorage[_vO194];//STORE
	final int _vO196 = _vO14+0+1;
	final int _vO197 = sp + 6;
	Rsx.setcpy(stackStorage, _vO197,privateStorage,_vO196, 1);//LOAD
	final int _vO198 = _vO14+2+0;
	final int _vO199 = sp + 7;
	Rsx.setcpy(stackStorage, _vO199,privateStorage,_vO198, 1);//LOAD
	final int _vO200 = sp + 8;
	Rsx.addF5float(stackStorage, _vO200,stackStorage, _vO197,stackStorage, _vO199);
	final int _vO201 = register0.offset+1;
	final int _vO202 = sp + 9;
	Rsx.setcpy(stackStorage, _vO202,register0.memory,_vO201, 1);//LOAD
	final int _vO203 = sp + 10;
	Rsx.addF5float(stackStorage, _vO203,stackStorage, _vO200,stackStorage, _vO202);
	final int _vO204 = register2.offset+0+1;
	register2.memory[_vO204] = stackStorage[_vO203];//STORE
_jmpEnd:
	sp += 11;
}

```