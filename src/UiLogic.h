#include "Book.h"
#include "MyContainer.h"
#include "SemesterBook.h"

class UiLogic {
private:
    static MyContainer<SemesterBook> semesterBooks;
    static MyContainer<Book*> books;
public:
    static void fileRead();
    static void fileWrite();

    static void semesterBookViewList();
    static void semesterBookViewDetails();
    static void semesterBookEdit();
    static void semesterBookAdd();
    static void semesterBookRemove();

    static void bookViewListAll();
    static void bookViewListBooks();
    static void bookViewListRecordBooks();
    static void bookViewDetails();
    static void bookEdit();
    static void bookAddBook();
    static void bookAddRecordBook();
    static void bookRemove();
};