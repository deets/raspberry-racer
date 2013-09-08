#include "tests/json-helper.hh"


JH jh(Json::Value v) {
  JH res;
  res(v);
  return res;
}

JH jh(const char* key, Json::Value v) {
  JH res = JH();
  res(key, v);
  return res;
}

