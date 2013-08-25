#ifndef WORLD_OBJECT_HH
#define WORLD_OBJECT_HH

#include <string>
#include <boost/intrusive/list.hpp>

#include "events/events.hh"
#include "gfx/openvg-companion.hh"
#include "assets/assets.hh"

using namespace std;
namespace bi = boost::intrusive;


class World;

class WorldObject : public bi::list_base_hook<> {

public:
  typedef bi::list<WorldObject> WorldObjectList;

  virtual ~WorldObject() {}

  virtual void process_input_events(const InputEventVector& events, double elapsed);
  virtual void render(const OpenVGCompanion& vgc) const;

  void add_object(WorldObject *obj);

  friend class World;

private:

  void dispatch_input_events(const InputEventVector& events, double elapsed);
  void dispatch_render(const OpenVGCompanion&);

  WorldObjectList _children;
};



class Image : public WorldObject {

  AssetManager& _am;
  string _asset_name;

public:
  Image(AssetManager& am, string asset_name);
  virtual void render(const OpenVGCompanion& vgc) const;

};


class Translator : public WorldObject {

  float _x, _y;
public:
  Translator(float x, float y);
  virtual void render(const OpenVGCompanion& vgc) const;
};


class LissajouAnimator : public WorldObject {

  float _width, _height, _alpha, _beta, _phase;
public:
  LissajouAnimator(float width, float height, float alpha, float _beta);

  virtual void process_input_events(const InputEventVector& events, double elapsed);
  virtual void render(const OpenVGCompanion& vgc) const;

};


#endif
