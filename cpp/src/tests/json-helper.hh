#ifndef JSON_HELPER_HH
#define JSON_HELPER_HH
#include <cassert>
#include "json/json.h"

class JH {

public:
  enum State {
    JHUnknown,
    JHObject,
    JHArray
  };

  JH()
    : _state(JHUnknown)
    , _v(0)
  {
  }

  JH(const Json::Value& v)
    : _state(JHUnknown)
    , _v(0)
  {
    (*this)(v);
  }

  JH(const JH& other) {
    _state = other._state;
    _v = 0;
    if(other._v) {
      _v = new Json::Value(*other._v);
    }
  }


  JH(JH& other) {
    _state = other._state;
    _v = 0;
    if(other._v) {
      _v = new Json::Value(*other._v);
    }
  }

  JH&  operator= (const JH & other) {
    assert(false);
    if(this != &other) {
      _state = other._state;
      if(_v) {
	delete _v;
      }
      if(other._v) {
	_v = new Json::Value(*other._v);
      }
    }
    return *this;
  }

  virtual ~JH() {
    if(_v) {
      delete _v;
    }
  }

  JH operator()(const Json::Value& v) {
    assert(_state == JHUnknown || _state == JHArray);
    if(_state == JHUnknown) {
      _v = new Json::Value(Json::arrayValue);
      _state = JHArray;
    }
    _v->append(v);
    return *this;
  }

  JH operator()(const char* key, const Json::Value& v) {
    assert(_state == JHUnknown || _state == JHObject);
    if(_state == JHUnknown) {
      _v = new Json::Value(Json::objectValue);
      _state = JHObject;
    }
    (*_v)[key] = v;
    assert(_v->isMember(key));
    return *this;
  }

  JH operator()(const char* key, JH& v) {
    const Json::Value json_v = v;
    return (*this)(key, json_v);
  }

  operator Json::Value() {
    return Json::Value(*_v);
  }

private:
  State _state;
  Json::Value* _v;
};

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

#endif
