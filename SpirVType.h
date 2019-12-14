#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

struct type_base
{
	type_base(const type_base&) = delete;
	type_base& operator=(const type_base&) = delete;
	type_base() = default;

	virtual ~type_base() = default;
	virtual size_t length() const = 0;
	virtual std::string marshal() const = 0;
	virtual std::string marshal2() const = 0;

	bool is(const std::string& marsh) const 
	{
		return marshal() == marsh;
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset) const 
	{
		throw std::runtime_error("type can not accessable");
	}

	virtual std::shared_ptr<const type_base> signleType() const
	{
		throw std::runtime_error("type can not accessable");
	}
};

template<bool sign, int bits>
struct int_type : public type_base
{
	// Унаследовано через type_base
	virtual size_t length() const override
	{
		return bits / 32;
	}
	virtual std::string marshal2() const override
	{
		return std::string(sign ? "" : "u") + "int" + std::to_string(bits);
	}
	virtual std::string marshal() const override
	{
		return "F6" + std::string(sign ? "s" : "u") + "int" + std::to_string(bits);
	}
};

template<int bits>
struct float_type : public type_base
{
	virtual size_t length() const override
	{
		return bits / 32;
	}
	virtual std::string marshal2() const override
	{
		if (bits == 32) {
			return "float";
		}
		else if (bits == 64) {
			return "double";
		}
		else if (bits == 16) {
			return "half";
		}
		return "float" + std::to_string(bits);
	}
	virtual std::string marshal() const override
	{
		if (bits == 32) {
			return "F5float";
		}
		else if (bits == 64) {
			return "F6double";
		}
		else if (bits == 16) {
			return "F4half";
		}
		return "F7float" + std::to_string(bits);
	}

};

struct void_type : public type_base
{
	virtual size_t length() const override
	{
		return 0;
	}
	virtual std::string marshal2() const override
	{
		return "void";
	}
	virtual std::string marshal() const override
	{
		return "F4void";
	}
};

struct bool_type : public type_base
{
	virtual size_t length() const override
	{
		return 1;
	}
	virtual std::string marshal2() const override
	{
		return "boolean";
	}

	virtual std::string marshal() const override
	{
		return "F7boolean";
	}
};

struct ptr_type : public type_base
{
	ptr_type(std::shared_ptr<const type_base> i) :
		innerType(i)
	{}

	std::shared_ptr<const type_base> innerType;

	virtual size_t length() const override
	{
		return 2;
	}
	virtual std::string marshal2() const override
	{
		return "*" + innerType->marshal2();
	}
	virtual std::string marshal() const override
	{
		return "P"  + innerType->marshal();
	}
	
};

struct vector_type : public type_base
{

	vector_type(std::shared_ptr<const type_base> i, size_t c) :
		innerType(i),
		count(c)
	{}


	std::shared_ptr<const type_base> innerType;
	const size_t     count;
	virtual size_t length() const override
	{
		return innerType->length() * count;
	}

	virtual std::string marshal() const override
	{
		return "V" + std::to_string(count) + "X" + innerType->marshal();
	}

	virtual std::string marshal2() const override
	{
		return "vector" + std::to_string(count) + "<" + innerType->marshal2() + ">";
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset)  const override
	{
		offset += innerType->length() * index;
		return std::static_pointer_cast<const type_base>(innerType);
	}

	virtual std::shared_ptr<const type_base> signleType() const override
	{
		return innerType;
	}
};

struct matrix_type : public type_base
{
	std::shared_ptr<const type_base> innerType;
	const size_t     count;


	matrix_type(std::shared_ptr<const type_base> i, size_t c) :
		innerType(i),
		count(c)
	{}


	virtual size_t length() const override
	{
		return innerType->length() * count;
	}

	virtual std::string marshal() const override
	{
		return "M" + std::to_string(count) + "X" + innerType->marshal();
	}

	virtual std::string marshal2() const override
	{
		return "matrix" + std::to_string(count) + "<" + innerType->marshal2() + ">";
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset) const override
	{
		offset += innerType->length() * index;
		return std::static_pointer_cast<const type_base>(innerType);
	}
	virtual std::shared_ptr<const type_base> signleType() const override
	{
		return innerType;
	}
};

struct array_type : public type_base
{
	std::shared_ptr<const type_base> innerType;
	const size_t     count;

	array_type(std::shared_ptr<const type_base> i, size_t c) :
		innerType(i),
		count(c)
	{}

	virtual size_t length() const override
	{
		return innerType->length() * count;
	}

	virtual std::string marshal() const override
	{
		return "A" + std::to_string(count) + "X" + innerType->marshal();
	}

	virtual std::string marshal2() const override
	{
		return "[" + std::to_string(count) + "] " + innerType->marshal2();
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset) const override
	{
		offset += innerType->length() * index;
		return std::static_pointer_cast<const type_base>(innerType);
	}
	virtual std::shared_ptr<const type_base> signleType() const override
	{
		return innerType;
	}
};

struct dynamic_array_type : public type_base
{
	std::shared_ptr<const type_base> innerType;

	dynamic_array_type(std::shared_ptr<const type_base> i) :
		innerType(i)
	{}

	virtual size_t length() const override
	{
		throw std::runtime_error("");
		return -1;
	}

	virtual std::string marshal() const override
	{
		return "Y" + innerType->marshal();
	}


	virtual std::string marshal2() const override
	{
		return "[] " + innerType->marshal2();
	}
	virtual std::shared_ptr<const type_base> signleType() const override
	{
		return innerType;
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset) const override
	{
		offset += innerType->length() * index;
		return std::static_pointer_cast<const type_base>(innerType);
	}
};

struct struct_type : public type_base
{
	template<typename X>
	struct_type(const X& a) :
		innerTypes(a.begin(), a.end())
	{}


	std::vector<std::shared_ptr<const type_base>> innerTypes;

	virtual size_t length() const override
	{
		size_t sz = 0;
		for (auto& x : innerTypes)
			sz += x->length();
		return sz;
	}


	virtual std::string marshal() const override
	{
		std::string buildType;
		size_t total = 0;
		for (size_t i = 0; i < innerTypes.size(); i++) {
			total += innerTypes[i]->marshal().size();
		}
		buildType += ("S") + std::to_string(innerTypes.size());
		for (size_t i = 0; i < innerTypes.size(); i++) {
			buildType += innerTypes[i]->marshal();
			if (i != innerTypes.size() - 1)
				buildType += ("");
		}
		return buildType;
	}

	virtual std::string marshal2() const override
	{
		std::string buildType;
		buildType += ("{");
		for (size_t i = 0; i < innerTypes.size(); i++) {
			buildType += innerTypes[i]->marshal2();
			if (i != innerTypes.size() - 1)
				buildType += (",");
		}
		buildType += "}";
		return buildType;
	}

	virtual std::shared_ptr<const type_base> accessTo(uint32_t index, uint32_t& offset) const override
	{
		for (size_t i = 0; i < index; i++) {
			offset += innerTypes[i]->length();
		}	
		return std::static_pointer_cast<const type_base>(innerTypes[index]);
	}
};


typedef std::shared_ptr<type_base> type_t;

template<typename T, typename... X>
type_t createType(X&& ... args)
{
	return std::dynamic_pointer_cast<type_base>(std::make_shared<T>(std::forward<X>(args)...));
}