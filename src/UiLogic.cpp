#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include "UiLogic.h"

MyContainer<Book*> UiLogic::books;
MyContainer<SemesterBook> UiLogic::semesterBooks;

bool UiLogic::yesNoPrompt(const std::string& prompt, bool isYesDefault) {
    char choice;
    while (true) {
        std::cout << prompt << " [" << (isYesDefault ? "Y/n" : "y/N") << "]: ";
        if (std::cin.peek() == '\n')
            std::cin.get();
        if (!std::cin.get(choice)) return false;

        if (choice == 'y' || choice == 'Y')
            return true;
        if (choice == 'n' || choice == 'N')
            return false;
        if (choice == '\n')
            return isYesDefault;
        std::cout << "Будь ласка, введіть 'y' або 'n'\n";
    }
    return false;
}

bool UiLogic::isIndexValid(const std::string& message, size_t idx, size_t size) {
    if (idx >= size) {
        std::cout << message << "\n";
        return false;
    }
    return true;
}

void UiLogic::fileRead() {
    std::string filename;
    std::cout << "Ім'я файлу: ";
    std::cin >> filename;
    std::ifstream f(filename);
    if (!f) {
        std::cout << "Не вдалося відкрити файл!\n";
        return;
    }

    f >> semesterBooks;
    std::string tag;
    size_t booksCount = 0;
    if (!(f >> tag >> booksCount) || tag != "PolymorphicBooks") {
        std::cout << "Помилка формату файлу!\n";
        return;
    }

    for (size_t i = 0; i < books.getSize(); ++i)
        delete books[i];
    books = MyContainer<Book*>();

    for (size_t i = 0; i < booksCount; ++i) {
        std::string objectTag;
        if (!(f >> objectTag)) {
            std::cout << "Помилка читання типу об'єкту!\n";
            return;
        }
        if (objectTag == "Book") {
            auto* b = new Book();
            if (!(f >> *b)) {
                std::cout << "Помилка читання книги зі сериалізованого поліморфічного масиву!\n";
                delete b;
                return;
            }
            books.add(b);
        } else if (objectTag == "RecordBook") {
            auto* rb = new RecordBook();
            if (!(f >> *rb)) {
                std::cout << "Помилка читання залікової книги зі сериалізованого поліморфічного масиву!\n";
                delete rb;
                return;
            }
            books.add(rb);
        } else {
            std::cout << "Невідомий тип об'єкту: " << objectTag << "\n";
            return;
        }
    }
    std::cout << "Успішно прочитано дані з файлу\n";
}

void UiLogic::fileWrite() {
    std::string filename;
    std::cout << "Ім'я файлу: ";
    std::cin >> filename;
    std::ofstream f(filename, std::ios::trunc);
    if (!f) {
        std::cout << "Не вдалося відкрити файл\n";
        return;
    }

    f << semesterBooks << '\n';
    f << "PolymorphicBooks " << books.getSize() << '\n';
    for (size_t i = 0; i < books.getSize(); ++i) {
        Book* ptr = books[i];
        if (dynamic_cast<RecordBook*>(ptr))
            f << "RecordBook " << *ptr << '\n';
        else
            f << "Book " << *ptr << '\n';
    }
    std::cout << "Успішно записано усі дані у файл\n";
}

bool UiLogic::semesterBooksIsEmpty() {
    if (semesterBooks.isEmpty()) {
        std::cout << "Список семестрових книг порожній\n";
        return true;
    }
    return false;
}

void UiLogic::semesterBookViewList() {
    if (semesterBooksIsEmpty()) return;

    size_t i = 0;
    for (const auto& sb : semesterBooks) {
        std::cout << i++ << ") Рік: " << sb.getYear()
            << " Кількість залікових книг: " << sb.getRecordBooksAmount()
            << " Середній рейтинг: " << (sb.getRecordBooksAmount()
                                         ? "-"
                                         : std::to_string(sb.getAverageRating()))
            << '\n';
    }
}

