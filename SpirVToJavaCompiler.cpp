#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <sstream>
#include <map>
#include <cassert>
#include <set>
#include <algorithm>
#include "./SpirVType.h"

template<typename T>
T tread(const uint8_t*& addr) 
{
	T v = *((T*)addr);
	addr += sizeof(T);
	return v;
}

template<typename T>
T treadX(const uint8_t*& addr, uint16_t& readed)
{
	T v;
	uint32_t* arr = reinterpret_cast<uint32_t*>(&v);
	for (size_t i = 0; i < (sizeof(T) / 4); i++) {
		arr[i] = reinterpret_cast<const uint32_t*>(addr)[i];
		addr += sizeof(uint32_t);
		readed++;
	}
	return v;
}

uint32_t readword(const uint8_t*& addr, uint16_t& readedWords)
{
	readedWords++;
	uint32_t v = *((uint32_t*)addr);
	addr += sizeof(uint32_t);
	return v;
}

std::string sread(const uint8_t*& addr, uint16_t& rededWords)
{
	std::string result;
	while (true) {
		rededWords++;
		uint32_t s = tread<uint32_t>(addr);
		const uint8_t* octet = (const uint8_t*)&s;
		for (int i = 0; i < 4; i++) {
			if (*octet)
				result.push_back(*octet++);
			else 
				return result;
		}
	}
	return result;
}


class object_base
{
	const char* m_type;
public:
	object_base(const object_base&) = delete;
	object_base& operator=(const object_base&) = delete;
	virtual ~object_base() = default;
	object_base(const char* type) :
		m_type(type)
	{}

	const char* getType() const 
	{
		return m_type;
	}

	template<typename T>
	bool is() const
	{
		if (strcmp(m_type, typeid(T).name()))
			return false;
		return true;
	}

	template<typename T>
	inline const T& q() const;
};

template<typename T>
class object_t : public object_base
{
public:
	T m_value;

	template<typename... X>
	object_t(X&& ... args) :
		object_base(typeid(T).name()),
		m_value{ std::forward<X>(args)... }
	{}
};

template<typename T>
inline const T& object_base::q() const
{
	if (strcmp(m_type, typeid(T).name()))
		throw std::runtime_error("invalid type query");
	return static_cast<const object_t<T>*>(this)->m_value;
}


typedef std::shared_ptr<object_base> object;
template<typename T, typename... X>
object createObject(X&&... args) 
{
	return std::make_shared<object_t<T>>(std::forward<X>(args)...);
}

template<typename T>
float getWord(T o, uint32_t index) 
{
	float* addr = reinterpret_cast<float*>(&o);
	return addr[index];
}


struct ptr
{
	uint32_t page;
	int32_t  offset;
};

void constantStore(std::shared_ptr<const object_base> vs, std::vector<float>& result) {

	if (vs->is<float>())
		result.push_back(getWord(vs->q<float>(), 0));
	else if (vs->is<int32_t>())
		result.push_back(getWord(vs->q<int32_t>(), 0));
	else if (vs->is<int64_t>()) {
		result.push_back(getWord(vs->q<int64_t>(), 0));
		result.push_back(getWord(vs->q<int64_t>(), 1));
	}
	else if (vs->is<double>())
	{
		result.push_back(getWord(vs->q<double>(), 0));
		result.push_back(getWord(vs->q<double>(), 1));
	}
	else if (vs->is<bool>())
		result.push_back(getWord(vs->q<bool>() ? uint32_t(1) : uint32_t(0), 0));
	else if (vs->is<ptr>())
	{
		result.push_back(getWord(vs->q<ptr>(), 0));
		result.push_back(getWord(vs->q<ptr>(), 1));
	}		
	else if (vs->is<std::vector<object>>())
	{
		std::vector<object> v = vs->q<std::vector<object>>();
		for (auto& x : v) {
			constantStore(x, result);
		}
	}
	else
		throw std::runtime_error("invalid constant object");
}


#define BUILD_TYPE


//decorations
std::map<uint32_t, uint32_t> buildIn;
std::map<uint32_t, uint32_t> bindings;

