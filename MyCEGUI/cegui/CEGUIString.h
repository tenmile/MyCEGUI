#pragma once

#include "CEGUIBase.h"
#include <string.h>
#include <stdexcept>
#include <cstddef>

namespace CEGUI
{

//可用于快速分配的字符串长度
#define STR_QUICKBUFF_SIZE	32
	typedef		uint8	utf8;
	//typedef		uint16	utf16;  // removed typedef to prevent usage, as utf16 is not supported (yet)
	typedef		uint32	utf32;

/*!
\brief
	String class used within the GUI system.

	For the most part, this class can replace std::string in basic usage.  However, currently String does not use the
	current locale, and also comparisons do not take into account the Unicode data tables, so are not 'correct'
	as such.
*/
class String
{
public:
	//内部类型
	typedef		utf32			value_type;					//!< Basic 'code point' type used for String (utf32)
	typedef		size_t			size_type;					//!< Unsigned type used for size values and indices
	typedef		std::ptrdiff_t	difference_type;			//!< Signed type used for differences
	typedef		utf32&			reference;					//!< Type used for utf32 code point references
	typedef		const utf32&	const_reference;			//!< Type used for constant utf32 code point references
	typedef		utf32*			pointer;					//!< Type used for utf32 code point pointers
	typedef		const utf32*	const_pointer;				//!< Type used for constant utf32 code point pointers

	static const size_type		npos;						//!< Value used to represent 'not found' conditions and 'all code points' etc.

private:
	size_type	d_cplength;			//!< holds length of string in code points (not including null termination)
	size_type	d_reserve;			//!< code point reserve size (currently allocated buffer size in code points).

	mutable utf8*		d_encodedbuff;		//!< holds string data encoded as utf8 (generated only by calls to c_str() and data())
	mutable size_type	d_encodeddatlen;	//!< holds length of encoded data (in case it's smaller than buffer).
	mutable size_type	d_encodedbufflen;	//!< length of above buffer (since buffer can be bigger then the data it holds to save re-allocations).

	utf32		d_quickbuff[STR_QUICKBUFF_SIZE];	//!< This is a integrated 'quick' buffer to save allocations for smallish strings
	utf32*		d_buffer;							//!< Pointer the the main buffer memory.  This is only valid when quick-buffer is not being used

public:
	/*************************************************************************
		Iterator Classes
	*************************************************************************/
    //! regular iterator for String.指向utf32数据类型
    class iterator : public std::iterator<std::random_access_iterator_tag, utf32>
    {
    public:
        iterator() : d_ptr(0) {}
        explicit iterator(utf32* const ptr) : d_ptr(ptr) {}

        utf32& operator*() const
        {
            return *d_ptr;
        }

        utf32* operator->() const
        {
            return &**this;
        }
		//前置
        String::iterator& operator++()
        {
            ++d_ptr;
            return *this;
        }
		//后置
        String::iterator operator++(int)
        {
            String::iterator temp = *this;
            ++*this;
            return temp;
        }
		//前置
        String::iterator& operator--()
        {
            --d_ptr;
            return *this;
        }
		//后置
        String::iterator operator--(int)
        {
            String::iterator temp = *this;
            --*this;
            return temp;
        }

        String::iterator& operator+=(difference_type offset)
        {
            d_ptr += offset;
            return *this;
        }

        String::iterator operator+(difference_type offset) const
        {
            String::iterator temp = *this;
            return temp += offset;
        }

        String::iterator& operator-=(difference_type offset)
        {
            return *this += -offset;
        }

        String::iterator operator-(difference_type offset) const
        {
            String::iterator temp = *this;
            return temp -= offset;
        }

        utf32& operator[](difference_type offset) const
        {
            return *(*this + offset);
        }

        friend difference_type operator-(const String::iterator& lhs,
                                         const String::iterator& rhs)
            { return lhs.d_ptr - rhs.d_ptr; }

        friend String::iterator operator+(difference_type offset, const String::iterator& iter)
            { return iter + offset; }

        friend bool operator==(const String::iterator& lhs,
                               const String::iterator& rhs)
            { return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const String::iterator& lhs,
                               const String::iterator& rhs)
            { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const String::iterator& lhs,
                              const String::iterator& rhs)
            { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const String::iterator& lhs,
                              const String::iterator& rhs)
            { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const String::iterator& lhs,
                               const String::iterator& rhs)
            { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const String::iterator& lhs,
                               const String::iterator& rhs)
            { return lhs.d_ptr >= rhs.d_ptr; }

        utf32* d_ptr;    //地址的指针，解引用得到的是地制值
    };

    //! const iterator for String.不能改变iterator指向的对象
    class const_iterator : public std::iterator<std::random_access_iterator_tag, const utf32>
    {
    public:
        const_iterator() : d_ptr(0) {}
        explicit const_iterator(const utf32* const ptr) : d_ptr(ptr) {}
        const_iterator(const String::iterator& iter) : d_ptr(iter.d_ptr) {}

        const utf32& operator*() const
        {
            return *d_ptr;
        }

        const utf32* operator->() const
        {
            return &**this;
        }

        String::const_iterator& operator++()
        {
            ++d_ptr;
            return *this;
        }

        String::const_iterator operator++(int)
        {
            String::const_iterator temp = *this;
            ++*this;
            return temp;
        }

        String::const_iterator& operator--()
        {
            --d_ptr;
            return *this;
        }

        String::const_iterator operator--(int)
        {
            String::const_iterator temp = *this;
            --*this;
            return temp;
        }

        String::const_iterator& operator+=(difference_type offset)
        {
            d_ptr += offset;
            return *this;
        }

        String::const_iterator operator+(difference_type offset) const
        {
            String::const_iterator temp = *this;
            return temp += offset;
        }

        String::const_iterator& operator-=(difference_type offset)
        {
            return *this += -offset;
        }

        String::const_iterator operator-(difference_type offset) const
        {
            String::const_iterator temp = *this;
            return temp -= offset;
        }

        const utf32& operator[](difference_type offset) const
        {
            return *(*this + offset);
        }

        String::const_iterator& operator=(const String::iterator& iter)
        {
            d_ptr = iter.d_ptr;
            return *this;
        }

        friend String::const_iterator operator+(difference_type offset, const String::const_iterator& iter)
            { return iter + offset; }

        friend difference_type operator-(const String::const_iterator& lhs,
                                         const String::const_iterator& rhs)
            { return lhs.d_ptr - rhs.d_ptr; }

