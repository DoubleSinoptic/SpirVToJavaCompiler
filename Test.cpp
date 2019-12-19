
#include <stdlib.h>
#include <memory>
#include <initializer_list>

struct _Float
{
	inline float intBitsToFloat(int b)
	{
		return *((float*)&b);
	}
	inline int floatToIntBits(float b)
	{
		return *((int*)& b);
	}
};
_Float Float;

struct _Rsx
{
	template<typename... Args>
	inline void setlf(float* addr, int offset, Args... args) {
		float ge[] = { args...};
		for (int i = 0; i < sizeof...(args); i++) {
			addr[i + offset] = ge[i];
		}
	}

	inline void setcpy(float* dst, int dstOffset, const float* addr, int offset, int length)
	{
		for (int i = 0; i < length; i++) {
			dst[i + dstOffset] = addr[i + offset];
		}
	}

	float* allocate(int size)
	{
		float* d = new float[size];
		memset(d, 0, sizeof(float)*size);
		return d;
	}

	inline void addF5float(float* dst, int dstOffset, float* a, int ao, float* b, int bo)
	{
		dst[dstOffset] = a[ao] + b[bo];
	}

	inline void addF6sint32(float* dst, int dstOffset, float* a, int ao, float* b, int bo)
	{
		dst[dstOffset] = Float.intBitsToFloat(Float.floatToIntBits(a[ao]) + Float.floatToIntBits(b[bo]));
	}

	inline void divF6sint32(float* dst, int dstOffset, float* a, int ao, float* b, int bo)
	{
		dst[dstOffset] = Float.intBitsToFloat(Float.floatToIntBits(a[ao]) / Float.floatToIntBits(b[bo]));
	}

	inline void bitwiseAndF6sint32(float* dst, int dstOffset, float* a, int ao, float* b, int bo)
	{
		dst[dstOffset] = Float.intBitsToFloat(Float.floatToIntBits(a[ao]) & Float.floatToIntBits(b[bo]));
	}

	inline void signedToFloatF5float(float* dst, int dstOffset, float* a, int ao)
	{
		dst[dstOffset] = Float.floatToIntBits(a[ao]);
	}

	inline void bitwiseXorF6sint32(float* dst, int dstOffset, float* a, int ao, float* b, int bo)
	{
		dst[dstOffset] = Float.intBitsToFloat(Float.floatToIntBits(a[ao]) ^ Float.floatToIntBits(b[bo]));
	}
	
};

#define null {}
#define public
#define private 
#define final

struct Buffer 
{
	float*	memory;
	int		offset;
};

_Rsx Rsx;