//types and objects
std::map<uint32_t, type_t>	 allTypes;
std::map<uint32_t, object>	 allConstantsX;


struct variable {
	int32_t		constantId;
	uint32_t	type;
	std::string memory;
	std::string offset;
};
std::map<uint32_t, variable> allVariables;

//resources
std::set<std::string> resources;


uint32_t opInc = 0;
std::set<std::string> strings;

uint32_t unmapPointer(uint32_t pointerTypeId)
{

	auto x = std::dynamic_pointer_cast<ptr_type>(allTypes[pointerTypeId]);
	assert(x);
	std::string s = x->innerType->marshal();
	for (auto& b : allTypes) {
		if (b.second->marshal() == s)
			return b.first;
	}
	throw std::runtime_error("invalid type moran");
}

std::string loadInt(uint32_t varIndex, std::stringstream& o) 
{
	std::string opIs = "_tmp" + std::to_string(opInc++);
	const auto& var = allVariables[varIndex];
	if (var.constantId != -1) {
		return std::to_string(allConstantsX[var.constantId]->q<int32_t>());
	} else {
		return "Float.floatToIntBits("+var.memory+"["+var.offset+"])";
	}
}

#define createEnumer(x, ...) \
const char* x (uint32_t index) \
{\
	static std::map<uint32_t, const char*> s = {\
		__VA_ARGS__\
	};\
	auto iter = s.find(index);\
	if (iter == s.end()) {\
		std::string s = # x "(" + std::to_string(index) + ")";\
		auto str = strings.find(s);\
		if (str == strings.end())\
			strings.emplace(s);\
		str = strings.find(s);\
		return (*str).c_str();\
	}\
	return iter->second;\
}

createEnumer(Decoration, 
	{35, "Offset"}, 
	{ 2, "Block" }, 
	{ 34, "DescriptorSet" },
	{ 33, "Binding" },
	{ 11, "BuiltIn"},
	{ 3, "BufferBlock" },
	{ 6, "ArrayStride" }
);
createEnumer(StorageClass,
	{ 0, "UniformConstant" },
	{ 1, "Input" },
	{ 2, "Uniform" },
	{ 3, "Output" },
	{ 4, "Workgroup" },
	{ 5, "CrossWorkgroup" },
	{ 6, "Private" },
	{ 7, "Function" },
	{ 8, "Generic" },
	{ 9, "PushConstant" },
	{ 10, "AtomicCounter" },
	{ 11, "Image" },
	{ 12, "StorageBuffer" }
);
createEnumer(FunctionControl,
	{ 0, "None" },
	{ 1, "Inline" },
	{ 2, "DontInline" },
	{ 4, "Pure" },
	{ 8, "Const" },
);

template<typename T>
const char* flags(T f, uint32_t flags) 
{
	std::string result;
	uint32_t bit = 1;
	for (uint32_t i = 0; i < 32; i++) {
		if (flags & bit) {
			result += std::string(f(bit)) + std::string(" |");
		}
		bit = bit << 1;
	}
	if (result.size())
		result.pop_back();
	auto iter = strings.find(result);
	if (iter == strings.end())
		strings.emplace(result);
	return (*strings.find(result)).c_str();
}

