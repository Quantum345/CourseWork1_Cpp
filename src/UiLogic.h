#include "Book.h"
#include "MyContainer.h"
#include "SemesterBook.h"

class UiLogic {
private:
    static MyContainer<SemesterBook> semesterBooks;
    static MyContainer<Book*> books;
public:
    static bool yesNoPrompt(const std::string& prompt, bool isYesDefault);
    static bool isIndexValid(const std::string& message, size_t idx, size_t size);

    static void fileRead();
    static void fileWrite();

    static bool semesterBooksIsEmpty();
    static void semesterBookViewList();
    static void semesterBookViewDetails();
    static void semesterBookEdit();
    static void semesterBookAdd();
    static void semesterBookRemove();

    static bool booksIsEmpty();
    static size_t getBooksCount();
    static size_t getRecordBooksCount();
    static void bookViewListAll();
    static void bookViewListBooks();
    static void bookViewListRecordBooks();
    static void bookViewDetails();
    static void bookEdit();
    static void bookAddBook();
    static void bookAddRecordBook();
    static void bookRemove();
};