void UiLogic::semesterBookViewDetails() {
    if (semesterBooksIsEmpty()) return;
    if (yesNoPrompt("Вивести список семестрових книг?", false))
        semesterBookViewList();

    size_t idx;
    std::cout << "Індекс семестрової книги: ";
    std::cin >> idx;
    if (!isIndexValid("Семестрова книга з вказаним індексом відсутня",
                      idx, semesterBooks.getSize()))
        return;
    const auto& sb = semesterBooks[idx];

    std::cout << "\nРік: " << sb.getYear() << '\n'
        << "Середній рейтинг: " << sb.getAverageRating() << '\n';

    auto retaking = sb.getRetakingCourseStudents();
    std::cout << "На повторне вивчення (" << retaking.getSize() << "):\n";
    for (const auto& rb : retaking) {
        std::cout << "  ";
        rb.printId(std::cout);
        std::cout << '\n';
    }

    const auto& rbs = sb.getRecordBooks();
    std::cout << "\nЗалікові книжки (" << rbs.getSize() << "):\n";
    size_t i = 0;
    for (const auto& rb : rbs) {
        std::cout << "  " << i++ << ") ";
        rb.printId(std::cout);
        std::cout << "  Студент: " << rb.getStudent()
            << "  Рейтинг: " << rb.getRating() << '\n';
    }
}

void UiLogic::semesterBookEdit() {
    if (semesterBooksIsEmpty()) return;
    if (yesNoPrompt("Вивести список семестрових книг?", false))
        semesterBookViewList();

    size_t idx;
    std::cout << "Індекс семестрової книги: ";
    std::cin >> idx;
    if (!isIndexValid("Семестрова книга з вказаним індексом відсутня",
                      idx, semesterBooks.getSize()))
        return;
    auto& sb = semesterBooks[idx];

    std::cout << "Поточний рік: " << sb.getYear() << '\n';
    std::cout << "Новий рік (0 — не змінювати): ";
    unsigned int newYear;
    std::cin >> newYear;
    if (newYear != 0)
        sb.setYear(newYear);

    while (yesNoPrompt("\nДодати залікову книгу з масиву?", false)) {
        if (getRecordBooksCount() == 0) {
            std::cout << "У списку немає залікових книг\n";
            break;
        }

        if (yesNoPrompt("Вивести список залікових книг?", false))
            bookViewListRecordBooks();

        size_t rbIdx;
        std::cout << "Індекс залікової книги: ";
        std::cin >> rbIdx;
        if (!isIndexValid("Книга з вказаним індексом відсутня", rbIdx, books.getSize()))
            continue;

        if (const auto& ptrRb = dynamic_cast<RecordBook const*>(books[rbIdx])) {
            sb.add(*ptrRb);
            std::cout << "Залікову книгу додано до семестрової книги\n";
        } else
            std::cout << "Помилка: елемент не є заліковою книгою.\n";
    }
    std::cout << "Збережено\n";
}

void UiLogic::semesterBookAdd() {
    unsigned int year;
    std::cout << "Рік: ";
    std::cin >> year;
    SemesterBook sb;
    sb.setYear(year);

    size_t n;
    std::cout << "Кількість залікових книжок: ";
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::string student, university;
        unsigned int sy, ay;
        std::cout << "Студент: ";
        std::getline(std::cin >> std::ws, student);
        std::cout << "Університет: ";
        std::getline(std::cin >> std::ws, university);
        std::cout << "Рік вступу: ";
        std::cin >> sy;
        std::cout << "Курс: ";
        std::cin >> ay;

        RecordBook rb(student, university, sy, ay, nullptr, 0);

        size_t subjectsAmount;
        std::cout << "Кількість предметів: ";
        std::cin >> subjectsAmount;
        for (size_t j = 0; j < subjectsAmount; ++j) {
            std::string name; unsigned int grade;
            std::cout << "Предмет: ";
            std::getline(std::cin >> std::ws, name);
            std::cout << "Оцінка: ";
            std::cin >> grade;
            rb.addSubject(Subject(name, grade));
        }
        sb.add(std::move(rb));
    }
    semesterBooks.add(std::move(sb));
    std::cout << "Додано\n";
}

void UiLogic::semesterBookRemove() {
    if (semesterBooksIsEmpty()) return;
    if (yesNoPrompt("Вивести список семестрових книг?", false))
        semesterBookViewList();

    size_t idx;
    std::cout << "Індекс семестрової книги для видалення: ";
    std::cin >> idx;
    if (!isIndexValid("Семестрова книга з вказаним індексом відсутня",
                      idx, semesterBooks.getSize()))
        return;
    semesterBooks.erase_at(idx);
    std::cout << "Видалено\n";
}

bool UiLogic::booksIsEmpty() {
    if (books.isEmpty()) {
        std::cout << "Список книг та залікових книг порожній\n";
        return true;
    }
    return false;
}

