#include <chrono>
#include <fstream>
#include <random>
#include <string>
#include <vector>
using namespace std;

enum Types { ORAL, EXERCISE, NUMBERS, NO_TASK };

struct Lesson {
  string name;
  Types type;
  int times_per_week = 0;
};

class Generator {

  mt19937 gen;

  string randNumbers(int n, int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    string s = "№ ";
    for (int i = 0; i < n; i++) {
      s += to_string(dist(gen));
      if (i + 1 != n)
        s += ", ";
    }
    return s;
  }

public:
  vector<vector<int>> schedule;
  int CurrentWeek;
  vector<Lesson> lessons = {
      {"", NO_TASK},         {"Англ яз", EXERCISE},   {"Астрономия", ORAL},
      {"Бел лит", NO_TASK},  {"Бел яз", EXERCISE},    {"Биология", ORAL},
      {"ДМП", NO_TASK},      {"География", ORAL},     {"Информатика", ORAL},
      {"История", ORAL},     {"Математика", NUMBERS}, {"Общество", ORAL},
      {"Русс лит", NO_TASK}, {"Русс яз", EXERCISE},   {"Физика", ORAL},
      {"Физра", NO_TASK},    {"Химия", ORAL}};
  ;

  void calculateTimesPerWeek() {
    for (auto &day : schedule)
      for (int id : day)
        if (id != 0)
          lessons[id].times_per_week++;
  }

  string generateTask(int id) {
    if (id == 0)
      return "";

    const Lesson &L = lessons[id];
    uniform_int_distribution<int> d(-1, 1);

    switch (L.type) {
    case ORAL: {
      string task =
          "§" + to_string(CurrentWeek * min(L.times_per_week, 2) + d(gen));
      if (L.name == "Химия")
        task += ", " + randNumbers(3, 1, 10);
      if (L.name == "Физика")
        task += ", " + randNumbers(3, 1000, 2000);
      return task;
    }
    case EXERCISE: {
      uniform_int_distribution<int> dev(-10, 10);
      int base = CurrentWeek * 20 + dev(gen);

      if (L.name == "Бел яз")
        return "пр " + to_string(base);
      if (L.name == "Англ яз")
        return "SB: p." + to_string(CurrentWeek * 15 + dev(gen)) + " ex." +
               to_string(abs(dev(gen) / 2) + 1);

      return "упр " + to_string(base);
    }
    case NUMBERS:
      return randNumbers(3, 1000, 3500);

    case NO_TASK:
    default:
      return "";
    }
  }

  Generator()
      : gen(chrono::high_resolution_clock::now().time_since_epoch().count()) {}

  bool loadSchedule(const string &filename) {
    ifstream f(filename);
    if (!f.is_open())
      return false;

    schedule.clear();
    string line;
    while (true) {
      vector<int> row;
      int x;
      bool any = false;
      for (int i = 0; i < 7; i++) {
        if (!(f >> x))
          return !schedule.empty();
        if (x != 0)
          any = true;
        row.push_back(x);
      }
      schedule.push_back(row);
    }
  }
};
