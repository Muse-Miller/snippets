#include<string>
#include<unordered_map>
#include<map>
#include <memory>
#include <stack>
#include<fstream>
#include<sstream>
#include<iostream>

#ifndef __JSON_PARSER__
#define __JSON_PARSER__


namespace self_json
{
	enum class JsonType {
		JINVALID = 0,
		JKEY = 1,
		JDICT = 2,
		JLIST = 3,
		JSTR = 4,
		JNUM = 5,
		JBOOL = 6,
		JNULL = 7,
		JOBJEND = 8,
		JEND = 9,
		JSPILT = 10
	}; 
	//enum class TokenType{};
	class JsonData
	{
	public:
		JsonData(JsonType type, int level,std::string key) :m_type(type), m_level(level),m_key(key)
		{}
		~JsonData() {}

		virtual std::string JsonToString() { std::string ret; return ret; };

		void SetType(JsonType type)
		{
			m_type = type;
		}
		JsonType GetType() { return m_type; }

		void SetKey(std::string key)
		{
			m_key = key;
		}
		std::string GetKey() { return m_key; }

		void SetLevel(int level)
		{
			m_level = level;
		}
		int GetLevel() { return m_level; }
	private:
		JsonType	m_type;
		int			m_level;
		std::string	m_key;
	};
	
	//组合数据类型，包含：字典、列表两种类型
	class JsonObj final :public JsonData
	{
	public:
		JsonObj(JsonType type, int level, std::string key) :JsonData(type, level,key)
		{}
		
		~JsonObj() {};

		void AddChild(std::shared_ptr<JsonData> child)
		{
			m_children.push_back(child);
		}
		virtual std::string JsonToString() override
		{
			std::string ret;
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (0 < GetKey().size())
			{
				ret += ('"' + GetKey() + '"');
				ret += ':';
			}
			if (JsonType::JDICT == GetType())
			{
				ret += "{\n";
			}
			else
			{
				ret += "[\n";
			}
			//
			for (auto child : m_children)
			{
				ret += child->JsonToString();
				if (child == m_children[m_children.size() - 1])
				{
					ret += "\n";
				}
				else
				{
					ret += ",\n";
				}
			}
			//
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (JsonType::JDICT == GetType())
			{
				ret += "}" ;
			}
			else
			{
				ret += "]";
			}
			return ret;
		}
	private:
		std::vector<std::shared_ptr<JsonData> > m_children;
	};

	//值数据类型：包含字符串、数字、布尔、null值
	class JsonStr final :public JsonData
	{
	public:
		JsonStr(int level, std::string key, std::string value) :JsonData(JsonType::JSTR, level, key), m_value(value)
		{}
		
		~JsonStr(){}

		void SetValue(std::string value)
		{
			m_value.swap(value);
		}
		std::string GetValue() { return m_value; }
		virtual std::string JsonToString() override
		{
			std::string ret;
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (0 < GetKey().size())
			{
				ret += ('"' + GetKey() + '"');
				ret += ':';
			}
			ret += ('"' + m_value + '"');
			return ret;
		}
	private:
	private:
		std::string m_value;
	};
	class JsonNum final :public JsonData
	{
	public:
		JsonNum(int level, std::string key, double value) :JsonData(JsonType::JSTR, level, key), m_value(value)
		{}

		~JsonNum() {}

		void SetValue(double value)
		{
			m_value = value;
		}
		double GetValue() { return m_value; }
		virtual std::string JsonToString() override
		{
			std::string ret;
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (0 < GetKey().size())
			{
				ret += ('"' + GetKey() + '"');
				ret += ':';
			}
			ret += std::to_string(m_value).data();
			return ret;
		}
	private:
	private:
		double m_value;
	};
	class JsonBool final :public JsonData
	{
	public:
		JsonBool(int level, std::string key, bool value) :JsonData(JsonType::JBOOL, level, key), m_value(value)
		{}

		~JsonBool() {}