void process(const std::vector<uint8_t>& binary, std::stringstream& output, std::stringstream& header, std::stringstream& ctor)
{
	const uint8_t* p = binary.data();

	uint32_t magic = tread<uint32_t>(p);
	if (magic != 0x07230203)
		throw std::runtime_error("invalid spirv binary");
	uint32_t ver = tread<uint32_t>(p);
	uint32_t gen = tread<uint32_t>(p);
	uint32_t bou = tread<uint32_t>(p);
	uint32_t shem = tread<uint32_t>(p);
	uint32_t functionAlloc = 0;
	uint32_t privateAlloc = 0;
	ctor << "void initialize() {\n";
	auto resolveConstant = [&](uint32_t constId, uint32_t type)
	{
		auto c = allConstantsX[constId];
		output << "int _vO" + std::to_string(constId) + " = " + std::to_string(privateAlloc) + ";\n";
		allVariables[constId] = { int32_t(constId), type , "privateStorage",  "_vO" + std::to_string(constId) };
		privateAlloc += allTypes[type]->length();

		std::vector<float> m;
		constantStore(c, m);
		std::string data;
		for (int i = 0; i < m.size(); i++) {
			char constBfk[100];
			float se = m[i];
			snprintf(constBfk, 100, "Float.intBitsToFloat(0x%x)", *((uint32_t*)&se));
			data += constBfk;
			if (i != m.size() - 1)
				data += ",";
		}

		ctor << "Rsx.setlf("+ allVariables[constId] .memory+", " + allVariables[constId].offset +", " + data + ");\n";
	
	};
	auto genericOp1Arg = [&](uint32_t resultType, uint32_t resultId, uint32_t op1, const std::string& op)
	{
		auto t = allTypes[resultType];
		auto& allocType = t;
		output << "sp -= " + std::to_string(allocType->length()) + ";\n";
		output << "int     _vO" + std::to_string(resultId) + " = sp;\n";
		allVariables[resultId] = { -1, resultType, "stackStorage", "_vO" + std::to_string(resultId) };
		output << "Rsx." + op + t->marshal() + "("+ allVariables[resultId].memory+","+ allVariables[resultId].offset+", "+ allVariables[op1].memory+", "+ allVariables[op1].offset+");\n";

		functionAlloc += allocType->length();
	};
	auto genericOp2Arg = [&](uint32_t resultType, uint32_t resultId, uint32_t op1, uint32_t op2, const std::string& op)
	{
		auto t = allTypes[resultType];
		auto& allocType = t;
		output << "sp -= " + std::to_string(allocType->length()) + ";\n";
		output << "int     _vO" + std::to_string(resultId) + " = sp;\n";
		allVariables[resultId] = { -1, resultType, "stackStorage", "_vO" + std::to_string(resultId) };

		output << "Rsx." + op  + t->marshal() + 
			"("+ allVariables[resultId].memory +", "+ allVariables[resultId].offset + 
			","+ allVariables[op1].memory +", "+ allVariables[op1].offset +","
			+ allVariables[op2].memory +", "+ allVariables[op2].offset +");\n";
		functionAlloc += allocType->length();
	};

	while (p < (binary.data() + binary.size())) {
		uint16_t instr = tread<uint16_t>(p);
		uint16_t wordCount = tread<uint16_t>(p);
		uint16_t rededWords = 1;
		auto rw = [&]() -> uint32_t {
			return readword(p, rededWords);
		};
		auto rf = [&]() -> float {
			uint32_t b = readword(p, rededWords);
			return *((float*)(&b));
		};

		printf("%d-%d)\t", wordCount, instr);
		if (instr == 17) {
			uint32_t capability = rw();
			printf("\tOpCapability %s\n", capability == 1 ? "Shader" : "(undefined)");
			if (capability != 1)
				throw std::runtime_error("invalid spv capability");
		}
		else if (instr == 11) {
			uint32_t resultId = rw();
			auto s = sread(p, rededWords);
			printf("$%d =\tOpExtInstImport %s\n", resultId, s.c_str());
		}
		else if (instr == 14) {
			uint32_t a = rw();
			uint32_t b = rw();
			printf("\tOpMemoryModel %d %d\n", a, b);
		}
		else if (instr == 15) {
			uint32_t exM = rw();
			uint32_t entrypId = rw();
			auto     name = sread(p, rededWords);
			printf("\tOpEntryPoint %d $%d %s", exM, entrypId, name.c_str());
			for (int i = rededWords; i < wordCount; i++) {
				uint32_t s = rw();
				printf(" $%d", s);
			}
			printf("\n");
		}
		else if (instr == 16) {
			uint32_t entryId = rw();
			uint32_t excM = rw();

			printf("\tOpExecutionMode $%d %d", entryId, excM);
			for (int i = rededWords; i < wordCount; i++) {
				printf(" %d", rw());
			}
			printf("\n");
		}
		else if (instr == 3) {
			uint32_t sourceLang = rw();
			uint32_t version = rw();

			printf("\tOpSource %d %d", sourceLang, version);
			
			if (wordCount != rededWords) {
				printf(" $%d", rw());
			}
			if (wordCount != rededWords) {
				printf(" %s", sread(p, rededWords).c_str());
			}
			printf("\n");
		}
		else if (instr == 5) {
			uint32_t id = rw();
			auto name = sread(p, rededWords);

			printf("\tOpName $%d %s", id, name.c_str());

			printf("\n");
		}
		else if (instr == 6) {
			uint32_t typeId = rw();
			uint32_t literalNumber = rw();
			auto name = sread(p, rededWords);

			printf("\tOpMemberName $%d %d %s", typeId, literalNumber, name.c_str());

			printf("\n");
		}
		else if (instr == 72) {
			uint32_t typeId = rw();
			uint32_t literalNumber = rw();
			uint32_t decortation = rw();

			printf("\tOpMemberDecorate $%d %d %s", typeId, literalNumber, Decoration(decortation));
			for (int i = rededWords; i < wordCount; i++) {
				printf(" %d", rw());
			}
			printf("\n");
		}
		else if (instr == 71) {
			uint32_t target = rw();
			uint32_t decortaion = rw();
	
			printf("\tOpDecorate $%d %s", target, Decoration(decortaion)); 
			if (decortaion == 11) {
				uint32_t buildInS = rw();
				buildIn[target] = buildInS;
			}
			else
			if (decortaion == 33) {
				uint32_t binding = rw();
				bindings[target] = binding;
			} else
			for (int i = rededWords; i < wordCount; i++) {
				printf(" %d", rw());
			}
			printf("\n");
		}
		else if (instr == 19) {
			uint32_t resultId = rw();
			BUILD_TYPE allTypes[resultId] = createType<void_type>();
			printf("$%d =\tOpTypeVoid", resultId);

			printf("\n");
		}
		else if (instr == 20) {
			uint32_t resultId = rw();
			BUILD_TYPE allTypes[resultId] = createType<bool_type>();
			printf("$%d =\tOpTypeBool", resultId);

			printf("\n");
		}
		else if (instr == 21) {
			uint32_t resultId = rw();
			uint32_t width = rw();
			uint32_t signess = rw();

			if (width == 32) {
				allTypes[resultId] = createType<int_type<true, 32>>();
			}
			else if (width == 16) {
				allTypes[resultId] = createType<int_type<true, 16>>();
			}
			else if (width == 64) {
				allTypes[resultId] = createType<int_type<true, 64>>();
			} else 
				std::runtime_error("invalid int width");
			
			printf("$%d =\tOpTypeInt %d %d", resultId, width, signess);

			printf("\n");
		}
		else if (instr == 22) {
			uint32_t resultId = rw();
			uint32_t width = rw();
			if (width == 32) {
				allTypes[resultId] = createType<float_type<32>>();
			} else {
				throw std::runtime_error("invalid type of sclaar");
			}	
			printf("$%d =\tOpTypeFloat %d", resultId, width);
			printf("\n");
		}
		else if (instr == 23) {
			uint32_t resultId = rw();
			uint32_t cmpType = rw();
			uint32_t cmpCount = rw();
	
			allTypes[resultId] = createType<vector_type>(allTypes[cmpType], cmpCount);

			printf("$%d =\tOpTypeVector $%d %d", resultId, cmpType, cmpCount);

			printf("\n");
		}
		else if (instr == 24) {
			uint32_t resultId = rw();
			uint32_t columnType = rw();
			uint32_t columnCount = rw();

			allTypes[resultId] = createType<matrix_type>(allTypes[columnType], columnCount);

			printf("$%d =\tOpTypeMatrix $%d %d", resultId, columnType, columnCount);

			printf("\n");
		}
		else if (instr == 32) {
			uint32_t resultId = rw();
			uint32_t storageClass = rw();
			uint32_t type = rw();
			auto p = allTypes[type];
			if (p == nullptr)
				throw std::runtime_error("invalid type for create pointer type");
			allTypes[resultId] = createType<ptr_type>(p);
		
			printf("$%d =\tOpTypePointer %s $%d", resultId, StorageClass(storageClass), type);

			printf("\n");
		}
	
		else if (instr == 33) {
			uint32_t resultId = rw();
			uint32_t returnType = rw();

			printf("$%d =\tOpTypeFunction $%d", resultId, returnType);
			for (int i = rededWords; i < wordCount; i++) {
				printf(" $%d", rw());
			}
			printf("\n");
		}

		else if (instr == 30) {
			uint32_t resultId = rw();
			printf("$%d =\tOpTypeStruct", resultId);

			std::vector<type_t> buildType;

			for (int i = rededWords; i < wordCount; i++) {
				auto x = rw();
				auto t = allTypes[x];
				if (t == nullptr)
					throw std::runtime_error("type not found");
				buildType.push_back(t);
				printf(" $%d", x);
			}
			allTypes[resultId] = createType<struct_type>(buildType);
			printf("\n");
		}
		else if (instr == 28) {
			uint32_t resultId = rw();
			uint32_t elementType = rw();
			uint32_t lengthConstId = rw();
		
			allTypes[resultId] = createType<array_type>(allTypes[elementType], allConstantsX[lengthConstId]->q<int32_t>());

			printf("$%d =\tOpTypeArray $%d $%d", resultId, elementType, lengthConstId);
			printf("\n");
		}
		else if (instr == 29) {
			uint32_t resultId = rw();
			uint32_t elementType = rw();

			allTypes[resultId] = createType<dynamic_array_type>(allTypes[elementType]);

			printf("$%d =\tOpTypeRuntimeArray $%d", resultId, elementType);
			printf("\n");
		}
		else if (instr == 44) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();		
			printf("$%d =\tOpConstantComposite $%d", resultId, resultType);
			std::vector<object> objects;
			for (int i = rededWords; i < wordCount; i++) {
				auto x = rw();
				objects.push_back(allConstantsX[x]);
				printf(" $%d",  x);
			}
			allConstantsX[resultId] = createObject<std::vector<object>>(objects);
			printf("\n");
			resolveConstant(resultId, resultType);
		}
		else if (instr == 43) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			printf("$%d =\tOpConstant $%d", resultId, resultType);
		
			auto type = allTypes[resultType];

			if (std::dynamic_pointer_cast<ptr_type>(type)) {
				auto x = treadX<ptr>(p, rededWords);
				allConstantsX[resultId] = createObject<ptr>(x);
				printf(" %d", x);
			}
			else
			if (type->is("F6double")) {
				auto x = treadX<double>(p, rededWords);
				allConstantsX[resultId] = createObject<double>(x);
				printf(" %f", x);
			}
			else if (type->is("F5float")) {
				auto x = treadX<float>(p, rededWords);
				allConstantsX[resultId] = createObject<float>(x);
				printf(" %f", x);
			}
			else if (type->is("F6sint32")) {
				auto x = treadX<int32_t>(p, rededWords);
				allConstantsX[resultId] = createObject<int32_t>(x);
				printf(" %d", x);
			}
			else if (type->is("F6uint32")) {
				auto x = treadX<uint32_t>(p, rededWords);
				allConstantsX[resultId] = createObject<int32_t>(int32_t(x));
				printf(" %u", x);
			}
			else
				throw std::runtime_error("invalid constant type");			
			printf("\n");

			resolveConstant(resultId, resultType);
		} else if (instr == 59) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t storageClass = rw();

			printf("$%d =\tOpVariable $%d %s", resultId, resultType, StorageClass(storageClass));
			for (int i = rededWords; i < wordCount; i++) {
				printf(" $%d", rw());
			}
			if (storageClass == 7) {
				auto& allocType = allTypes[unmapPointer(resultType)];
				output << "sp -= " + std::to_string(allocType->length()) + ";\n";
				output << "int     _vO" + std::to_string(resultId) + " = sp;\n";
				allVariables[resultId] = { -1, resultType, "stackStorage", "_vO" + std::to_string(resultId) };
				functionAlloc += allocType->length();			
			} 
			else if (storageClass == 6) {
				auto& allocType = allTypes[unmapPointer(resultType)];
				output << "int     _vO" + std::to_string(resultId) + " = " + std::to_string(privateAlloc) + ";\n";
				allVariables[resultId] = { -1, resultType, "privateStorage", "_vO" + std::to_string(resultId) };
				privateAlloc += allocType->length();
			}
			else if (storageClass == 2) {
				uint32_t regisrerId = bindings[resultId];
				std::string registerName = "register" + std::to_string(regisrerId);
				resources.insert(registerName);
				auto& allocType = allTypes[unmapPointer(resultType)];
				allVariables[resultId] = { -1, resultType, registerName + ".memory", registerName + ".offset" };
			}
			else if (storageClass == 1) {
				uint32_t regisrerId = buildIn[resultId];
				std::string registerName = "input" + std::to_string(regisrerId);
				resources.insert(registerName);
				auto& allocType = allTypes[unmapPointer(resultType)];
				allVariables[resultId] = { -1, resultType, registerName + ".memory", registerName + ".offset" };

			}
			printf("\n");
		}
		else if (instr == 56) {
			output << "_jmpEnd:\n\tsp += " + std::to_string(functionAlloc) + ";\n";
			output << "}\n";
			printf("\tOpFunctionEnd");
			printf("\n");
		}
		else if (instr == 248) {
			uint32_t resultId = rw();
			printf("$%d =\tOpLabel", resultId);
			output << "_jmp" << std::to_string(resultId) << ":\n";
			printf("\n");
		}
		else if (instr == 54) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t functionControl = rw();
			uint32_t functionType = rw();
			functionAlloc = 0;
			printf("$%d =\tOpFunction $%d (%s) $%d", resultId, resultType, flags(FunctionControl, functionControl), functionType);
			printf("\n");

			output << "void function() {\n";
		}


		else if (instr == 62) {
			uint32_t pointer = rw();
			uint32_t object = rw();
		
			printf("\tOpStore $%d $%d", pointer, object);

			const auto& pointerR = allVariables[pointer];
			const auto& objectR = allVariables[object];
			
			std::string mem;
			std::string offset;
			if (pointerR.constantId != -1) {
				auto c = allConstantsX[pointerR.constantId];
				if (!c->is<ptr>()) {
					throw std::runtime_error("invalid ptr const");
				}
				ptr g = c->q<ptr>();
				mem = "Rsx.getPage(" + std::to_string(g.page) + ")";
				offset = std::to_string(g.offset);
			}
			else {
				mem = pointerR.memory;
				offset = pointerR.offset;
			}

			uint32_t copyLength = allTypes[objectR.type]->length();
			if (copyLength == 1) 
				output << mem + "[" + offset + "] = " + objectR.memory + "[" + objectR.offset + "];//STORE\n";
			else 
				output << "Rsx.setcpy(" + mem + "," + offset + "," + objectR.memory + "," + objectR.offset +"," + std::to_string(copyLength) + ");//STORE\n";
		
			for (int i = rededWords; i < wordCount; i++) {
				printf(" %d", rw());
			}
			printf("\n");
		}
		else if (instr == 61) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t pointer = rw();
			printf("$%d =\tOpLoad $%d $%d", resultId, resultType, pointer);

			auto allocType = allTypes[resultType];
			output << "sp -= " + std::to_string(allocType->length()) + ";\n";
			output << "int     _vO" + std::to_string(resultId) + " = sp;\n";

			allVariables[resultId] = { -1, resultType, "stackStorage",  "_vO" + std::to_string(resultId) };

			functionAlloc += allocType->length();

			output << "Rsx.setcpy("+ allVariables[resultId].memory + ", "+ allVariables[resultId].offset + "," + allVariables[pointer].memory + "," + allVariables[pointer].offset +", " + std::to_string(allTypes[resultType]->length()) + ");//LOAD\n";
			
			printf("\n");
		}
		else if (instr == 81) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t compisite = rw();

			const auto& var = allVariables[compisite];

			printf("$%d =\tOpCompositeExtract $%d $%d", resultId, resultType, compisite);
			//std::string opIs = "_tmp" + std::to_string(opInc++);
			bool isFirst = true;
			uint32_t memOffset = 0;
			auto baseType = std::static_pointer_cast<const type_base>(allTypes[var.type]);
			for (int i = rededWords; i < wordCount; i++) {
				auto gafs = rw();	
				baseType = baseType->accessTo(gafs, memOffset);
			}

			if (allTypes[resultType]->marshal() != baseType->marshal())
				throw std::runtime_error("invalid access type");

			allVariables[resultId] = { -1, resultType, var.memory, "_vO" + std::to_string(resultId) };
			output << "int     _vO" + std::to_string(resultId) + " =_vO" + std::to_string(compisite) + " + " + std::to_string(memOffset) + ";\n";
			printf("\n");
		}
		else if (instr == 80) {		
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			
			auto t = allTypes[resultType];;
			output << "sp -= " + std::to_string(t->length()) + ";\n";
			output << "int     _vO" + std::to_string(resultId) + " = sp;\n";
			functionAlloc += t->length();
			allVariables[resultId] = { -1, resultType, "stackStorage","_vO" + std::to_string(resultId) };

			printf("$%d =\tOpCompositeConstruct $%d ", resultId, resultType);
			uint32_t copyOff = 0;
			for (int i = rededWords; i < wordCount; i++) {
				auto var = rw();
				printf(" $%d ", var);
				auto varType = allTypes[allVariables[var].type];
				output << "Rsx.setcpy(" + allVariables[resultId].memory + "," + allVariables[resultId].offset + " + " + std::to_string(copyOff) + "," + allVariables[var].memory + "," + allVariables[var].offset + ", " + std::to_string(varType->length()) + ");\n";
				copyOff += varType->length();
			}

			printf("\n");
		}
		else if (instr == 65) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t base = rw();

			const auto& baseVariable = allVariables[base];

			printf("$%d =\tOpAccessChain $%d $%d", resultId, resultType, base);
			std::vector<std::string> offsets;
			auto baseType = std::static_pointer_cast<const type_base>(allTypes[unmapPointer(baseVariable.type)]);
			for (int i = rededWords; i < wordCount; i++) {
				auto gafs = rw();
				printf(" $%d", gafs);
				auto var = allVariables[gafs];
				if (var.constantId != -1) {
					uint32_t memOffset = 0;
					baseType = baseType->accessTo(allConstantsX[gafs]->q<int32_t>(), memOffset);
					offsets.push_back(std::to_string(memOffset));				
				} else {
					std::string id = loadInt(gafs, output); 
					offsets.push_back(id + "*" + std::to_string(baseType->signleType()->length()));
					uint32_t memOffset = 0;
					baseType = baseType->accessTo(0, memOffset);
				}
			}
			//offsets.erase(std::remove(offsets.begin(), offsets.end(), "0"), offsets.end());
			std::string resultSums;
			for (size_t i = 0; i < offsets.size(); i++)
			{			
				resultSums += offsets[i];				
				resultSums += "+";	
			}
			if (resultSums.size())
				resultSums.pop_back();
			if (allTypes[unmapPointer(resultType)]->marshal() != baseType->marshal())
				throw std::runtime_error("invalid access type");

			allVariables[resultId] = {-1, resultType, baseVariable.memory, "_vO" + std::to_string(resultId) };

			output << "int     _vO" + std::to_string(resultId) + " = "+ baseVariable.offset + "+" + resultSums + ";\n";

			printf("\n");
		}	
		else if (instr == 128) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpIAdd $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "add");
			printf("\n");
		}
		else if (instr == 129) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpFAdd $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "add");
			printf("\n");
		}
		else if (instr == 131) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpFSub $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "sub");
			printf("\n");
		}
		else if (instr == 130) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpISub $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "sub");
			printf("\n");
		}
		else if (instr == 132) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpIMul $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "mul");
			printf("\n");
		}
		else if (instr == 133) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpFMul $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "mul");
			printf("\n");
		}
		else if (instr == 124) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			printf("$%d =\tOpBitcast $%d $%d", resultId, resultType, op1);
			genericOp1Arg(resultType, resultId, op1, "bitcast");
			printf("\n");
		}
		else if (instr == 145) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpMatrixTimesVector $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "mXv" + allTypes[allVariables[op1].type]->marshal() + "X" + allTypes[allVariables[op2].type]->marshal());
			printf("\n");
		}
		else if (instr == 146) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpMatrixTimesMatrix $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "mXm" + allTypes[allVariables[op1].type]->marshal() + "X" + allTypes[allVariables[op2].type]->marshal());
			printf("\n");
		}

		else if (instr == 142) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpVectorTimesScalar $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "vXs" + allTypes[allVariables[op1].type]->marshal() + "X" + allTypes[allVariables[op2].type]->marshal());
			printf("\n");
		}

		else if (instr == 144) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpVectorTimesMatrix $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "vXm" + allTypes[allVariables[op1].type]->marshal() + "X" + allTypes[allVariables[op2].type]->marshal());
			printf("\n");
		}

		else if (instr == 143) {
			uint32_t resultType = rw();
			uint32_t resultId = rw();
			uint32_t op1 = rw();
			uint32_t op2 = rw();
			printf("$%d =\tOpMatrixTimesScalar $%d $%d $%d", resultId, resultType, op1, op2);
			genericOp2Arg(resultType, resultId, op1, op2, "mXs" + allTypes[allVariables[op1].type]->marshal() + "X" + allTypes[allVariables[op2].type]->marshal());
			printf("\n");
		}

		else if (instr == 249) {
			uint32_t target = rw();
			printf("\tOpBranch $%d", target);

			output << "goto _jmp" + std::to_string(target) + ";\n";
			printf("\n");
		}






		if (wordCount != rededWords || wordCount < rededWords) {
			uint16_t dif = wordCount - rededWords;
			p += sizeof(uint32_t)*dif;
			printf("\tERROR: influcient words count: %d\n", dif);
		}


		printf("");
	}


	for (auto& x : resources) {
		if (x != "privateStorage")
			header << "public  Buffer  " + x + " = null;\n";
	}
	header << "private float[] privateStorage = Rsx.allocate(" + std::to_string(privateAlloc) +");\n";
	header << "private float[] stackStorage   = Rsx.allocate(128);\n";
	header << "private int     sp             = 128;\n";
	ctor << "}\n";
}