struct Sample 
{
	void initialize() {
		Rsx.setlf(privateStorage, _vO11, Float.intBitsToFloat(0x5));
		Rsx.setlf(privateStorage, _vO15, Float.intBitsToFloat(0x3f800000));
		Rsx.setlf(privateStorage, _vO16, Float.intBitsToFloat(0x40000000));
		Rsx.setlf(privateStorage, _vO17, Float.intBitsToFloat(0x3f800000), Float.intBitsToFloat(0x40000000));
		Rsx.setlf(privateStorage, _vO18, Float.intBitsToFloat(0x40800000));
		Rsx.setlf(privateStorage, _vO19, Float.intBitsToFloat(0x40000000), Float.intBitsToFloat(0x40800000));
		Rsx.setlf(privateStorage, _vO20, Float.intBitsToFloat(0x40400000));
		Rsx.setlf(privateStorage, _vO21, Float.intBitsToFloat(0x40c00000));
		Rsx.setlf(privateStorage, _vO22, Float.intBitsToFloat(0x40400000), Float.intBitsToFloat(0x40c00000));
		Rsx.setlf(privateStorage, _vO23, Float.intBitsToFloat(0x41000000));
		Rsx.setlf(privateStorage, _vO24, Float.intBitsToFloat(0x40800000), Float.intBitsToFloat(0x41000000));
		Rsx.setlf(privateStorage, _vO25, Float.intBitsToFloat(0x40a00000));
		Rsx.setlf(privateStorage, _vO26, Float.intBitsToFloat(0x41200000));
		Rsx.setlf(privateStorage, _vO27, Float.intBitsToFloat(0x40a00000), Float.intBitsToFloat(0x41200000));
		Rsx.setlf(privateStorage, _vO28, Float.intBitsToFloat(0x3f800000), Float.intBitsToFloat(0x40000000), Float.intBitsToFloat(0x40000000), Float.intBitsToFloat(0x40800000), Float.intBitsToFloat(0x40400000), Float.intBitsToFloat(0x40c00000), Float.intBitsToFloat(0x40800000), Float.intBitsToFloat(0x41000000), Float.intBitsToFloat(0x40a00000), Float.intBitsToFloat(0x41200000));
		Rsx.setlf(privateStorage, _vO30, Float.intBitsToFloat(0x3));
		Rsx.setlf(privateStorage, _vO35, Float.intBitsToFloat(0x0));
		Rsx.setlf(privateStorage, _vO36, Float.intBitsToFloat(0x1));
		Rsx.setlf(privateStorage, _vO37, Float.intBitsToFloat(0x2));
		Rsx.setlf(privateStorage, _vO38, Float.intBitsToFloat(0x3));
		Rsx.setlf(privateStorage, _vO57, Float.intBitsToFloat(0x4));
		Rsx.setlf(privateStorage, _vO123, Float.intBitsToFloat(0x33));
		Rsx.setlf(privateStorage, _vO129, Float.intBitsToFloat(0x0));
		Rsx.setlf(privateStorage, _vO133, Float.intBitsToFloat(0x1));
		Rsx.setlf(privateStorage, _vO276, Float.intBitsToFloat(0xfffffffd));
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
	final int _vO57 = 44;
	final int _vO123 = 45;
	final int _vO129 = 46;
	final int _vO133 = 47;
	final int _vO276 = 48;
	public  Buffer  register0 = null;
	public  Buffer  register1 = null;
	public  Buffer  register2 = null;
	private final float* privateStorage = Rsx.allocate(49);
	private final float* stackStorage = Rsx.allocate(128);
	private int     sp = 128;
	void function() {
		sp -= 27;
		final int _vO14 = 1;
	_jmp5:
		Rsx.setcpy(privateStorage, _vO14, privateStorage, _vO28, 10);//STORE
		final int _vO156 = register0.offset + 0;
		final int _vO157 = sp + 0;
		stackStorage[_vO157] = register0.memory[_vO156];
		final int _vO158 = register0.offset + 1;
		final int _vO159 = sp + 1;
		stackStorage[_vO159] = register0.memory[_vO158];
		final int _vO162 = sp + 2;
		Rsx.divF6sint32(stackStorage, _vO162, stackStorage, _vO159, privateStorage, _vO276);
		final int _vO163 = sp + 3;
		Rsx.addF6sint32(stackStorage, _vO163, stackStorage, _vO157, stackStorage, _vO162);
		final int _vO164 = register0.offset + 2;
		final int _vO165 = sp + 4;
		stackStorage[_vO165] = register0.memory[_vO164];
		final int _vO166 = register0.offset + 4;
		final int _vO167 = sp + 5;
		stackStorage[_vO167] = register0.memory[_vO166];
		final int _vO168 = sp + 6;
		Rsx.bitwiseAndF6sint32(stackStorage, _vO168, stackStorage, _vO165, stackStorage, _vO167);
		final int _vO169 = register0.offset + 3;
		final int _vO170 = sp + 7;
		stackStorage[_vO170] = register0.memory[_vO169];
		final int _vO171 = sp + 8;
		Rsx.bitwiseXorF6sint32(stackStorage, _vO171, stackStorage, _vO168, stackStorage, _vO170);
		final int _vO172 = sp + 9;
		Rsx.bitwiseAndF6sint32(stackStorage, _vO172, stackStorage, _vO163, stackStorage, _vO171);
		final int _vO174 = sp + 10;
		Rsx.addF6sint32(stackStorage, _vO174, stackStorage, _vO172, privateStorage, _vO36);
		final int _vO176 = register1.offset + 0 + Float.floatToIntBits(stackStorage[_vO172]) * 4;
		final int _vO178 = _vO176 + 0;
		register1.memory[_vO178] = privateStorage[_vO35];//STORE
		final int _vO180 = _vO176 + 1;
		final int _vO182 = _vO180 + 0;
		register1.memory[_vO182] = privateStorage[_vO36];//STORE
		final int _vO184 = _vO180 + 1;
		register1.memory[_vO184] = privateStorage[_vO37];//STORE
		final int _vO186 = _vO180 + 2;
		register1.memory[_vO186] = privateStorage[_vO38];//STORE
		final int _vO188 = sp + 11;
		Rsx.addF6sint32(stackStorage, _vO188, stackStorage, _vO172, privateStorage, _vO37);
		final int _vO191 = sp + 12;
		Rsx.addF6sint32(stackStorage, _vO191, stackStorage, _vO172, privateStorage, _vO37);
		final int _vO193 = register1.offset + 0 + Float.floatToIntBits(stackStorage[_vO174]) * 4;
		final int _vO195 = _vO193 + 0;
		register1.memory[_vO195] = stackStorage[_vO188];//STORE
		final int _vO197 = _vO193 + 1;
		final int _vO199 = _vO197 + 0;
		register1.memory[_vO199] = privateStorage[_vO36];//STORE
		final int _vO201 = _vO197 + 1;
		register1.memory[_vO201] = privateStorage[_vO37];//STORE
		final int _vO203 = _vO197 + 2;
		register1.memory[_vO203] = privateStorage[_vO38];//STORE
		final int _vO205 = sp + 13;
		Rsx.addF6sint32(stackStorage, _vO205, stackStorage, _vO172, privateStorage, _vO38);
		final int _vO210 = register1.offset + 0 + Float.floatToIntBits(stackStorage[_vO191]) * 4;
		final int _vO212 = _vO210 + 0;
		register1.memory[_vO212] = stackStorage[_vO205];//STORE
		final int _vO214 = _vO210 + 1;
		final int _vO216 = _vO214 + 0;
		register1.memory[_vO216] = privateStorage[_vO36];//STORE
		final int _vO218 = _vO214 + 1;
		register1.memory[_vO218] = privateStorage[_vO37];//STORE
		final int _vO220 = _vO214 + 2;
		register1.memory[_vO220] = privateStorage[_vO38];//STORE
		final int _vO221 = register1.offset + 0 + 4 + 0;
		final int _vO222 = sp + 14;
		stackStorage[_vO222] = register1.memory[_vO221];
		final int _vO223 = register1.offset + 0 + Float.floatToIntBits(stackStorage[_vO222]) * 4 + 0;
		register1.memory[_vO223] = privateStorage[_vO123];//STORE
		final int _vO224 = _vO14 + 0 + 0;
		final int _vO225 = sp + 15;
		stackStorage[_vO225] = privateStorage[_vO224];
		final int _vO226 = _vO14 + 2 + 1;
		final int _vO227 = sp + 16;
		stackStorage[_vO227] = privateStorage[_vO226];
		final int _vO228 = sp + 17;
		Rsx.addF5float(stackStorage, _vO228, stackStorage, _vO225, stackStorage, _vO227);
		final int _vO229 = register0.offset + 0;
		final int _vO230 = sp + 18;
		stackStorage[_vO230] = register0.memory[_vO229];
		final int _vO231 = sp + 19;
		Rsx.signedToFloatF5float(stackStorage, _vO231, stackStorage, _vO230);
		final int _vO232 = sp + 20;
		Rsx.addF5float(stackStorage, _vO232, stackStorage, _vO228, stackStorage, _vO231);
		final int _vO233 = register2.offset + 0 + 0;
		register2.memory[_vO233] = stackStorage[_vO232];//STORE
		final int _vO234 = _vO14 + 0 + 1;
		final int _vO235 = sp + 21;
		stackStorage[_vO235] = privateStorage[_vO234];
		final int _vO236 = _vO14 + 2 + 0;
		final int _vO237 = sp + 22;
		stackStorage[_vO237] = privateStorage[_vO236];
		final int _vO238 = sp + 23;
		Rsx.addF5float(stackStorage, _vO238, stackStorage, _vO235, stackStorage, _vO237);
		final int _vO239 = register0.offset + 1;
		final int _vO240 = sp + 24;
		stackStorage[_vO240] = register0.memory[_vO239];
		final int _vO241 = sp + 25;
		Rsx.signedToFloatF5float(stackStorage, _vO241, stackStorage, _vO240);
		final int _vO242 = sp + 26;
		Rsx.addF5float(stackStorage, _vO242, stackStorage, _vO238, stackStorage, _vO241);
		final int _vO243 = register2.offset + 0 + 1;
		register2.memory[_vO243] = stackStorage[_vO242];//STORE
	_jmpEnd:
		sp += 27;
	}
};


void CallTest() 
{
	Sample sample;
	sample.register0 = { Rsx.allocate(35), 0 };
	sample.register1 = { Rsx.allocate(4 * 5), 0 };
	sample.register2 = { Rsx.allocate(16), 0 };



	sample.register0.memory[0] = Float.intBitsToFloat(1);
	sample.initialize();
	sample.function();

	std::initializer_list<float> rg0(sample.register0.memory, sample.register0.memory + 35);
	std::initializer_list<float> rg1(sample.register1.memory, sample.register1.memory + 35);
	std::initializer_list<float> rg2(sample.register2.memory, sample.register2.memory + 35);

	std::initializer_list<int> rg0u((int*)sample.register0.memory, (int*)sample.register0.memory + 35);
	std::initializer_list<int> rg1u((int*)sample.register1.memory, (int*)sample.register1.memory + 35);
	std::initializer_list<int> rg2u((int*)sample.register2.memory, (int*)sample.register2.memory + 35);

}