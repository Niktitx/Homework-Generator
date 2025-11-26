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
  int times_per_week = 0;

  Object(string _name, Types _type) : Name(_name), type(_type) {};
};

Object lessons[] = {
    {"Физика", oral},      {"История", oral},     {"Математика", numbers},
    {"Физра", no_task},    {"Биология", oral},    {"Бел яз", exerсise},
    {"Англ яз", exerсise}, {"Бел лит", no_task},  {"Химия", oral},
    {"Русс лит", no_task}, {"Русс яз", exerсise}, {"ДМП", no_task},
    {"Информатика", oral}, {"Астрономия", oral},  {"Общество", oral},
    {"География", oral},   {"", no_task}

};

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
std::mt19937 gen(static_cast<unsigned int>(
    std::chrono::high_resolution_clock::now().time_since_epoch().count()));

string CreateNumbers(int n, int min, int max) {
  string s = "№ ";
  std::uniform_int_distribution<int> dist(min, max);
  for (int i = 0; i < n; i++)
    s += to_string(dist(gen)) + ", ";
  s.pop_back();
  s.pop_back();
  return s;
}

void GenerateTask(int id, int n) {
  if (id == 16)
    return;
  string task;

  std::uniform_int_distribution<int> dist(-1, 1);
  switch (lessons[id].type) {
  case oral:
    task = "$" + to_string(CurrentWeek * min(lessons[id].times_per_week, 2) +
                           dist(gen));
    if (lessons[id].Name == "Химия")
      task += ", " + CreateNumbers(3, 1, 10);
    else if (lessons[id].Name == "Физика") {
      task += ", " + CreateNumbers(3, 1000, 2000);
    }
    break;
  case exerсise:
    dist = uniform_int_distribution<int>(-10, 10);
    if (lessons[id].Name == "Бел яз")
      task = "пр " + to_string(CurrentWeek * 20 + dist(gen));
    else if (lessons[id].Name == "Англ яз")
      task = "SB: p." + to_string(CurrentWeek * 15 + dist(gen)) + " ex." +
             to_string(abs(dist(gen) / 2));
    else
      task = "упр " + to_string(CurrentWeek * 20 + dist(gen));
    break;
  case numbers:
    task = CreateNumbers(3, 1000, 3500);
    break;
  case no_task:
    break;
  }
  cout << n + 1 << ". " << lessons[id].Name << "\t" << task << endl;
}

int main() {
  cout << "Введите номер параграфа по предмету, который 1 раз в неделю: ";
  cin >> CurrentWeek;
  cout << endl;
  calculateTimesPerWeek();
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++)
      GenerateTask(schedule[i][j], j);
    cout << endl;
  }
  return 0;
}
