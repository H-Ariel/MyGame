#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>


class json
{
private:
	enum class JsonType;
	typedef std::string Str_t;
	typedef std::vector<json> List_t;
	typedef std::map<std::string, json> Dict_t;
	typedef long long Int_t;
	typedef double Float_t;


public:
	json();
	json(bool n);
	json(short n);
	json(int n);
	json(long long n);
	json(float n);
	json(double n);
	json(const char n[]);
	json(const std::string& n);
	json(const std::initializer_list<json>& n);
	json(const std::vector<json>& n);
	json(const std::map<std::string, json>& n);
	json(const json& j);
	json(json&& j) noexcept;
	~json();

	json& operator=(bool n);
	json& operator=(short n);
	json& operator=(int n);
	json& operator=(long long n);
	json& operator=(float n);
	json& operator=(double n);
	json& operator=(const char n[]);
	json& operator=(const std::string& n);
	json& operator=(const std::initializer_list<json>& n);
	json& operator=(const std::vector<json>& n);
	json& operator=(const std::map<std::string, json>& n);
	json& operator=(const nullptr_t&);
	json& operator=(const json& j);
	json& operator=(json&& j) noexcept;

	operator const bool() const;
	operator const short() const;
	operator const int() const;
	operator const long long() const;
	operator const float() const;
	operator const double() const;
	operator const char* () const;
	operator const Str_t() const;
	operator const List_t() const;
	operator const Dict_t() const;

	json& operator[](const std::string& key);
	json& operator[](const char key[]);
	json& operator[](size_t idx);
	json& operator[](int idx);

	const json& operator[](const std::string& key) const;
	const json& operator[](const char key[]) const;
	const json& operator[](size_t idx) const;
	const json& operator[](int idx) const;

	void append(const json& json);

	bool contains(const std::string& key) const;

	void list_iterate(std::function<void(const json&)> func) const;
	void dict_iterate(std::function<void(const std::string&, const json&)> func) const;

	void clear();

	std::string ToString() const;
	static json FromString(const std::string& string);
	static json FromFile(const std::string& filename);

private:
	void SetBool(bool n);
	void SetInt(Int_t n);
	void SetFloat(Float_t n);
	void SetStr(const Str_t& n);
	void SetList(const List_t& n);
	void SetDict(const Dict_t& n);

	void* data;
	JsonType type;


	class Parser
	{
	public:
		Parser(std::string json_str);
		json Parse();

	private:
		json ReadBool();
		json ReadNumber();
		json ReadString();
		json ReadList();
		json ReadDict();
		void skipSpaces();

		const std::string json_str;
		size_t pos;
	};
};


std::ostream& operator<<(std::ostream& os, const json& j);
std::istream& operator>>(std::istream& is, json& j);
