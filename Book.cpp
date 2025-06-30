#include "Book.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>  // 添加 uintmax_t 支持

using namespace std;

CBook::CBook() {
    memset(m_cName, 0, NUM1);
    memset(m_clsbn, 0, NUM1);
    memset(m_cPrice, 0, NUM2);
    memset(m_cAuthor, 0, NUM2);
}

// 修正构造函数实现
CBook::CBook(const char* cName, const char* clsbn,
    const char* cPrice, const char* cAuthor) {
    strncpy_s(m_cName, NUM1, cName ? cName : "", NUM1 - 1);
    strncpy_s(m_clsbn, NUM1, clsbn ? clsbn : "", NUM1 - 1);
    strncpy_s(m_cPrice, NUM2, cPrice ? cPrice : "", NUM2 - 1);
    strncpy_s(m_cAuthor, NUM2, cAuthor ? cAuthor : "", NUM2 - 1);

    m_cName[NUM1 - 1] = '\0';
    m_clsbn[NUM1 - 1] = '\0';
    m_cPrice[NUM2 - 1] = '\0';
    m_cAuthor[NUM2 - 1] = '\0';
}

// 修正 GetRecordCount
long CBook::GetRecordCount() {
    ifstream fin("book.dat", ios::binary | ios::ate);
    if (!fin) return 0;

    const long recordSize = static_cast<long>(NUM1 + NUM1 + NUM2 + NUM2);
    const auto fileSize = fin.tellg();
    fin.close();

    if (fileSize <= 0 || static_cast<uintmax_t>(fileSize) > LONG_MAX) {
        return 0;
    }
    return static_cast<long>(fileSize) / recordSize;
}

// 修正 DeleteData 的文件操作
void CBook::DeleteData(long position) {
    ifstream in("book.dat", ios::binary);
    if (!in) throw runtime_error("无法打开源文件");

    ofstream out("temp.dat", ios::binary);
    if (!out) throw runtime_error("无法创建临时文件");

    const long recordSize = static_cast<long>(NUM1 + NUM1 + NUM2 + NUM2);
    char* buffer = new char[recordSize];
    long count = 0;

    while (in.read(buffer, recordSize)) {
        if (count++ != position) {
            out.write(buffer, recordSize);
        }
    }

    in.close();
    out.close();

    // 添加错误处理
    if (remove("book.dat") != 0) {
        throw runtime_error("删除原文件失败");
    }
    if (rename("temp.dat", "book.dat") != 0) {
        throw runtime_error("重命名文件失败");
    }
    delete[] buffer;
}
// ==== 添加到 Book.cpp 底部 ====

void CBook::WriteData() const {
    ofstream out("book.dat", ios::binary | ios::app);
    if (!out) throw runtime_error("无法打开文件进行写入");

    out.write(m_cName, NUM1);
    out.write(m_clsbn, NUM1);
    out.write(m_cPrice, NUM2);
    out.write(m_cAuthor, NUM2);
    out.close();
}

void CBook::UpdateData(long position) const {
    fstream file("book.dat", ios::binary | ios::in | ios::out);
    if (!file) throw runtime_error("无法打开文件进行更新");

    const long recordSize = NUM1 + NUM1 + NUM2 + NUM2;
    file.seekp(position * recordSize, ios::beg);

    file.write(m_cName, NUM1);
    file.write(m_clsbn, NUM1);
    file.write(m_cPrice, NUM2);
    file.write(m_cAuthor, NUM2);
    file.close();
}

CBook CBook::ReadData(long position) {
    ifstream in("book.dat", ios::binary);
    if (!in) throw runtime_error("无法打开文件进行读取");

    const long recordSize = NUM1 + NUM1 + NUM2 + NUM2;
    in.seekg(position * recordSize, ios::beg);

    char name[NUM1], isbn[NUM1], price[NUM2], author[NUM2];
    in.read(name, NUM1);
    in.read(isbn, NUM1);
    in.read(price, NUM2);
    in.read(author, NUM2);
    in.close();

    return CBook(name, isbn, price, author);
}