        friend bool operator==(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
            { return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
            { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const String::const_iterator& lhs,
                              const String::const_iterator& rhs)
            { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const String::const_iterator& lhs,
                              const String::const_iterator& rhs)
            { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
            { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
            { return lhs.d_ptr >= rhs.d_ptr; }

        const utf32* d_ptr;
    };

	//反向操作迭代器
	typedef	std::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef std::reverse_iterator<iterator>			reverse_iterator;

public:
    //快速比较，只是为了区分两个字符串是否相等，为了查找的方便
    struct FastLessCompare
    {
        bool operator() (const String& a, const String& b) const
        {
            const size_t la = a.length();
            const size_t lb = b.length();
            if (la == lb)
                return (memcmp(a.ptr(), b.ptr(), la*sizeof(utf32)) < 0);
            return (la < lb);
        }
    };

public:
	String(void)
	{
		init();        //将类置于最初始的状态
	}
	~String(void);

	String(const String& str)
	{
		init();
		assign(str);
	}

	String(const String& str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(str, str_idx, str_num);
	}

	//对标准库的string类进行处理
	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const std::string& std_str)
	{
		init();
		assign(std_str);
	}

	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const std::string& std_str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(std_str, str_idx, str_num);
	}

	//用utf8字符串进行初始化
	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const utf8* utf8_str)
	{
		init();
		assign(utf8_str);
	}

	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const utf8* utf8_str, size_type chars_len)
	{
		init();
		assign(utf8_str, chars_len);
	}

	//将一个utf32字符重复num遍
	//exception std::length_error	Thrown if resulting String object would be too big.
	String(size_type num, utf32 code_point)
	{
		init();
		assign(num, code_point);
	}

	String(const_iterator iter_beg, const_iterator iter_end)
	{
		init();
		append(iter_beg, iter_end); //在字符串末尾插入数据
	}

	//用c-string进行初始化
	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const char* cstr)
	{
		init();
		assign(cstr);
	}

	//exception std::length_error	Thrown if resulting String object would be too big.
	String(const char* chars, size_type chars_len)
	{
		init();
		assign(chars, chars_len);
	}

	//字符长度，不包含结尾符，内部以utf32进行编码
	size_type	size(void) const
	{
		return d_cplength;
	}

	size_type	length(void) const
	{
		return d_cplength;
	}

	bool	empty(void) const
	{
		return	(d_cplength == 0);
	}

	//最长字符数，一个字符占四个字节
	static size_type	max_size(void)
	{
		return (((size_type)-1) / sizeof(utf32));
	}

	//能够容纳的字符数
	size_type capacity(void) const
	{
		return d_reserve - 1;
	}

	//exception std::length_error	Thrown if resulting String object would be too big.
	void	reserve(size_type num = 0)
	{
		if (num == 0)
			trim();
		else
			grow(num);
	}

	int		compare(const String& str) const
	{
		return compare(0, d_cplength, str);
	}

	//exception std::out_of_range	Thrown if either \a idx or \a str_idx are invalid.
	int		compare(size_type idx, size_type len, const String& str, size_type str_idx = 0, size_type str_len = npos) const
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if ((len == npos) || (idx + len > d_cplength))
			len = d_cplength - idx;

		if ((str_len == npos) || (str_idx + str_len > str.d_cplength))
			str_len = str.d_cplength - str_idx;

		int val = (len == 0) ? 0 : utf32_comp_utf32(&ptr()[idx], &str.ptr()[str_idx], (len < str_len) ? len : str_len);

