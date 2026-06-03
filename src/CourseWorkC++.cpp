#include <iostream>
#include <functional>
#include <utility>
#include <limits>
#include <string>
#include "UiLogic.h"
#include "MyContainer.h"

struct MenuItem {
    std::string label;
    std::function<void()> action;
    MyContainer<MenuItem> submenu;

    MenuItem() = default;

    explicit MenuItem(std::string l, std::function<void()> a = [] {})
        : label(std::move(l)), action(std::move(a)) {}

    MenuItem(std::string l, MyContainer<MenuItem> ch)
        : label(std::move(l)), submenu(std::move(ch)) {}
};

int getChoice(const MyContainer<MenuItem>& items) {
    for (size_t i = 0; i < items.getSize(); ++i)
        std::cout << "[" << i << "] " << items[i].label << '\n';

    std::cout << ">>> ";
    int choice;
    if (!(std::cin >> choice) || choice < 0 || choice >= items.getSize()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return choice;
}

void runMenu(
    const MyContainer<MenuItem>& menu,
    const std::string& title = "",
    const MyContainer<std::string>& parents = {}
) {
    MyContainer<std::string> path = parents;
    if (!title.empty())
        path.add(title);

    while (true) {
        if (!path.isEmpty()) {
            std::cout << "\n[" << path[0];
            for (size_t i = 1; i < path.getSize(); ++i)
                std::cout << "] > [" << path[i];
            std::cout << "]\n";
        }

        int choice = getChoice(menu);
        if (choice == -1) {
            std::cout << "Вказаний номер пункту відсутній\n";
            continue;
        }
        const MenuItem& item = menu[choice];

        if (choice == 0)
            break;
        if (item.action)
            item.action();
        if (!item.submenu.isEmpty())
            runMenu(item.submenu, item.label, path);
    }
}

const MyContainer rootMenu{
    MenuItem{"Вихід"},

    MenuItem{"Файл", MyContainer<MenuItem>{
        MenuItem{"Повернутися назад"},
        MenuItem{"Прочитати", UiLogic::fileRead},
        MenuItem{"Записати", UiLogic::fileWrite}
    }},

    MenuItem{"Семестрові книги", MyContainer<MenuItem>{
        MenuItem{"Повернутися назад"},
        MenuItem{"Переглянути", MyContainer<MenuItem>{
            MenuItem{"Повернутися назад"},
            MenuItem{"Список", UiLogic::semesterBookViewList},
            MenuItem{"Детально про семестрову книгу", UiLogic::semesterBookViewDetails}
        }},
        MenuItem{"Редагувати", UiLogic::semesterBookEdit},
        MenuItem{"Додати", UiLogic::semesterBookAdd},
        MenuItem{"Видалити", UiLogic::semesterBookRemove}
    }},

    MenuItem{"Книги та залікові книги", MyContainer<MenuItem>{
        MenuItem{"Повернутися назад"},
        MenuItem{"Переглянути", MyContainer<MenuItem>{
            MenuItem{"Повернутися назад"},
            MenuItem{"Список", MyContainer<MenuItem>{
                MenuItem{"Повернутися назад"},
                MenuItem{"З усіма книгами", UiLogic::bookViewListAll},
                MenuItem{"Тільки книги", UiLogic::bookViewListBooks},
                MenuItem{"Тільки залікові книги", UiLogic::bookViewListRecordBooks}
            }},
            MenuItem{"Детально про книгу чи залікову книгу", UiLogic::bookViewDetails}
        }},
        MenuItem{"Редагувати", UiLogic::bookEdit},
        MenuItem{"Додати", MyContainer<MenuItem>{
            MenuItem{"Повернутися назад"},
            MenuItem{"Книгу", UiLogic::bookAddBook},
            MenuItem{"Залікову книгу", UiLogic::bookAddRecordBook}
        }},
        MenuItem{"Видалити", UiLogic::bookRemove}
    }}
};

int main() {
    std::cout << "Курсова робота\nВаріант 14\n";
    runMenu(rootMenu, "Головне меню");
    return 0;
}