size_t UiLogic::getBooksCount() {
    size_t count = 0;
    for (const auto& b : books)
        if (dynamic_cast<RecordBook const*>(b) == nullptr)
            ++count;
    return count;
}

size_t UiLogic::getRecordBooksCount() {
    size_t count = 0;
    for (const auto& b : books)
        if (dynamic_cast<RecordBook const*>(b))
            ++count;
    return count;
}

void UiLogic::bookViewListAll() {
    if (booksIsEmpty()) return;

    size_t i = 0;
    for (const auto& b : books) {
        std::cout << i++ << ") " << (dynamic_cast<RecordBook const*>(b)
                                     ? "Залікова книга   "
                                     : "Книга   ");
        b->printId(std::cout);
        std::cout << "   Ім'я: " << b->getName() << '\n';
    }
}

void UiLogic::bookViewListBooks() {
    if (booksIsEmpty()) return;
    if (getBooksCount() == 0) {
        std::cout << "У списку немає книг\n";
        return;
    }

    for (size_t i = 0; i < books.getSize(); ++i)
        if (books[i] && dynamic_cast<RecordBook const*>(books[i]) == nullptr) {
            const auto& ptrB = books[i];
            std::cout << i << ") ";
            ptrB->printId(std::cout);
            std::cout << "  Ім'я: " << ptrB->getName()
                << "  Автор: " << ptrB->getAuthor()
                << "  Рік: " << ptrB->getYear()
                << '\n';
        }
}

void UiLogic::bookViewListRecordBooks() {
    if (booksIsEmpty()) return;
    if (getRecordBooksCount() == 0) {
        std::cout << "У списку немає залікових книг\n";
        return;
    }

    for (size_t i = 0; i < books.getSize(); ++i)
        if (const auto& ptrRb = dynamic_cast<RecordBook const*>(books[i])) {
            std::cout << i << ") ";
            ptrRb->printId(std::cout);
            std::cout << "  Ім'я: " << ptrRb->getName()
                << "  Курс: " << ptrRb->getAcademicYear()
                << "  Студент: " << ptrRb->getStudent()
                << "  Університет: " << ptrRb->getUniversity()
                << "  Кількість предметів: " << ptrRb->getSubjectsAmount()
                << '\n';
        }
}

void UiLogic::bookViewDetails() {
    if (booksIsEmpty()) return;
    if (yesNoPrompt("Вивести список книг?", false))
        bookViewListAll();

    size_t idx;
    std::cout << "Індекс книги: ";
    std::cin >> idx;
    if (!isIndexValid("Книга з вказаним індексом відсутня",
                      idx, books.getSize()))
        return;

    books[idx]->printId(std::cout);
    if (auto prb = dynamic_cast<const RecordBook*>(books[idx])) {
        const RecordBook& rb = *prb;

        std::cout << '\n'
            << "Студент:     " << rb.getStudent() << '\n'
            << "Університет: " << rb.getUniversity() << '\n'
            << "Рік вступу:  " << rb.getAcademicYear() << '\n'
            << "УДК:         " << rb.getUdcCode().getCode() << '\n'
            << "Назва:       " << rb.getName() << '\n'
            << "Автор:       " << rb.getAuthor() << '\n'
            << "Видавництво: " << rb.getPublishingHouse() << '\n'
            << "Рейтинг:     " << rb.getRating() << '\n';

        const auto& subjects = rb.getSubjects();
        std::cout << "\nПредмети (" << subjects.getSize() << "):\n";
        for (size_t i = 0; i < subjects.getSize(); ++i) {
            const auto& s = subjects[i];
            std::cout << "  " << (i + 1) << ") "
                << s.getName()
                << "    Бал: " << s.getGrade()
                << "    ECTS: " << s.getEctsGrade()
                << "    Національна оцінка: " << s.getNationalGrade() << '\n';
        }
    } else {
        const Book& b = *books[idx];
        std::cout << '\n'
            << "УДК:         " << b.getUdcCode().getCode() << '\n'
            << "Автор:       " << b.getAuthor() << '\n'
            << "Назва:       " << b.getName() << '\n'
            << "Видавництво: " << b.getPublishingHouse() << '\n'
            << "Рік:         " << b.getYear() << '\n';
    }
}

