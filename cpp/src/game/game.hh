#pragma once
#include <Box2D/Box2D.h>
#include <boost/intrusive/list.hpp>
#include <boost/function.hpp>

#include "events/events.hh"
#include "scene/scene-graph.hh"

namespace bi = boost::intrusive;

namespace rracer {

  #define WORLD_VELOCITY_ITERATIONS 6
  #define WORLD_POSITION_ITERATIONS 2


  class GameObject : public bi::list_base_hook<>  {
  public:
    typedef bi::list<GameObject> GameObjectList;
    typedef GameObjectList::iterator iterator;

    GameObject();
    virtual ~GameObject();

    virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    virtual void setup_within_world(b2World *);


    iterator begin();
    iterator end();

    GameObject* parent() const;
    void parent(GameObject* parent);
    void add_object(GameObject *obj);

    friend class Game;

  protected:

    void dispatch_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);

    void object_added(GameObject* parent, GameObject* child);
    virtual void on_object_added(GameObject* parent, GameObject* child);

    GameObjectList _children;

  private:
    GameObject* _parent;
  };


  //==========================================================

  class GameRoot : public GameObject {
  public:
    GameRoot(boost::function<void (GameObject*, GameObject*)> on_object_added_callback);
    virtual void on_object_added(GameObject*, GameObject*);

  private:
    boost::function<void (GameObject*, GameObject*)> _on_object_added_callback;
  };


  //==========================================================

  class Game {

    void on_object_added(GameObject* parent, GameObject* child);

  public:

    class iterator : public std::iterator<std::forward_iterator_tag, GameObject> {

    public:
      typedef GameObject::GameObjectList::iterator go_iterator;
      iterator(go_iterator, go_iterator);
      iterator(const iterator&);
      virtual ~iterator();

      iterator& operator=(const iterator&);
      bool operator==(const iterator&) const;
      bool operator!=(const iterator&) const;

      iterator& operator++();
      reference operator*() const;
      pointer operator->() const;

    private:
      stack<pair<go_iterator, go_iterator> > _iterators;
    };

    /**
     *
     * The scene_graph can be NULL!
     */
    Game(SceneGraph* scene_graph);

    virtual ~Game();

    b2World* world() const;

    bool has_ended() { return _has_ended; };

    void step(const GameEventVector &events, Real elapsed);

    void add_object(GameObject *obj);

    iterator begin();
    iterator end();

  protected:

    SceneGraph* _scene_graph;

  private:


    shared_ptr<GameRoot> _root;

    b2World* _world;

    GameEventVector _next_frame_events;

    TimeInfo _time_info;

    bool _has_ended;

  };


} // end ns::rracer


