# SpirVToJavaCompiler
 spirv to java code converter. it sounds funny but it's not a joke.

```hlsl

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

```

Convert's to

```java

void initialize() {
	Rsx.setlf(_vM10,_vO10, Float.intBitsToFloat(0x3));
	Rsx.setlf(_vM15,_vO15, Float.intBitsToFloat(0x0));
	Rsx.setlf(_vM16,_vO16, Float.intBitsToFloat(0x1));
	Rsx.setlf(_vM17,_vO17, Float.intBitsToFloat(0x2));
	Rsx.setlf(_vM18,_vO18, Float.intBitsToFloat(0x3));
	_vM28 = register1.memory;
	_vO28 = register1.offset;
	Rsx.setlf(_vM82,_vO82, Float.intBitsToFloat(0x33));
}
public  Buffer  register1 = null;
private float[] privateStorage = Rsx.allocate(6);
private float[] stackStorage   = Rsx.allocate(128);
private int     sp             = 128;
float[] _vM10 = privateStorage;
int     _vO10 = 0;
float[] _vM15 = privateStorage;
int     _vO15 = 1;
float[] _vM16 = privateStorage;
int     _vO16 = 2;
float[] _vM17 = privateStorage;
int     _vO17 = 3;
float[] _vM18 = privateStorage;
int     _vO18 = 4;
float[] _vM28;
int     _vO28;
float[] _vM82 = privateStorage;
int     _vO82 = 5;
void function() {
_jmp5:
	int _tmp0 = 0;
	_tmp0 += 0;
	float[] _vM90 = _vM28;
	int     _vO90 =_vO28 + _tmp0;
	int _tmp1 = 0;
	float[] _vM92 = _vM90;
	int     _vO92 =_vO90 + _tmp1;
	Rsx.setcpy(_vM92,_vO92, _vM15, _vO15, 1);//STORE
	int _tmp2 = 1;
	float[] _vM94 = _vM90;
	int     _vO94 =_vO90 + _tmp2;
	int _tmp3 = 0;
	float[] _vM96 = _vM94;
	int     _vO96 =_vO94 + _tmp3;
	Rsx.setcpy(_vM96,_vO96, _vM16, _vO16, 1);//STORE
	int _tmp4 = 1;
	float[] _vM98 = _vM94;
	int     _vO98 =_vO94 + _tmp4;
	Rsx.setcpy(_vM98,_vO98, _vM17, _vO17, 1);//STORE
	int _tmp5 = 2;
	float[] _vM100 = _vM94;
	int     _vO100 =_vO94 + _tmp5;
	Rsx.setcpy(_vM100,_vO100, _vM18, _vO18, 1);//STORE
	int _tmp6 = 0;
	_tmp6 += 4;
	float[] _vM107 = _vM28;
	int     _vO107 =_vO28 + _tmp6;
	int _tmp7 = 0;
	float[] _vM109 = _vM107;
	int     _vO109 =_vO107 + _tmp7;
	Rsx.setcpy(_vM109,_vO109, _vM17, _vO17, 1);//STORE
	int _tmp8 = 1;
	float[] _vM111 = _vM107;
	int     _vO111 =_vO107 + _tmp8;
	int _tmp9 = 0;
	float[] _vM113 = _vM111;
	int     _vO113 =_vO111 + _tmp9;
	Rsx.setcpy(_vM113,_vO113, _vM16, _vO16, 1);//STORE
	int _tmp10 = 1;
	float[] _vM115 = _vM111;
	int     _vO115 =_vO111 + _tmp10;
	Rsx.setcpy(_vM115,_vO115, _vM17, _vO17, 1);//STORE
	int _tmp11 = 2;
	float[] _vM117 = _vM111;
	int     _vO117 =_vO111 + _tmp11;
	Rsx.setcpy(_vM117,_vO117, _vM18, _vO18, 1);//STORE
	int _tmp12 = 0;
	_tmp12 += 8;
	float[] _vM124 = _vM28;
	int     _vO124 =_vO28 + _tmp12;
	int _tmp13 = 0;
	float[] _vM126 = _vM124;
	int     _vO126 =_vO124 + _tmp13;
	Rsx.setcpy(_vM126,_vO126, _vM18, _vO18, 1);//STORE
	int _tmp14 = 1;
	float[] _vM128 = _vM124;
	int     _vO128 =_vO124 + _tmp14;
	int _tmp15 = 0;
	float[] _vM130 = _vM128;
	int     _vO130 =_vO128 + _tmp15;
	Rsx.setcpy(_vM130,_vO130, _vM16, _vO16, 1);//STORE
	int _tmp16 = 1;
	float[] _vM132 = _vM128;
	int     _vO132 =_vO128 + _tmp16;
	Rsx.setcpy(_vM132,_vO132, _vM17, _vO17, 1);//STORE
	int _tmp17 = 2;
	float[] _vM134 = _vM128;
	int     _vO134 =_vO128 + _tmp17;
	Rsx.setcpy(_vM134,_vO134, _vM18, _vO18, 1);//STORE
	int _tmp18 = 0;
	_tmp18 += 4;
	_tmp18 += 0;
	float[] _vM135 = _vM28;
	int     _vO135 =_vO28 + _tmp18;
	sp -= 1;
	float[] _vM136 = stackStorage;
	int     _vO136 = sp;
	Rsx.setcpy(_vM136,_vO136,_vM135,_vO135, 1);//LOAD
	int _tmp19 = 0;
	int _tmp20 =  Float.floatToIntBits(_vM136[_vO136]);
	_tmp19 += _tmp20 * 4;
	_tmp19 += 0;
	float[] _vM137 = _vM28;
	int     _vO137 =_vO28 + _tmp19;
	Rsx.setcpy(_vM137,_vO137, _vM82, _vO82, 1);//STORE
_jmpEnd:
	sp += 1;
}

```