void UiLogic::bookEdit() {
    if (booksIsEmpty()) return;
    if (yesNoPrompt("Вивести список книг?", false))
        bookViewListAll();

    size_t idx;
    std::cout << "Індекс книги: ";
    std::cin >> idx;
    if (!isIndexValid("Книга з вказаним індексом відсутня",
                      idx, books.getSize()))
        return;

    if (auto prb = dynamic_cast<RecordBook*>(books[idx])) {
        RecordBook& rb = *prb;

        std::string val;
        std::cout << "Студент [" << rb.getStudent() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) rb.setStudent(val);

        std::cout << "Університет [" << rb.getUniversity() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) rb.setUniversity(val);

        std::cout << "Курс [" << rb.getAcademicYear() << "] (0 — залишити): ";
        unsigned int ay;
        std::cin >> ay;
        if (ay != 0) rb.setAcademicYear(ay);

        std::cout << "УДК [" << rb.getUdcCode().getCode() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) rb.setUdcCode(UdcCode(val));

        std::cout << "Автор [" << rb.getAuthor() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) rb.setAuthor(val);

        std::cout << "Видавництво [" << rb.getPublishingHouse() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) rb.setPublishingHouse(val);
    } else {
        Book& b = *books[idx];

        std::string val;
        std::cout << "УДК [" << b.getUdcCode().getCode() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) b.setUdcCode(UdcCode(val));

        std::cout << "Автор [" << b.getAuthor() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) b.setAuthor(val);

        std::cout << "Назва [" << b.getName() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) b.setName(val);

        std::cout << "Видавництво [" << b.getPublishingHouse() << "] (Enter — залишити): ";
        std::getline(std::cin, val);
        if (!val.empty()) b.setPublishingHouse(val);

        std::cout << "Рік [" << b.getYear() << "] (0 — залишити): ";
        unsigned int yr;
        std::cin >> yr;
        if (yr != 0) b.setYear(yr);
    }
    std::cout << "Збережено\n";
}

void UiLogic::bookAddBook() {
    std::string code, author, name, pub;
    unsigned int year;
    std::cout << "\nВказані зірочкою (*) поля є обов'язковими\n"
        << "Назва*: ";
    std::getline(std::cin >> std::ws, name);
    std::cout << "Автор: ";
    std::getline(std::cin, author);
    std::cout << "Рік*: ";
    std::cin >> year;
    if (std::cin.peek() == '\n') std::cin.get();
    std::cout << "Видавництво: ";
    std::getline(std::cin, pub);
    std::cout << "УДК: ";
    std::getline(std::cin, code);
    books.add(new Book(UdcCode(code), author, name, pub, year));
    std::cout << "Додано\n";
}

void UiLogic::bookAddRecordBook() {
    std::string student, university;
    unsigned int sy, ay;
    std::cout << "\nВказані зірочкою (*) поля є обов'язковими\n"
        << "Студент*: ";
    std::getline(std::cin >> std::ws, student);
    std::cout << "Університет*: ";
    std::getline(std::cin >> std::ws, university);
    std::cout << "Рік вступу*: ";
    std::cin >> sy;
    std::cout << "Курс*: ";
    std::cin >> ay;

    auto* ptrRb = new RecordBook(student, university, sy, ay, nullptr, 0);

    if (yesNoPrompt("Заповнити поля книги — УДК, автор, видавництво?", false)) {
        std::string code, author, pub;
        std::cout << "УДК: ";
        std::getline(std::cin, code);
        std::cout << "Автор: ";
        std::getline(std::cin, author);
        std::cout << "Видавництво: ";
        std::getline(std::cin, pub);
        ptrRb->setUdcCode(UdcCode(code));
        ptrRb->setAuthor(author);
        ptrRb->setPublishingHouse(pub);
    }

    size_t subjectsAmount;
    std::cout << "Кількість предметів: ";
    std::cin >> subjectsAmount;
    for (size_t j = 0; j < subjectsAmount; ++j) {
        std::string name;
        unsigned int grade;
        std::cout << "Предмет: ";
        std::getline(std::cin >> std::ws, name);
        std::cout << "Оцінка: ";
        std::cin >> grade;
        ptrRb->addSubject(Subject(name, grade));
    }

    books.add(ptrRb);
    std::cout << "Додано\n";
}

void UiLogic::bookRemove() {
    if (booksIsEmpty()) return;
    if (yesNoPrompt("Вивести список книг?", false))
        bookViewListAll();

    size_t idx;
    std::cout << "Індекс книги: ";
    std::cin >> idx;
    if (!isIndexValid("Книга з вказаним індексом відсутня",
                      idx, semesterBooks.getSize()))
        return;

    delete books[idx];
    books.erase_at(idx);
    std::cout << "Видалено\n";
}