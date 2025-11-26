#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <string>

using namespace std;

enum Types { oral, exerсise, numbers, no_task };
struct Object {
  string Name;
  Types type;
  int lastTask;
  int times_per_week = 0;

  Object(string _name, Types _type, int _lastTask)
      : Name(_name), type(_type), lastTask(_lastTask) {};
};

Object lessons[] = {{"Физика", oral, 14},
                    {"История", oral, 16},
                    {"Математика", numbers, 2303},
                    {"Физра", no_task, 0},
                    {"Биология", oral, 14},
                    {"Бел яз", exerсise, 100},
                    {"Англ яз", exerсise, 100},
                    {"Бел лит", no_task, 0},
                    {"Химия", oral, 16},
                    {"Русс лит", no_task, 0},
                    {"Русс яз", exerсise, 100},
                    {"ДМП", no_task, 0},
                    {"Информатика", oral, 8},
                    {"Астрономия", oral, 12},
                    {"Общество", oral, 14},
                    {"География", oral, 14},
                    {"", no_task, 0}};

int schedule[5][7] = {{1, 2, 3, 0, 4, 5, 16},
                      {6, 5, 7, 2, 0, 8, 16},
                      {9, 2, 0, 10, 4, 2, 3},
                      {3, 11, 2, 6, 13, 8, 12},
                      {14, 1, 2, 9, 0, 15, 16}};

void calculateTimesPerWeek() {
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 7; j++)
      lessons[schedule[i][j]].times_per_week++;
}

int CurrentWeek;

void GenerateTask(int id, int n) {
  if (id == 16)
    return;
  string task;
  std::mt19937 gen(static_cast<unsigned int>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  std::uniform_int_distribution<int> dist(-1, 1);
  switch (lessons[id].type) {
  case oral:
    task =
        "$" + to_string(CurrentWeek * lessons[id].times_per_week + dist(gen));
    break;
  case exerсise:
    dist = uniform_int_distribution<int>(10, 20);
    task = "упр " + to_string(lessons[id].lastTask + dist(gen));
    break;
  case numbers:
    dist = uniform_int_distribution<int>(1000, 2000);
    task = "№ " + to_string(dist(gen));
    task += ", " + to_string(dist(gen));
    task += ", " + to_string(dist(gen));
    break;
  case no_task:
    break;
  }
  cout << n + 1 << ". " << lessons[id].Name << "\t" << task << endl;
}

int main() {
  cout << "Введите номер параграфа по предмету, который 1 раз в неделю: ";
  cin >> CurrentWeek;
  calculateTimesPerWeek();
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++)
      GenerateTask(schedule[i][j], j);
    cout << endl;
  }
  return 0;
}
