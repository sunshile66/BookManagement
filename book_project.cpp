#define NOMINMAX  // 禁用 Windows.h 中的 min/max 宏
#include "Book.h"
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <string>
#include <limits>
#include <cmath>

using namespace std;

// 设置控制台标题
void SetConsoleTitle() {
#ifdef _WIN32
    SetConsoleTitleA("图书管理系统 - 专业版");
#endif
}

// 清除屏幕
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 显示主菜单
void ShowMenu() {
    ClearScreen();
    cout << "\n===== 图书管理系统 =====\n";
    cout << "1. 添加图书\n";
    cout << "2. 显示所有图书\n";
    cout << "3. 搜索图书\n";
    cout << "4. 修改图书信息\n";
    cout << "5. 删除图书\n";
    cout << "6. 数据统计\n";
    cout << "7. 退出系统\n";
    cout << "=========================\n";
    cout << "请选择操作 (1-7): ";
}

// 显示图书详情
void DisplayBookDetails(const CBook& book, int index = -1) {
    if (index >= 0) {
        cout << "\n图书 #" << index + 1;
    }
    cout << "\n-------------------------------------";
    cout << "\n书名: " << book.GetName();
    cout << "\nISBN: " << book.GetClsbn();
    cout << "\n价格: " << book.GetPrice();
    cout << "\n作者: " << book.GetAuthor();
    cout << "\n-------------------------------------\n";
}

// 添加新书
void AddBook() {
    ClearScreen();
    cout << "\n===== 添加新书 =====\n";

    char name[NUM1], isbn[NUM1], price[NUM2], author[NUM2];

    cout << "书名: ";
    cin.getline(name, NUM1);
    cout << "ISBN: ";
    cin.getline(isbn, NUM1);
    cout << "价格: ";
    cin.getline(price, NUM2);
    cout << "作者: ";
    cin.getline(author, NUM2);

    if (strlen(name) == 0 || strlen(isbn) == 0) {
        cout << "\n错误: 书名和ISBN不能为空!\n";
        return;
    }

    CBook book(name, isbn, price, author);
    book.WriteData();

    cout << "\n图书添加成功!\n";
}

