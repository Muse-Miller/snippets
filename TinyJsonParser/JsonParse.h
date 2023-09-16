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
	
	//����������ͣ��������ֵ䡢�б���������
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

	//ֵ�������ͣ������ַ��������֡�������nullֵ
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
		//��ȡ��һ�����ʣ��Լ��õ��ʵ����ԣ�src ʼ��ָ��δ���ʵĵ�һ���ַ�
		void GetNxtToken(const char*& src,char* buffer,JsonType& type)
		{
			memset(buffer, 0x00, 1024);
			//�����հ��ַ�
			while ((char)33 > *src) { ++src; }
			//��ȡ��һ������
			if ('{' == *src) {
				type = JsonType::JDICT;
			}
			else if ('[' == *src) {
				type = JsonType::JLIST;
			}
			else if ('"' == *src) {
				//���ַ������ݴ洢��buffer��
				int i = 0;
				do {
					++src;
					if ('"' == *src)break;
					buffer[i] = *src;
					++i;
				} while ('"' != *src);
				//�Թ�������"���������հ�
				++src;
				while ((char)32 > *src) { ++src; }
				//�ٴζ�ȡһ�����ʣ������жϸ��ַ��������ͣ�key��value
				if (':' == *src)
				{
					type = JsonType::JKEY;
				}
				else if (',' == *src)
				{
					type = JsonType::JSTR;
				}
				else
				{	//˵����ȡ���� ] ���� }�������ַ���
					//����Ҫ���������ַ�����ӵ�������У�
					//��Ҫ����һ���ַ������ⵯ�����ڵ����
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
			//ʼ�ս�srcָ��δɨ��ĵ�һ���ַ�
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
		//rootָ���¼ÿ�δ�ջ�е�����JsonObj��ָ�룬
		//�����һ�ε���ʱ��˵�����еĵ��ʶ���ɨ����ϣ��������еĶ��󶼹�����ϣ�
		//root���¼���ö�����
		std::shared_ptr<JsonObj> m_root{ nullptr };
		//��¼���������·��˳��
		//��ֻ�е�������Dict��Listʱ��Ҫ��ջ��¼����Ϊ����Ԫ�ض������к��ӣ���ֵ�ڴ���ʱ����ȷ����
		std::stack<std::shared_ptr<JsonObj> > m_record;
	};
}

#endif //__JSON_PARSER__
