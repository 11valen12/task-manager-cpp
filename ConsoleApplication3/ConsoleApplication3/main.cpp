#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
#include <locale>
#include <cstdlib> // Для использования функции system

using namespace std;

class Task {
public:
    Task(const string& description, bool completed, const string& deadline)
        : description(description), completed(completed), deadline(deadline) {}

    string getDescription() const {
        return description;
    }

    bool isCompleted() const {
        return completed;
    }

    string getDeadline() const {
        return deadline;
    }

    void setDescription(const string& desc) {
        description = desc;
    }

    void setCompleted(bool value) {
        completed = value;
    }

    void setDeadline(const string& dl) {
        deadline = dl;
    }

private:
    string description;
    bool completed;
    string deadline;
};

class TaskManager {
public:
    void addTask(const Task& task) {
        tasks.push_back(task);
        recordDateTime();
    }

    void editTask(int index, const Task& task) {
        tasks[index] = task;
        recordDateTime();
    }

    void deleteTask(int index) {
        tasks.erase(tasks.begin() + index);
        recordDateTime();
    }

    void displayTasks() const {
        cout << "Задачи:\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". ";
            cout << "Описание: " << tasks[i].getDescription() << ", ";
            cout << "Статус: " << (tasks[i].isCompleted() ? "Выполнена" : "Не выполнена") << ", ";
            cout << "Срок выполнения: " << tasks[i].getDeadline() << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& task : tasks) {
                file << task.getDescription() << "," << task.isCompleted() << "," << task.getDeadline() << endl;
            }
            file.close();
        }
        else {
            cerr << "Ошибка: Не удалось открыть файл." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            tasks.clear();
            string line;
            while (getline(file, line)) {
                string description, deadline;
                bool completed;
                size_t pos = line.find(',');
                description = line.substr(0, pos);
                line.erase(0, pos + 1);
                pos = line.find(',');
                completed = stoi(line.substr(0, pos));
                line.erase(0, pos + 1);
                deadline = line;
                tasks.emplace_back(description, completed, deadline);
            }
            file.close();
        }
        else {
            cerr << "Ошибка: Не удалось открыть файл." << endl;
        }
    }

    vector<Task>& getTasks() {
        return tasks;
    }

private:
    vector<Task> tasks;

    void recordDateTime() {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now); // Используем безопасную функцию ctime_s
        cout << "Дата и время изменения: " << dt;
    }
};

bool isValidTimeInput(const string& input) {
    for (char c : input) {
        if (!isdigit(c) && c != ':' && c != ' ' && c != 'a' && c != 'd' && c != 'y') {
            return false;
        }
    }
    return true;
}

void clearConsole() {
#ifdef _WIN32 // Проверяем операционную систему (Windows)
    system("cls"); // Очищаем консоль
#else // Если не Windows, предполагаем, что это UNIX-подобная ОС (Linux, macOS)
    system("clear"); // Очищаем консоль
#endif
}

int main() {
    setlocale(LC_ALL, "Russian"); // Устанавливаем русскую локаль для корректного отображения русских символов в консоли
    TaskManager manager;

    // Загрузка данных из файла при запуске программы
    manager.loadFromFile("tasks.txt");

    int choice;
    do {
        cout << "\nСистема управления задачами\n";
        cout << "1. Добавить задачу\n";
        cout << "2. Редактировать задачу\n";
        cout << "3. Удалить задачу\n";
        cout << "4. Показать задачи\n";
        cout << "5. Сохранить задачи в файл\n";
        cout << "6. Выйти\n";
        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            clearConsole(); // Очищаем консоль перед добавлением задачи
            string description, deadline;
            bool completed;
            cout << "Введите описание задачи: ";
            cin.ignore();
            getline(cin, description);
            cout << "Задача выполнена? (1 для Да, 0 для Нет): ";
            cin >> completed;
            cout << "Введите срок выполнения задачи (например, '12:30' для времени или '15 day' для дней): ";
            cin.ignore();
            getline(cin, deadline);
            while (!isValidTimeInput(deadline)) {
                cout << "Некорректный формат времени. Пожалуйста, введите время в формате 'часы:минуты' или 'дней':" << endl;
                getline(cin, deadline);
            }
            manager.addTask(Task(description, completed, deadline));
            break;
        }
        case 2: {
            clearConsole(); // Очищаем консоль перед редактированием задачи
            int index;
            cout << "Введите индекс задачи для редактирования: ";
            cin >> index;
            if (index >= 1 && index <= manager.getTasks().size()) {
                int editChoice;
                cout << "Что вы хотите отредактировать?\n";
                cout << "1. Описание задачи\n";
                cout << "2. Статус выполнения задачи\n";
                cout << "3. Срок выполнения задачи\n";
                cout << "Введите ваш выбор: ";
                cin >> editChoice;

                switch (editChoice) {
                case 1: {
                    string description;
                    cout << "Введите новое описание задачи: ";
                    cin.ignore();
                    getline(cin, description);
                    manager.getTasks()[index - 1].setDescription(description);
                    break;
                }
                case 2: {
                    bool completed;
                    cout << "Задача выполнена? (1 для Да, 0 для Нет): ";
                    cin >> completed;
                    manager.getTasks()[index - 1].setCompleted(completed);
                    break;
                }
                case 3: {
                    string deadline;
                    cout << "Введите новый срок выполнения задачи (например, '12:30' для времени или '15 day' для дней): ";
                    cin.ignore();
                    getline(cin, deadline);
                    while (!isValidTimeInput(deadline)) {
                        cout << "Некорректный формат времени. Пожалуйста, введите время в формате 'часы:минуты' или 'дней':" << endl;
                        getline(cin, deadline);
                    }
                    manager.getTasks()[index - 1].setDeadline(deadline);
                    break;
                }
                default:
                    cout << "Неверный выбор. Пожалуйста, введите число от 1 до 3." << endl;
                }
            }
            else {
                cout << "Неверный индекс!" << endl;
            }
            break;
        }
        case 3: {
            clearConsole(); // Очищаем консоль перед удалением задачи
            int index;
            cout << "Введите индекс задачи для удаления: ";
            cin >> index;
            if (index >= 1 && index <= manager.getTasks().size()) {
                manager.deleteTask(index - 1);
            }
            else {
                cout << "Неверный индекс!" << endl;
            }
            break;
        }
        case 4:
            clearConsole(); // Очищаем консоль перед показом задач
            manager.displayTasks();
            break;
        case 5:
            clearConsole(); // Очищаем консоль перед сохранением в файл
            manager.saveToFile("tasks.txt");
            break;
        case 6:
            cout << "Выход из программы...\n";
            break;
        default:
            cout << "Неверный выбор. Пожалуйста, введите число от 1 до 6." << endl;
        }
    } while (choice != 6);

    return 0;
}