// 显示所有图书
void ListBooks() {
    ClearScreen();
    long count = CBook::GetRecordCount();

    if (count == 0) {
        cout << "\n图书库为空!\n";
        return;
    }

    cout << "\n===== 图书列表 (共" << count << "本) =====\n";
    for (long i = 0; i < count; i++) {
        CBook book = CBook::ReadData(i);
        DisplayBookDetails(book, i);
    }

    cout << "\n按回车键返回主菜单...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 搜索图书
void SearchBooks() {
    ClearScreen();
    cout << "\n===== 搜索图书 =====\n";
    cout << "1. 按书名搜索\n";
    cout << "2. 按作者搜索\n";
    cout << "3. 按ISBN搜索\n";
    cout << "4. 返回主菜单\n";
    cout << "请选择搜索方式: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > 3) return;

    char keyword[NUM1];
    cout << (choice == 1 ? "输入书名关键字: " :
        choice == 2 ? "输入作者关键字: " : "输入ISBN: ");
    cin.getline(keyword, NUM1);

    long count = CBook::GetRecordCount();
    vector<long> results;

    for (long i = 0; i < count; i++) {
        CBook book = CBook::ReadData(i);
        const char* searchField = choice == 1 ? book.GetName() :
            choice == 2 ? book.GetAuthor() : book.GetClsbn();

        if (strstr(searchField, keyword) != nullptr) {
            results.push_back(i);
        }
    }

    ClearScreen();
    if (results.empty()) {
        cout << "\n未找到匹配的图书!\n";
        return;
    }

    cout << "\n===== 搜索结果 (共" << results.size() << "本) =====\n";
    for (size_t i = 0; i < results.size(); i++) {
        CBook book = CBook::ReadData(results[i]);
        DisplayBookDetails(book, results[i]);
    }

    cout << "\n按回车键返回主菜单...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 修改图书信息
void ModifyBook() {
    ListBooks();
    long count = CBook::GetRecordCount();

    if (count == 0) return;

    cout << "\n输入要修改的图书编号 (1-" << count << "): ";
    long index;
    cin >> index;
    cin.ignore();

    if (index < 1 || index > count) {
        cout << "\n无效的编号!\n";
        return;
    }

    CBook book = CBook::ReadData(index - 1);
    DisplayBookDetails(book, index - 1);

    char name[NUM1], isbn[NUM1], price[NUM2], author[NUM2];

    cout << "\n===== 修改图书信息 (直接回车保留原值) =====\n";
    cout << "书名 (" << book.GetName() << "): ";
    cin.getline(name, NUM1);
    if (strlen(name) == 0) strcpy_s(name, book.GetName());

    cout << "ISBN (" << book.GetClsbn() << "): ";
    cin.getline(isbn, NUM1);
    if (strlen(isbn) == 0) strcpy_s(isbn, book.GetClsbn());

    cout << "价格 (" << book.GetPrice() << "): ";
    cin.getline(price, NUM2);
    if (strlen(price) == 0) strcpy_s(price, book.GetPrice());

    cout << "作者 (" << book.GetAuthor() << "): ";
    cin.getline(author, NUM2);
    if (strlen(author) == 0) strcpy_s(author, book.GetAuthor());

    CBook newBook(name, isbn, price, author);
    newBook.UpdateData(index - 1);

    cout << "\n图书信息更新成功!\n";
}

// 删除图书
void DeleteBook() {
    ListBooks();
    long count = CBook::GetRecordCount();

    if (count == 0) return;

    cout << "\n输入要删除的图书编号 (1-" << count << "): ";
    long index;
    cin >> index;
    cin.ignore();

    if (index < 1 || index > count) {
        cout << "\n无效的编号!\n";
        return;
    }

    CBook book = CBook::ReadData(index - 1);
    DisplayBookDetails(book);

    cout << "\n确认删除这本书吗? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        CBook::DeleteData(index - 1);
        cout << "\n图书删除成功!\n";
    }
    else {
        cout << "\n删除操作已取消\n";
    }
}

// 数据统计
void ShowStatistics() {
    ClearScreen();
    long count = CBook::GetRecordCount();

    if (count == 0) {
        cout << "\n图书库为空!\n";
        return;
    }

    // 作者统计
    vector<string> authors;
    for (long i = 0; i < count; i++) {
        CBook book = CBook::ReadData(i);
        authors.push_back(book.GetAuthor());
    }

    sort(authors.begin(), authors.end());
    auto last = unique(authors.begin(), authors.end());
    size_t uniqueAuthors = distance(authors.begin(), last);

    // 价格统计
    double minPrice = numeric_limits<double>::max();
    double maxPrice = 0.0;
    double totalPrice = 0.0;
    int validPrices = 0;

    for (long i = 0; i < count; i++) {
        CBook book = CBook::ReadData(i);
        const char* priceStr = book.GetPrice();
        char* endPtr;
        double price = strtod(priceStr, &endPtr);

        if (endPtr != priceStr && price > 0 && !isnan(price)) {
            minPrice = (std::min)(minPrice, price);  // 使用 (std::min) 避免宏扩展
            maxPrice = (std::max)(maxPrice, price);  // 使用 (std::max) 避免宏扩展
            totalPrice += price;
            validPrices++;
        }
    }

    cout << "\n===== 图书数据统计 =====\n";
    cout << "图书总数: " << count << " 本\n";
    cout << "作者人数: " << uniqueAuthors << " 位\n";

    if (validPrices > 0) {
        cout << fixed << setprecision(2);
        cout << "最低价格: " << minPrice << " 元\n";
        cout << "最高价格: " << maxPrice << " 元\n";
        cout << "平均价格: " << totalPrice / validPrices << " 元\n";
    }
    else {
        cout << "价格统计: 无有效价格数据\n";
    }

    cout << "\n按回车键返回主菜单...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    SetConsoleTitle();

    int choice;
    bool exitRequested = false;

    // 创建数据文件（如果不存在）
    {
        ofstream file("book.dat", ios::app | ios::binary);
        // 文件会在作用域结束时自动关闭
    }

    do {
        ShowMenu();
        cin >> choice;
        cin.ignore();

        try {
            switch (choice) {
            case 1: AddBook(); break;
            case 2: ListBooks(); break;
            case 3: SearchBooks(); break;
            case 4: ModifyBook(); break;
            case 5: DeleteBook(); break;
            case 6: ShowStatistics(); break;
            case 7:
                cout << "\n感谢使用图书管理系统，再见!\n";
                exitRequested = true;
                break;
            default:
                cout << "\n无效选择，请重新输入!\n";
            }

            if (!exitRequested && choice != 2 && choice != 6) {
                cout << "\n按回车键继续...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        catch (const exception& e) {
            cerr << "\n错误: " << e.what() << "\n";
            cout << "按回车键继续...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (!exitRequested);

    return 0;
}