		return (val != 0) ? ((val < 0) ? -1 : 1) : (len < str_len) ? -1 : (len == str_len) ? 0 : 1;
	}

	//和标准库string进行比较，并不支持多字节编码
	int		compare(const std::string& std_str) const
	{
		return compare(0, d_cplength, std_str);
	}

	//exception std::out_of_range	Thrown if either \a idx or \a str_idx are invalid.
	int		compare(size_type idx, size_type len, const std::string& std_str, size_type str_idx = 0, size_type str_len = npos) const
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (std_str.size() < str_idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for std::string"));

		if ((len == npos) || (idx + len > d_cplength))
			len = d_cplength - idx;

		if ((str_len == npos) || (str_idx + str_len > std_str.size()))
			str_len = (size_type)std_str.size() - str_idx;

		int val = (len == 0) ? 0 : utf32_comp_char(&ptr()[idx], &std_str.c_str()[str_idx], (len < str_len) ? len : str_len);

		return (val != 0) ? ((val < 0) ? -1 : 1) : (len < str_len) ? -1 : (len == str_len) ? 0 : 1;
	}


	//和utf8字符串进行比较
	int		compare(const utf8* utf8_str) const
	{
		return compare(0, d_cplength, utf8_str, encoded_size(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid.
	int		compare(size_type idx, size_type len, const utf8* utf8_str) const
	{
		return compare(idx, len, utf8_str, encoded_size(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid.
	//exception std::length_error	Thrown if \a str_cplen is set to npos.
	int		compare(size_type idx, size_type len, const utf8* utf8_str, size_type str_cplen) const
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (str_cplen == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if ((len == npos) || (idx + len > d_cplength))
			len = d_cplength - idx;

		int val = (len == 0) ? 0 : utf32_comp_utf8(&ptr()[idx], utf8_str, (len < str_cplen) ? len : str_cplen);

		return (val != 0) ? ((val < 0) ? -1 : 1) : (len < str_cplen) ? -1 : (len == str_cplen) ? 0 : 1;
	}

	int		compare(const char* cstr) const
	{
		return compare(0, d_cplength, cstr, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid.
	int		compare(size_type idx, size_type len, const char* cstr) const
	{
		return compare(idx, len, cstr, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid.
	//exception std::length_error	Thrown if \a chars_len is set to npos.
	int		compare(size_type idx, size_type len, const char* chars, size_type chars_len) const
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if ((len == npos) || (idx + len > d_cplength))
			len = d_cplength - idx;

		int val = (len == 0) ? 0 : utf32_comp_char(&ptr()[idx], chars, (len < chars_len) ? len : chars_len);

		return (val != 0) ? ((val < 0) ? -1 : 1) : (len < chars_len) ? -1 : (len == chars_len) ? 0 : 1;
	}

	//返回左值
	reference	operator[](size_type idx)
	{
		return (ptr()[idx]);
	}

	//const对象返回右值
	value_type	operator[](size_type idx) const
	{
		return ptr()[idx];
	}

	//exception std::out_of_range	Thrown if \a idx is >= length().
	reference	at(size_type idx)
	{
		if (d_cplength <= idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		return ptr()[idx];
	}

	//exception std::out_of_range	Thrown if \a idx is >= length().
	const_reference	at(size_type idx) const
	{
		if (d_cplength <= idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		return ptr()[idx];
	}

	//返回utf8字符串
	const char* c_str(void) const
	{
		return (const char*)build_utf8_buff();
	}

	const utf8* data(void) const
	{
		return build_utf8_buff();
	}

    //返回使用中的缓冲区指针
	utf32*	ptr(void)
	{
		return (d_reserve > STR_QUICKBUFF_SIZE) ? d_buffer : d_quickbuff;
	}

	//const类型
	const utf32*	ptr(void) const
	{
		return (d_reserve > STR_QUICKBUFF_SIZE) ? d_buffer : d_quickbuff;
	}

	// copy, at most, 'len' code-points of the string, begining with code-point 'idx', into the array 'buf' as valid utf8 encoded data
	// return number of utf8 code units placed into the buffer
	//exception std::out_of_range	Thrown if \a idx was invalid for this String.
	size_type	copy(utf8* buf, size_type len = npos, size_type idx = 0) const
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (len == npos)
			len = d_cplength;

		return encode(&ptr()[idx], buf, npos, len);
	}

	//The number of utf8 code units (bytes) required to hold the specified sub-string when encoded as utf8 data.
	//exception std::out_of_range	Thrown if \a idx was invalid for this String.
	size_type	utf8_stream_len(size_type num = npos, size_type idx = 0) const
	{
		using namespace std;

		if (d_cplength < idx)
			CEGUI_THROW(out_of_range("Index was out of range for CEGUI::String object"));

		size_type	maxlen = d_cplength - idx;

		return encoded_size(&ptr()[idx], ceguimin(num, maxlen));
	}

	String&	operator=(const String& str)
	{
		return assign(str);
	}

	//exception std::out_of_range	Thrown if str_idx is invalid for \a str
	String&	assign(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
			CEGUI_THROW(std::out_of_range("Index was out of range for CEGUI::String object"));

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx)) //计算需要复制的长度
			str_num = str.d_cplength - str_idx;

		grow(str_num);
		setlen(str_num);
		memcpy(ptr(), &str.ptr()[str_idx], str_num * sizeof(utf32));

		return *this;
	}

	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	operator=(const std::string& std_str)
	{
		return assign(std_str);
	}

	//exception std::out_of_range	Thrown if \a str_idx is invalid for \a std_str
	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	assign(const std::string& std_str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (std_str.size() < str_idx)
			CEGUI_THROW(std::out_of_range("Index was out of range for std::string object"));

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
			str_num = (size_type)std_str.size() - str_idx;

		grow(str_num);
		setlen(str_num);

		while(str_num--) //倒着复制，类型不匹配，需要一个个进行转化，并没有对多字节编码进行处理
		{
			((*this)[str_num]) = static_cast<utf32>(static_cast<unsigned char>(std_str[str_num + str_idx]));
		}

		return *this;
	}

	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	operator=(const utf8* utf8_str)
	{
		return assign(utf8_str, utf_length(utf8_str));
	}

	//utf8字符串以0结尾
	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, utf_length(utf8_str));
	}


	//exception std::length_error	Thrown if the resulting String would have been too large, or if str_num is 'npos'.
	String&	assign(const utf8* utf8_str, size_type str_num)
	{
		if (str_num == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		size_type enc_sze = encoded_size(utf8_str, str_num);

		grow(enc_sze);
		//此时d_reserve是增长后的缓冲区长度
		encode(utf8_str, ptr(), d_reserve, str_num);
		setlen(enc_sze);
		return *this;
	}

	String&	operator=(utf32 code_point)
	{
		return assign(1, code_point);
	}

	//exception std::length_error	Thrown if \a num was 'npos'
	String&	assign(size_type num, utf32 code_point)
	{
		if (num == npos)
			CEGUI_THROW(std::length_error("Code point count can not be 'npos'"));

		grow(num);
		setlen(num);
		utf32* p = ptr();

		while(num--)
			*p++ = code_point;

		return *this;
	}

	String&	operator=(const char* cstr)
	{
		return assign(cstr, strlen(cstr));
	}

	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	assign(const char* cstr)
	{
		return assign(cstr, strlen(cstr));
	}

	//不支持多字节编码
	//exception std::length_error	Thrown if the resulting String would have been too large.
	String&	assign(const char* chars, size_type chars_len)
	{
		grow(chars_len);
		utf32* pt = ptr();

		for (size_type i = 0; i < chars_len; ++i)
		{
			*pt++ = static_cast<utf32>(static_cast<unsigned char>(*chars++));
		}

		setlen(chars_len);
		return *this;
	}

	void	swap(String& str)
	{
		size_type	temp_len	= d_cplength;
		d_cplength = str.d_cplength;
		str.d_cplength = temp_len;

		size_type	temp_res	= d_reserve;
		d_reserve = str.d_reserve;
		str.d_reserve = temp_res;

		utf32*		temp_buf	= d_buffer;
		d_buffer = str.d_buffer;
		str.d_buffer = temp_buf;

		// see if we need to swap 'quick buffer' data
		if (temp_res <= STR_QUICKBUFF_SIZE)
		{
			utf32		temp_qbf[STR_QUICKBUFF_SIZE];

			memcpy(temp_qbf, d_quickbuff, STR_QUICKBUFF_SIZE * sizeof(utf32));
			memcpy(d_quickbuff, str.d_quickbuff, STR_QUICKBUFF_SIZE * sizeof(utf32));
			memcpy(str.d_quickbuff, temp_qbf, STR_QUICKBUFF_SIZE * sizeof(utf32));
		}

	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String&	operator+=(const String& str)
	{
		return append(str);
	}

	//exception std::out_of_range	Thrown if \a str_idx is invalid for \a str.
	//exception std::length_error	Thrown if resulting String would be too large.
	String& append(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
			str_num = str.d_cplength - str_idx;

		grow(d_cplength + str_num);
		memcpy(&ptr()[d_cplength], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(d_cplength + str_num);
		return *this;
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String&	operator+=(const std::string& std_str)
	{
		return append(std_str);
	}

	//exception std::out_of_range	Thrown if \a str_idx is invalid for \a std_str.
	//exception std::length_error	Thrown if resulting String would be too large.
	String& append(const std::string& std_str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (std_str.size() < str_idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for std::string"));

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
			str_num = (size_type)std_str.size() - str_idx;

		size_type newsze = d_cplength + str_num;

		grow(newsze);
		utf32* pt = &ptr()[newsze-1];

		while(str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_num]));

		setlen(newsze);
		return *this;
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String&	operator+=(const utf8* utf8_str)
	{
		return append(utf8_str, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String& append(const utf8* utf8_str)
	{
		return append(utf8_str, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if resulting String would be too large, or if \a len was 'npos'
	String& append(const utf8* utf8_str, size_type len)
	{
		if (len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		encode(utf8_str, &ptr()[d_cplength], encsz, len);
		setlen(newsz);

		return *this;
	}

	//exception std::length_error	Thrown if resulting String would be too long.
	String& operator+=(utf32 code_point)
	{
		return append(1, code_point);
	}

	//exception std::length_error	Thrown if resulting String would be too long, or if \a num was 'npos'.
	String& append(size_type num, utf32 code_point)
	{
		if (num == npos)
			CEGUI_THROW(std::length_error("Code point count can not be 'npos'"));

		size_type newsz = d_cplength + num;
		grow(newsz);

		utf32* p = &ptr()[d_cplength];

		while(num--)
			*p++ = code_point;

		setlen(newsz);

		return *this;
	}

	//exception std::length_error	Thrown if resulting String would be too long.
	void	push_back(utf32 code_point)
	{
		append(1, code_point);
	}

	//在字符串末尾插入数据
	//exception std::length_error	Thrown if the resulting string would be too large.
	String&	append(const_iterator iter_beg, const_iterator iter_end)
	{
		return replace(end(), end(), iter_beg, iter_end);
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String&	operator+=(const char* cstr)
	{
		return append(cstr, strlen(cstr));
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	String& append(const char* cstr)
	{
		return append(cstr, strlen(cstr));
	}

	//exception std::length_error	Thrown if resulting String would be too large, or if \a chars_len was 'npos'
	String& append(const char* chars, size_type chars_len)
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		size_type newsz = d_cplength + chars_len;

		grow(newsz);

		utf32* pt = &ptr()[newsz-1];

		while(chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large.
	String&	insert(size_type idx, const String& str)
	{
		return insert(idx, str, 0, npos);
	}

	//exception std::out_of_range	Thrown if \a idx or \a str_idx are out of range.
	//exception std::length_error	Thrown if resulting String would be too large.
	String& insert(size_type idx, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
			str_num = str.d_cplength - str_idx;

		size_type newsz = d_cplength + str_num;
		grow(newsz);
		memmove(&ptr()[idx + str_num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large.
	String&	insert(size_type idx, const std::string& std_str)
	{
		return insert(idx, std_str, 0, npos);
	}

	//exception std::out_of_range	Thrown if \a idx or \a str_idx are out of range.
	//exception std::length_error	Thrown if resulting String would be too large.
	String& insert(size_type idx, const std::string& std_str, size_type str_idx, size_type str_num)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (std_str.size() < str_idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for std::string"));

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
			str_num = (size_type)std_str.size() - str_idx;

		size_type newsz = d_cplength + str_num;
		grow(newsz);

		memmove(&ptr()[idx + str_num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));

		utf32* pt = &ptr()[idx + str_num - 1];

		while(str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_idx + str_num]));

		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large.
	String&	insert(size_type idx, const utf8* utf8_str)
	{
		return insert(idx, utf8_str, utf_length(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large, or if \a len is 'npos'
	String& insert(size_type idx, const utf8* utf8_str, size_type len)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (len == npos)
			CEGUI_THROW(std::length_error("Length of utf8 encoded string can not be 'npos'"));

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		memmove(&ptr()[idx + encsz], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		encode(utf8_str, &ptr()[idx], encsz, len);
		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large, or if \a num is 'npos'
	String& insert(size_type idx, size_type num, utf32 code_point)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (num == npos)
			CEGUI_THROW(std::length_error("Code point count can not be 'npos'"));

		size_type newsz = d_cplength + num;
		grow(newsz);

		memmove(&ptr()[idx + num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));

		utf32* pt = &ptr()[idx + num - 1];

		while(num--)
			*pt-- = code_point;

		setlen(newsz);

		return *this;
	}

	//exception std::length_error	Thrown if resulting String would be too large, or if \a num is 'npos'
	void insert(iterator pos, size_type num, utf32 code_point)
	{
		insert(safe_iter_dif(pos, begin()), num, code_point);
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	iterator insert(iterator pos, utf32 code_point)
	{
		insert(pos, 1, code_point);
		return pos;
	}

	//exception std::length_error	Thrown if resulting String would be too large.
	void	insert(iterator iter_pos, const_iterator iter_beg, const_iterator iter_end)
	{
		replace(iter_pos, iter_pos, iter_beg, iter_end);
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large.
	String&	insert(size_type idx, const char* cstr)
	{
		return insert(idx, cstr, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	//exception std::length_error	Thrown if resulting String would be too large, or if \a chars_len is 'npos'
	String& insert(size_type idx, const char* chars, size_type chars_len)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length of char array can not be 'npos'"));

		size_type newsz = d_cplength + chars_len;

		grow(newsz);
		memmove(&ptr()[idx + chars_len], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));

		utf32* pt = &ptr()[idx + chars_len - 1];

		while(chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);

		return *this;
	}

	void	clear(void)
	{
		setlen(0);
		trim();
	}

	String& erase(void)
	{
		clear();
		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	String&	erase(size_type idx)
	{
		return erase(idx, 1);
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	String& erase(size_type idx, size_type len)
	{
        // cover the no-op case.
        if (len == 0)
            return *this;

		if (d_cplength <= idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (len == npos)
			len = d_cplength - idx;

		size_type newsz = d_cplength - len;

		//缓冲区并没有发生改变
		memmove(&ptr()[idx], &ptr()[idx + len], (d_cplength - idx - len) * sizeof(utf32));
		setlen(newsz);
		return	*this;
	}

	String& erase(iterator pos)
	{
		return erase(safe_iter_dif(pos, begin()), 1);
	}

	String& erase(iterator iter_beg, iterator iter_end)
	{
		return erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
	}

	//exception std::length_error	Thrown if the String would be too large.
	void	resize(size_type num)
	{
		resize(num, utf32());
	}

	//exception std::length_error	Thrown if the String would be too large.
	void	resize(size_type num, utf32 code_point)
	{
		if (num < d_cplength)
		{
			setlen(num);
		}
		else
		{
			append(num - d_cplength, code_point);
		}

	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(size_type idx, size_type len, const String& str)
	{
		return replace(idx, len, str, 0, npos);
	}

	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(iterator iter_beg, iterator iter_end, const String& str)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), str, 0, npos);
	}

	//exception std::out_of_range	Thrown if either \a idx, or \a str_idx are invalid
	//exception std::length_error	Thrown if the resulting String would have been too large.
	String& replace(size_type idx, size_type len, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (((str_idx + str_num) > str.d_cplength) || (str_num == npos))
			str_num = str.d_cplength - str_idx;

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + str_num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + str_num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(size_type idx, size_type len, const std::string& std_str)
	{
		return replace(idx, len, std_str, 0, npos);
	}

	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(iterator iter_beg, iterator iter_end, const std::string& std_str)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), std_str, 0, npos);
	}

	//exception std::out_of_range	Thrown if either \a idx, or \a str_idx are invalid
	//exception std::length_error	Thrown if the resulting String would have been too large.
	String& replace(size_type idx, size_type len, const std::string& std_str, size_type str_idx, size_type str_num)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (std_str.size() < str_idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for std::string"));

		if (((str_idx + str_num) > std_str.size()) || (str_num == npos))
			str_num = (size_type)std_str.size() - str_idx;

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + str_num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + str_num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		utf32* pt = &ptr()[idx + str_num - 1];

		while (str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_idx + str_num]));

		setlen(newsz);

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(size_type idx, size_type len, const utf8* utf8_str)
	{
		return replace(idx, len, utf8_str, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str)
	{
		return replace(iter_beg, iter_end, utf8_str, utf_length(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large, or if \a str_len was 'npos'.
	String& replace(size_type idx, size_type len, const utf8* utf8_str, size_type str_len)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type encsz = encoded_size(utf8_str, str_len);
		size_type newsz = d_cplength + encsz - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + encsz], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		encode(utf8_str, &ptr()[idx], encsz, str_len);

		setlen(newsz);
		return *this;
	}

	//exception std::length_error	Thrown if the resulting String would be too large, or if \a str_len was 'npos'.
	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str, size_type str_len)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), utf8_str, str_len);
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if resulting String would have been too long, or if \a num was 'npos'.
	String& replace(size_type idx, size_type len, size_type num, utf32 code_point)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (num == npos)
			CEGUI_THROW(std::length_error("Code point count can not be 'npos'"));

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		utf32* pt = &ptr()[idx + num - 1];

		while (num--)
			*pt-- = code_point;

		setlen(newsz);

		return *this;
	}

	//exception std::length_error	Thrown if resulting String would have been too long, or if \a num was 'npos'.
	String& replace(iterator iter_beg, iterator iter_end, size_type num, utf32 code_point)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), num, code_point);
	}

	//用newBeg和newEnd之间的数据代替beg和end之间的数据
	//exception std::length_error	Thrown if the resulting string would be too long.
	String& replace(iterator iter_beg, iterator iter_end, const_iterator iter_newBeg, const_iterator iter_newEnd)
	{
		if (iter_newBeg == iter_newEnd) //如新信息为空，则删除原信息
		{
			erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
		}
		else
		{
			size_type str_len = safe_iter_dif(iter_newEnd, iter_newBeg);
			size_type idx = safe_iter_dif(iter_beg, begin());
			size_type len = safe_iter_dif(iter_end, iter_beg);

			if ((len + idx) > d_cplength)
				len = d_cplength - idx;

			size_type newsz = d_cplength + str_len - len;

			grow(newsz);

			//将idx+len之后的数据往后移
			if ((idx + len) < d_cplength)
				memmove(&ptr()[idx + str_len], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

			memcpy(&ptr()[idx], iter_newBeg.d_ptr, str_len * sizeof(utf32));
			setlen(newsz);
		}

		return *this;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(size_type idx, size_type len, const char* cstr)
	{
		return replace(idx, len, cstr, strlen(cstr));
	}

	//exception std::length_error	Thrown if the resulting String would be too large.
	String& replace(iterator iter_beg, iterator iter_end, const char* cstr)
	{
		return replace(iter_beg, iter_end, cstr, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String
	//exception std::length_error	Thrown if the resulting String would be too large, or if \a chars_len was 'npos'.
	String& replace(size_type idx, size_type len, const char* chars, size_type chars_len)
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for CEGUI::String"));

		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for the char array can not be 'npos'"));

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + chars_len - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + chars_len], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		utf32* pt = &ptr()[idx + chars_len - 1];

		while (chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);
		return *this;
	}

	//exception std::length_error	Thrown if the resulting String would be too large, or if \a chars_len was 'npos'.
	String& replace(iterator iter_beg, iterator iter_end, const char* chars, size_type chars_len)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), chars, chars_len);
	}

	size_type	find(utf32 code_point, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			while (idx < d_cplength)
			{
				if (*pt++ == code_point)
					return idx;

				++idx;
			}

		}

		return npos;
	}

	size_type	rfind(utf32 code_point, size_type idx = npos) const
	{
		if (idx >= d_cplength)
			idx = d_cplength - 1;

		if (d_cplength > 0)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (*pt-- == code_point)
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find(const String& str, size_type idx = 0) const
	{
		if ((str.d_cplength == 0) && (idx < d_cplength))
			return idx;

		if (idx < d_cplength)
		{
			// loop while search string could fit in to search area
			while (d_cplength - idx >= str.d_cplength)
			{
				if (0 == compare(idx, str.d_cplength, str))
					return idx;

				++idx;
			}

		}

		return npos;
	}

	size_type	rfind(const String& str, size_type idx = npos) const
	{
		if (str.d_cplength == 0)
			return (idx < d_cplength) ? idx : d_cplength;

		if (str.d_cplength <= d_cplength)
		{
			if (idx > (d_cplength - str.d_cplength))
				idx = d_cplength - str.d_cplength;

			do
			{
				if (0 == compare(idx, str.d_cplength, str))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find(const std::string& std_str, size_type idx = 0) const
	{
		std::string::size_type sze = std_str.size();

		if ((sze == 0) && (idx < d_cplength))
			return idx;

		if (idx < d_cplength)
		{
			// loop while search string could fit in to search area
			while (d_cplength - idx >= sze)
			{
				if (0 == compare(idx, (size_type)sze, std_str))
					return idx;

				++idx;
			}

		}

		return npos;
	}

	size_type	rfind(const std::string& std_str, size_type idx = npos) const
	{
		std::string::size_type sze = std_str.size();

		if (sze == 0)
			return (idx < d_cplength) ? idx : d_cplength;

		if (sze <= d_cplength)
		{
			if (idx > (d_cplength - sze))
				idx = d_cplength - sze;

			do
			{
				if (0 == compare(idx, (size_type)sze, std_str))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find(const utf8* utf8_str, size_type idx = 0) const
	{
		return find(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	rfind(const utf8* utf8_str, size_type idx = npos) const
	{
		return rfind(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if \a str_len is 'npos'
	size_type	find(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		size_type sze = encoded_size(utf8_str, str_len);

		if ((sze == 0) && (idx < d_cplength))
			return idx;

		if (idx < d_cplength)
		{
			// loop while search string could fit in to search area
			while (d_cplength - idx >= sze)
			{
				if (0 == compare(idx, sze, utf8_str, sze))
					return idx;

				++idx;
			}

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a str_len is 'npos'
	size_type	rfind(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		size_type sze = encoded_size(utf8_str, str_len);

		if (sze == 0)
			return (idx < d_cplength) ? idx : d_cplength;

		if (sze <= d_cplength)
		{
			if (idx > (d_cplength - sze))
				idx = d_cplength - sze;

			do
			{
				if (0 == compare(idx, sze, utf8_str, sze))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find(const char* cstr, size_type idx = 0) const
	{
		return find(cstr, idx, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	rfind(const char* cstr, size_type idx = npos) const
	{
		return rfind(cstr, idx, strlen(cstr));
	}

	//exception std::length_error	Thrown if \a chars_len is 'npos'
	size_type	find(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if ((chars_len == 0) && (idx < d_cplength))
			return idx;

		if (idx < d_cplength)
		{
			// loop while search string could fit in to search area
			while (d_cplength - idx >= chars_len)
			{
				if (0 == compare(idx, chars_len, chars, chars_len))
					return idx;

				++idx;
			}

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a chars_len is 'npos'
	size_type	rfind(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if (chars_len == 0)
			return (idx < d_cplength) ? idx : d_cplength;

		if (chars_len <= d_cplength)
		{
			if (idx > (d_cplength - chars_len))
				idx = d_cplength - chars_len;

			do
			{
				if (0 == compare(idx, chars_len, chars, chars_len))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find_first_of(const String& str, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != str.find(*pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	size_type	find_first_not_of(const String& str, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == str.find(*pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	size_type	find_first_of(const std::string& std_str, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(std_str, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	size_type	find_first_not_of(const std::string& std_str, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(std_str, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_first_of(const utf8* utf8_str, size_type idx = 0) const
	{
		return find_first_of(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_first_not_of(const utf8* utf8_str, size_type idx = 0) const
	{
		return find_first_not_of(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if \a str_len was 'npos'.
	size_type	find_first_of(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (idx < d_cplength)
		{
			size_type encsze = encoded_size(utf8_str, str_len);

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(utf8_str, encsze, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a str_len was 'npos'.
	size_type	find_first_not_of(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (idx < d_cplength)
		{
			size_type encsze = encoded_size(utf8_str, str_len);

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(utf8_str, encsze, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	size_type	find_first_of(utf32 code_point, size_type idx = 0) const
	{
		return find(code_point, idx);
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_first_not_of(utf32 code_point, size_type idx = 0) const
	{
		if (idx < d_cplength)
		{
			do
			{
				if ((*this)[idx] != code_point)
					return idx;

			} while(idx++ < d_cplength);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_first_of(const char* cstr, size_type idx = 0) const
	{
		return find_first_of(cstr, idx, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_first_not_of(const char* cstr, size_type idx = 0) const
	{
		return find_first_not_of(cstr, idx, strlen(cstr));
	}

	//exception std::length_error	Thrown if \a chars_len was 'npos'.
	size_type	find_first_of(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(chars, chars_len, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a chars_len was 'npos'.
	size_type	find_first_not_of(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if (idx < d_cplength)
		{
			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(chars, chars_len, *pt++))
					return idx;

			} while (++idx != d_cplength);

		}

		return npos;
	}

	size_type	find_last_of(const String& str, size_type idx = npos) const
	{
		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != str.find(*pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find_last_not_of(const String& str, size_type idx = npos) const
	{
		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == str.find(*pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find_last_of(const std::string& std_str, size_type idx = npos) const
	{
		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(std_str, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find_last_not_of(const std::string& std_str, size_type idx = npos) const
	{
		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(std_str, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_last_of(const utf8* utf8_str, size_type idx = npos) const
	{
		return find_last_of(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_last_not_of(const utf8* utf8_str, size_type idx = npos) const
	{
		return find_last_not_of(utf8_str, idx, utf_length(utf8_str));
	}

	//exception std::length_error	Thrown if \a str_len was 'npos'.
	size_type	find_last_of(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			size_type encsze = encoded_size(utf8_str, str_len);

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(utf8_str, encsze, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a str_len was 'npos'.
	size_type	find_last_not_of(const utf8* utf8_str, size_type idx, size_type str_len) const
	{
		if (str_len == npos)
			CEGUI_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			size_type encsze = encoded_size(utf8_str, str_len);

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(utf8_str, encsze, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	size_type	find_last_of(utf32 code_point, size_type idx = npos) const
	{
		return rfind(code_point, idx);
	}

	size_type	find_last_not_of(utf32 code_point, size_type idx = npos) const
	{
		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			do
			{
				if ((*this)[idx] != code_point)
					return idx;

			} while(idx-- != 0);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_last_of(const char* cstr, size_type idx = npos) const
	{
		return find_last_of(cstr, idx, strlen(cstr));
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	size_type	find_last_not_of(const char* cstr, size_type idx = npos) const
	{
		return find_last_not_of(cstr, idx, strlen(cstr));
	}

	//exception std::length_error	Thrown if \a chars_len was 'npos'.
	size_type	find_last_of(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos != find_codepoint(chars, chars_len, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::length_error	Thrown if \a chars_len was 'npos'.
	size_type	find_last_not_of(const char* chars, size_type idx, size_type chars_len) const
	{
		if (chars_len == npos)
			CEGUI_THROW(std::length_error("Length for char array can not be 'npos'"));

		if (d_cplength > 0)
		{
			if (idx >= d_cplength)
				idx = d_cplength - 1;

			const utf32* pt = &ptr()[idx];

			do
			{
				if (npos == find_codepoint(chars, chars_len, *pt--))
					return idx;

			} while (idx-- != 0);

		}

		return npos;
	}

	//exception std::out_of_range	Thrown if \a idx is invalid for this String.
	String	substr(size_type idx = 0, size_type len = npos) const
	{
		if (d_cplength < idx)
			CEGUI_THROW(std::out_of_range("Index is out of range for this CEGUI::String"));

		return String(*this, idx, len);
	}

	//返回迭代器，指向缓冲区开始的位置
	iterator		begin(void)
	{
		return iterator(ptr());
	}

	const_iterator	begin(void) const
	{
		return const_iterator(ptr());
	}

	//d_cplength代表字符长度，不包括结尾0
	iterator		end(void)
	{
		return iterator(&ptr()[d_cplength]);
	}

	const_iterator	end(void) const
	{
		return const_iterator(&ptr()[d_cplength]);
	}

	reverse_iterator		rbegin(void)
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator	rbegin(void) const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator		rend(void)
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator	rend(void) const
	{
		return const_reverse_iterator(begin());
	}

private:
    bool	grow(size_type new_size);
    void	trim(void);
	void	setlen(size_type len)
	{
		d_cplength = len;
		ptr()[len] = (utf32)(0);
	}
	void	init(void)
	{
		d_reserve			= STR_QUICKBUFF_SIZE;
		d_encodedbuff		= 0;
		d_encodedbufflen	= 0;
		d_encodeddatlen		= 0;
        d_buffer            = 0;
		setlen(0);
	}

	// return true if the given pointer is inside the string data
	bool	inside(utf32* inptr)
	{
		if (inptr < ptr() || ptr() + d_cplength <= inptr)
			return false;
		else
			return true;
	}

	//不检测iter2是否为空值？
	size_type safe_iter_dif(const const_iterator& iter1, const const_iterator& iter2) const
	{
		return (iter1.d_ptr == 0) ? 0 : (iter1 - iter2);
	}

	// encoding functions
	// for all:
	//	src_len is in code units, or 0 for null terminated string.
	//	dest_len is in code units.
	//	returns number of code units put into dest buffer.
	size_type encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len = 0) const
	{
		// count length for null terminated source...
		if (src_len == 0)
		{
			src_len = utf_length(src);
		}

		size_type destCapacity = dest_len;

		// while there is data in the source buffer,
		for (uint idx = 0; idx < src_len; ++idx)
		{
			utf32	cp = src[idx];

			// check there is enough destination buffer to receive this encoded unit (exit loop & return if not)
			if (destCapacity < encoded_size(cp))
			{
				break;
			}

			if (cp < 0x80)
			{
				*dest++ = (utf8)cp;
				--destCapacity;
			}
			else if (cp < 0x0800)
			{
				*dest++ = (utf8)((cp >> 6) | 0xC0);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 2;
			}
			else if (cp < 0x10000)
			{
				*dest++ = (utf8)((cp >> 12) | 0xE0);
				*dest++ = (utf8)(((cp >> 6) & 0x3F) | 0x80);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 3;
			}
			else
			{
				*dest++ = (utf8)((cp >> 18) | 0xF0);
				*dest++ = (utf8)(((cp >> 12) & 0x3F) | 0x80);
				*dest++ = (utf8)(((cp >> 6) & 0x3F) | 0x80);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 4;
			}

		}

		return dest_len - destCapacity;
	}

	//返回转化后剩余的缓冲区长度
	size_type encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len = 0) const
	{
		// count length for null terminated source...
		if (src_len == 0)
		{
			src_len = utf_length(src);
		}

		size_type destCapacity = dest_len;

		// while there is data in the source buffer, and space in the dest buffer
		for (uint idx = 0; ((idx < src_len) && (destCapacity > 0));)
		{
			utf32	cp;
			utf8	cu = src[idx++];

			if (cu < 0x80)
			{
				cp = (utf32)(cu);
			}
			else if (cu < 0xE0)
			{
				cp = ((cu & 0x1F) << 6);
				cp |= (src[idx++] & 0x3F);
			}
			else if (cu < 0xF0)
			{
				cp = ((cu & 0x0F) << 12);
				cp |= ((src[idx++] & 0x3F) << 6);
				cp |= (src[idx++] & 0x3F);
			}
			else
			{
				cp = ((cu & 0x07) << 18);
				cp |= ((src[idx++] & 0x3F) << 12);
				cp |= ((src[idx++] & 0x3F) << 6);
				cp |= (src[idx++] & 0x3F);
			}

			*dest++ = cp;
			--destCapacity;
		}

		return dest_len - destCapacity;
	}

	//utf32字符对应的utf8字符长度长度
	size_type encoded_size(utf32 code_point) const
	{
		if (code_point < 0x80)
			return 1;
		else if (code_point < 0x0800)
			return 2;
		else if (code_point < 0x10000)
			return 3;
		else
			return 4;
	}

	//utf32转化为utf8所需的长度，不包含结束符
	size_type encoded_size(const utf32* buf) const
	{
		return encoded_size(buf, utf_length(buf));
	}

	//utf32转化为utf8所需的长度
	size_type encoded_size(const utf32* buf, size_type len) const
	{
		size_type count = 0;

		while (len--)
		{
			count += encoded_size(*buf++);
		}

		return count;
	}

	//utf8转化为utf32所需的长度，不包含结束符
	size_type encoded_size(const utf8* buf) const
	{
		return encoded_size(buf, utf_length(buf));
	}

	//utf8的字符串长度，包含结尾符0
	size_type encoded_size(const utf8* buf, size_type len) const
	{
		utf8 tcp;
		size_type count = 0;

		while (len--)
		{
			tcp = *buf++;
			++count;
			size_type size = 0;

			if (tcp < 0x80)
			{
			}
			else if (tcp < 0xE0)
			{
				size = 1;
				++buf;
			}
			else if (tcp < 0xF0)
			{
				size = 2;
				buf += 2;
			}
			else
			{
				size = 3;
				buf += 3;
			}

			if (len >= size)
				len -= size;
			else 
				break;
		}

		return count;
	}

	//utf8字节长度，不包含0
	size_type utf_length(const utf8* utf8_str) const
	{
		size_type cnt = 0;
		while (*utf8_str++)
			cnt++;

		return cnt;
	}

	// return number of code units in a null terminated string
	size_type utf_length(const utf32* utf32_str) const
	{
		size_type cnt = 0;
		while (*utf32_str++)
			cnt++;

		return cnt;
	}

	// build an internal buffer with the string encoded as utf8 (remains valid until string is modified).
    utf8* build_utf8_buff(void) const;

	// compare two utf32 buffers
	int	utf32_comp_utf32(const utf32* buf1, const utf32* buf2, size_type cp_count) const
	{
		if (!cp_count)
			return 0;

		while ((--cp_count) && (*buf1 == *buf2))
			buf1++, buf2++;

		return *buf1 - *buf2;
	}

	// compare utf32 buffer with char buffer (chars are taken to be code-points in the range 0x00-0xFF)
	int utf32_comp_char(const utf32* buf1, const char* buf2, size_type cp_count) const
	{
		if (!cp_count)
			return 0;

		while ((--cp_count) && (*buf1 == static_cast<utf32>(static_cast<unsigned char>(*buf2))))
			buf1++, buf2++;

		return *buf1 - static_cast<utf32>(static_cast<unsigned char>(*buf2));
	}

	// compare utf32 buffer with encoded utf8 data
	int utf32_comp_utf8(const utf32* buf1, const utf8* buf2, size_type cp_count) const
	{
		if (!cp_count)
			return 0;

		utf32	cp;
		utf8	cu;

		do
		{
			cu = *buf2++;

			if (cu < 0x80)
			{
				cp = (utf32)(cu);
			}
			else if (cu < 0xE0)
			{
				cp = ((cu & 0x1F) << 6);
				cp |= (*buf2++ & 0x3F);
			}
			else if (cu < 0xF0)
			{
				cp = ((cu & 0x0F) << 12);
				cp |= ((*buf2++ & 0x3F) << 6);
				cp |= (*buf2++ & 0x3F);
			}
			else
			{
				cp = ((cu & 0x07) << 18);
				cp |= ((*buf2++ & 0x3F) << 12);
				cp |= ((*buf2++ & 0x3F) << 6);
				cp |= (*buf2++ & 0x3F);
			}

		} while ((*buf1++ == cp) && (--cp_count));

		return (*--buf1) - cp;
	}

	// return index of first occurrence of 'code_point' in std::string 'str', or npos if none
	size_type find_codepoint(const std::string& str, utf32 code_point) const
	{
		size_type idx = 0, sze = (size_type)str.size();

		while (idx != sze)
		{
			if (code_point == static_cast<utf32>(static_cast<unsigned char>(str[idx])))
				return idx;

			++idx;
		}

		return npos;
	}

	// return index of first occurrence of 'code_point' in utf8 encoded string 'str', or npos if none.  len is in code points.
	size_type find_codepoint(const utf8* str, size_type len, utf32 code_point) const
	{
		size_type idx = 0;

		utf32	cp;
		utf8	cu;

		while (idx != len) {
			cu = *str++;

			if (cu < 0x80)
			{
				cp = (utf32)(cu);
			}
			else if (cu < 0xE0)
			{
				cp = ((cu & 0x1F) << 6);
				cp |= (*str++ & 0x3F);
			}
			else if (cu < 0xF0)
			{
				cp = ((cu & 0x0F) << 12);
				cp |= ((*str++ & 0x3F) << 6);
				cp |= (*str++ & 0x3F);
			}
			else
			{
				cp = ((cu & 0x07) << 18);
				cp |= ((*str++ & 0x3F) << 12);
				cp |= ((*str++ & 0x3F) << 6);
				cp |= (*str++ & 0x3F);
			}

			if (code_point == cp)
				return idx;

			++idx;
		}

		return npos;
	}


	// return index of first occurrence of 'code_point' in char array 'chars', or npos if none
	size_type find_codepoint(const char* chars, size_type chars_len, utf32 code_point) const
	{
		for (size_type idx = 0; idx != chars_len; ++idx)
		{
			if (code_point == static_cast<utf32>(static_cast<unsigned char>(chars[idx])))
				return idx;
		}

		return npos;
	}

};

//在类的外部，是全局函数
bool operator==(const String& str1, const String& str2);
bool operator==(const String& str, const std::string& std_str);
bool operator==(const std::string& std_str, const String& str);
bool operator==(const String& str, const utf8* utf8_str);
bool operator==(const utf8* utf8_str, const String& str);
bool operator!=(const String& str1, const String& str2);
bool operator!=(const String& str, const std::string& std_str);
bool operator!=(const std::string& std_str, const String& str);
bool operator!=(const String& str, const utf8* utf8_str);
bool operator!=(const utf8* utf8_str, const String& str);
bool operator<(const String& str1, const String& str2);
bool operator<(const String& str, const std::string& std_str);
bool operator<(const std::string& std_str, const String& str);
bool operator<(const String& str, const utf8* utf8_str);
bool operator<(const utf8* utf8_str, const String& str);
bool operator>(const String& str1, const String& str2);
bool operator>(const String& str, const std::string& std_str);
bool operator>(const std::string& std_str, const String& str);
bool operator>(const String& str, const utf8* utf8_str);
bool operator>(const utf8* utf8_str, const String& str);
bool operator<=(const String& str1, const String& str2);
bool operator<=(const String& str, const std::string& std_str);
bool operator<=(const std::string& std_str, const String& str);
bool operator<=(const String& str, const utf8* utf8_str);
bool operator<=(const utf8* utf8_str, const String& str);
bool operator>=(const String& str1, const String& str2);
bool operator>=(const String& str, const std::string& std_str);
bool operator>=(const std::string& std_str, const String& str);
bool operator>=(const String& str, const utf8* utf8_str);
bool operator>=(const utf8* utf8_str, const String& str);
bool operator==(const String& str, const char* c_str);
bool operator==(const char* c_str, const String& str);
bool operator!=(const String& str, const char* c_str);
bool operator!=(const char* c_str, const String& str);
bool operator<(const String& str, const char* c_str);
bool operator<(const char* c_str, const String& str);
bool operator>(const String& str, const char* c_str);
bool operator>(const char* c_str, const String& str);
bool operator<=(const String& str, const char* c_str);
bool operator<=(const char* c_str, const String& str);
bool operator>=(const String& str, const char* c_str);
bool operator>=(const char* c_str, const String& str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const String& str1, const String& str2);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const String& str, const std::string& std_str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const std::string& std_str, const String& str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const String& str, const utf8* utf8_str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const utf8* utf8_str, const String& str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const String& str, utf32 code_point);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(utf32 code_point, const String& str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const String& str, const char* c_str);
//exception std::length_error	Thrown if the resulting String would be too large.
String operator+(const char* c_str, const String& str);
std::ostream& operator<<(std::ostream& s, const String& str);
void swap(String& str1, String& str2);

};