void print(int c) 
{
	for (int i = 0; i < c; i++)
		putchar(' ');
}

void print_all_object(object x) 
{
	if (x->is<ptr>()) {
		printf("%X-%x", x->q<ptr>().page, x->q<ptr>().offset);
	}
	else
	if (x->is<double>()) {
		printf("%f", x->q<double>());
	}
	else
	if (x->is<float>()) {
		printf("%f", x->q<float>());
	}
	else
	if (x->is<int32_t>()) {
		printf("%d", x->q<int32_t>());
	}
	else
	if (x->is<bool>()) {
		printf("%s", x->q<bool>() ? "true" : "false");
	}
	else
	if (x->is<std::vector<object>>()) {
		printf("{");
		const std::vector<object>& a = x->q<std::vector<object>>();
		for (size_t i = 0; i < a.size(); i++) {
			print_all_object(a[i]);
			if (i != a.size() - 1)
				printf(",");
		}
		printf("}");
	}
}


int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		std::stringstream s; std::stringstream eg; std::stringstream ctor;
		std::ifstream f(argv[i], std::ios::binary | std::ios::ate);
		size_t length = f.tellg();
		f.seekg(std::ios::beg, 0);
		std::vector<uint8_t> l(length, 0);
		f.read((char*)l.data(), length);
		process(l, s, eg, ctor);
		printf("==============\n");
		printf("%s", ctor.str().c_str());
		printf("%s", eg.str().c_str());
		printf("%s", s.str().c_str());
		printf("==============\n");
	}

	for (auto& x : allTypes) {
		printf("%d: \t%s\n", x.first, x.second->marshal().c_str());
		printf("%d: \t%s\n", x.first, x.second->marshal2().c_str());
	}


	for (auto& x : allConstantsX) {
		printf("%d = ", x.first);
		print_all_object(x.second);
		printf("\n");
	}
	
    std::cout << "all good!\n";
}