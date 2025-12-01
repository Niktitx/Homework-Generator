#include "crow_all.h"
#include "generator.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static string load_file(const string &path) {
  ifstream in(path, ios::in | ios::binary);
  if (!in)
    return string();
  ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

static string json_escape(const string &s) {
  string out;
  out.reserve(s.size() + 8);
  for (unsigned char c : s) {
    switch (c) {
    case '\"':
      out += "\\\"";
      break;
    case '\\':
      out += "\\\\";
      break;
    case '\b':
      out += "\\b";
      break;
    case '\f':
      out += "\\f";
      break;
    case '\n':
      out += "\\n";
      break;
    case '\r':
      out += "\\r";
      break;
    case '\t':
      out += "\\t";
      break;
    default:
      if (c < 0x20) {
        char buf[7];
        snprintf(buf, sizeof(buf), "\\u%04x", c);
        out += buf;
      } else
        out += (char)c;
    }
  }
  return out;
}

int main() {
  Generator gen;
  if (!gen.loadSchedule("schedule.txt")) {
    cerr << "Warning: schedule.txt not loaded or missing. Continuing.\n";
  }

  crow::SimpleApp app;

  // отдаём статический HTML из папки static
  CROW_ROUTE(app, "/")([&]() {
    string html = load_file("static/index.html");
    if (html.empty()) {
      crow::response r(500, "index.html not found");
      r.set_header("Content-Type", "text/plain; charset=utf-8");
      return r;
    }
    crow::response r(html);
    r.set_header("Content-Type", "text/html; charset=utf-8");
    return r;
  });

  // маршрут генерации
  CROW_ROUTE(app, "/generate")
  ([&](const crow::request &req) {
    int week = 1;
    if (req.url_params.get("week")) {
      try {
        week = stoi(req.url_params.get("week"));
      } catch (...) {
        week = 1;
      }
    }

    gen.CurrentWeek = week;
    gen.calculateTimesPerWeek();

    static const char *Days[] = {"Понедельник", "Вторник", "Среда", "Четверг",
                                 "Пятница"};

    ostringstream out;
    out << "{\"days\":[";

    for (size_t i = 0; i < gen.schedule.size(); ++i) {
      if (i)
        out << ",";
      out << "{";
      out << "\"name\":\"" << json_escape(Days[i]) << "\",";
      out << "\"tasks\":[";

      bool firstT = true;
      for (size_t j = 0; j < gen.schedule[i].size(); ++j) {
        int id = gen.schedule[i][j];
        if (id == -1)
          continue;

        string subj = gen.lessons[id].name;
        string task = gen.generateTask(id);

        if (!firstT)
          out << ",";
        firstT = false;

        out << "{";
        out << "\"subject\":\"" << json_escape(subj) << "\",";
        out << "\"task\":\"" << json_escape(task) << "\"";
        out << "}";
      }

      out << "]";
      out << "}";
    }

    out << "]}";

    crow::response r;
    r.code = 200;
    r.set_header("Content-Type", "application/json; charset=utf-8");
    r.body = out.str();
    return r;
  });

  CROW_ROUTE(app, "/ping")([]() { return "pong"; });

  app.port(8080).multithreaded().run();
  return 0;
}
