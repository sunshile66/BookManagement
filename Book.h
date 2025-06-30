#pragma once
#include <ios>
#include <cstring>
#include <string>

#define NUM1 128
#define NUM2 50

class CBook {
public:
    CBook();
    // 修正构造函数声明
    CBook(const char* cName, const char* clsbn,
        const char* cPrice, const char* cAuthor);  // 移除多余空格

    const char* GetName() const { return m_cName; }
    const char* GetClsbn() const { return m_clsbn; }
    const char* GetPrice() const { return m_cPrice; }
    const char* GetAuthor() const { return m_cAuthor; }

    void WriteData() const;
    void UpdateData(long position) const;
    static CBook ReadData(long position);
    static void DeleteData(long position);
    static long GetRecordCount();

private:
    char m_cName[NUM1];
    char m_clsbn[NUM1];
    char m_cPrice[NUM2];
    char m_cAuthor[NUM2];
};