		void SetValue(bool value)
		{
			m_value = value;
		}
		bool GetValue() { return m_value; }
		virtual std::string JsonToString() override
		{
			std::string ret;
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (0 < GetKey().size())
			{
				ret += ('"' + GetKey() + '"');
				ret += ':';
			}
			if (m_value)ret += "true";
			else ret += "false";

			return ret;
		}
	private:
	private:
		bool m_value;
	};
	class JsonNull final:public JsonData
	{
	public:
		JsonNull(int level,std::string key):JsonData(JsonType::JNULL,level,key)
		{}
		virtual std::string JsonToString() override
		{
			std::string ret;
			for (int i = 0; i < GetLevel(); ++i)
			{
				ret += '\t';
			}
			if (0 < GetKey().size())
			{
				ret += ('"' + GetKey() + '"');
				ret += ':';
			}
			ret += "null";

			return ret;
		}
	private:
	private:
	};

#define LOG(X) std::cout<<#X<<std::endl;
#ifdef LOG(X)
#undef LOG(X)
#endif
#define LOG(X) //DEBUG
	class JsonParser
	{
	public:
		void Test(const char* src)
		{
			RunParser(src);
			std::cout << m_root->JsonToString() << std::endl;;
			//for(auto child : m_root->m_children)
		}
	private:
		void RunParser(const char* src)
		{
			while (JsonType::JEND != m_token_type)
			{
				GetNxtToken(src, m_buffer, m_token_type);

				if (JsonType::JDICT == m_token_type)
				{
					std::shared_ptr<JsonObj> child = std::make_shared<JsonObj>
						(JsonType::JDICT, m_record.size(), m_token_key);//m_record.top()->GetLevel() + 1

					LOG(DICT);
					m_token_key.clear();
					if (!m_record.empty())
					{
						m_record.top()->AddChild(child);
					}
					m_record.push(child);
				}
				else if (JsonType::JLIST == m_token_type)
				{
					std::shared_ptr<JsonObj> child = std::make_shared<JsonObj>
						(JsonType::JLIST, m_record.top()->GetLevel() + 1, m_token_key);
					LOG(LIST);
					m_token_key.clear();
					m_record.top()->AddChild(child);
					m_record.push(child);
				}
				else if (JsonType::JKEY == m_token_type)
				{
					m_token_key = m_buffer;
				}
				else if (JsonType::JSTR == m_token_type)
				{
					std::shared_ptr<JsonData> child = std::make_shared<JsonStr>
						(m_record.top()->GetLevel() + 1, m_token_key, m_buffer);
					LOG(STR);
					m_token_key.clear();
					m_record.top()->AddChild(child);
				}
				else if (JsonType::JNUM == m_token_type)
				{
					std::string res = m_buffer;
					double value = std::stod(res);

					std::shared_ptr<JsonData> child = std::make_shared<JsonNum>
						(m_record.top()->GetLevel() + 1, m_token_key, value);
					LOG(NUM);
					m_token_key.clear();
					m_record.top()->AddChild(child);
				}
				else if (JsonType::JBOOL == m_token_type)
				{
					std::string res = m_buffer;
					bool value{false};
					if ('t' == res.at(0)) { value = true; }
					std::shared_ptr<JsonData> child = std::make_shared<JsonBool>
						(m_record.top()->GetLevel() + 1, m_token_key, value);
					LOG(BOOL);
					m_token_key.clear();
					m_record.top()->AddChild(child);
				}
				else if (JsonType::JNULL == m_token_type)
				{
					std::shared_ptr<JsonData> child = std::make_shared<JsonNull>
						(m_record.top()->GetLevel() + 1, m_token_key);
					LOG(NULL);
					m_token_key.clear();
					m_record.top()->AddChild(child);
				}
				else if (JsonType::JOBJEND == m_token_type)
				{
					LOG(OBJEND);
					m_root = m_record.top();
					m_record.pop();
				}
				else if (JsonType::JSPILT == m_token_type) 
				{
					continue;
				}
				else if (JsonType::JINVALID == m_token_type)
				{
					break;
				}
				else
				{
					break;
				}
			}
		}
		//获取下一个单词，以及该单词的属性，src 始终指向未访问的第一个字符
		void GetNxtToken(const char*& src,char* buffer,JsonType& type)
		{
			memset(buffer, 0x00, 1024);
			//跳过空白字符
			while ((char)33 > *src) { ++src; }
			//读取下一个单词
			if ('{' == *src) {
				type = JsonType::JDICT;
			}
			else if ('[' == *src) {
				type = JsonType::JLIST;
			}
			else if ('"' == *src) {
				//将字符串内容存储到buffer中
				int i = 0;
				do {
					++src;
					if ('"' == *src)break;
					buffer[i] = *src;
					++i;
				} while ('"' != *src);
				//略过后引号"，并跳过空白
				++src;
				while ((char)32 > *src) { ++src; }
				//再次读取一个单词，用以判断该字符串的类型：key、value
				if (':' == *src)
				{
					type = JsonType::JKEY;
				}
				else if (',' == *src)
				{
					type = JsonType::JSTR;
				}
				else
				{	//说明读取到了 ] 或者 }这两个字符，
					//除了要将读到的字符串添加到父结点中，
					//还要回退一个字符，避免弹出父节点出错
					type = JsonType::JSTR;
					--src;
				}
			}
			else if (',' == *src)
			{
				type = JsonType::JSPILT;
			}
			else if ('t' == *src) {
				int i = 0;
				while (',' != *src && ']' != *src && '}' != *src)
				{
					buffer[i] = *src;
					++i;
					++src;
				}
				if (']' == *src || '}' == *src)
				{
					--src;
				}
				type = JsonType::JBOOL;
			}
			else if ('f' == *src) {
				int i = 0;
				while (',' != *src && ']' != *src && '}' != *src)
				{
					buffer[i] = *src;
					++i;
					++src;
				}
				if (']' == *src || '}' == *src)
				{
					--src;
				}
				type = JsonType::JBOOL;
			}
			else if ('n' == *src) {
				int i = 0;
				while (',' != *src && ']' != *src && '}' != *src)
				{
					buffer[i] = *src;
					++i;
					++src;
				}
				if (']' == *src || '}' == *src)
				{
					--src;
				}
				type = JsonType::JNULL;
			}
			else if (']' == *src) {
				type = JsonType::JOBJEND;
			}
			else if ('}' == *src) {
				type = JsonType::JOBJEND;
			}
			else if (IsNum(src)) {
				int i = 0;
				while (',' != *src && ']' != *src && '}' != *src)
				{
					buffer[i] = *src;
					++i;
					++src;
				}
				if (']' == *src || '}' == *src)
				{
					--src;
				}
				type = JsonType::JNUM;
			}
			else if (char(33) > *src && '\0' != *src)
			{
				type = JsonType::JINVALID;
			}
			else if('\0' == *src) {
				type = JsonType::JEND;
				return;
			}
			else
			{
				type = JsonType::JINVALID;
			}
			//始终将src指向未扫描的第一个字符
			++src;
		}
		bool IsNum(const char* src)
		{
			if (
				('+' ^ *src) &&
				('-' ^ *src) &&
				('0' ^ *src) &&
				('1' ^ *src) &&
				('2' ^ *src) &&
				('3' ^ *src) &&
				('4' ^ *src) &&
				('5' ^ *src) &&
				('6' ^ *src) &&
				('7' ^ *src) &&
				('8' ^ *src) &&
				('9' ^ *src))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	private:
		char m_buffer[1024];
		JsonType m_token_type;
		std::string m_token_key;
		//root指针记录每次从栈中弹出的JsonObj的指针，
		//当最后一次弹出时，说明所有的单词都已扫描完毕，并且所有的对象都构造完毕，
		//root会记录整棵对象树
		std::shared_ptr<JsonObj> m_root{ nullptr };
		//记录解析对象的路径顺序
		//（只有当解析到Dict、List时需要入栈记录，因为其他元素都不会有孩子，且值在创建时就已确定）
		std::stack<std::shared_ptr<JsonObj> > m_record;
	};
}

#endif //__JSON_PARSER__
