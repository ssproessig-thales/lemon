/* -*- mode: C++; indent-tabs-mode: nil; -*-
 *
 * This file is a part of LEMON, a generic C++ optimization library.
 *
 * Copyright (C) 2003-2009
 * Egervary Jeno Kombinatorikus Optimalizalasi Kutatocsoport
 * (Egervary Research Group on Combinatorial Optimization, EGRES).
 *
 * Permission to use, modify and distribute this software is granted
 * provided that this copyright notice appears in all copies. For
 * precise terms see the accompanying LICENSE file.
 *
 * This software is provided "AS IS" with no warranty of any kind,
 * express or implied, and with no claim as to its suitability for any
 * purpose.
 *
 */

#ifndef LEMON_FULL_GRAPH_H
#define LEMON_FULL_GRAPH_H

#include <lemon/core.h>
#include <lemon/bits/graph_extender.h>

///\ingroup graphs
///\file
///\brief FullGraph and FullDigraph classes.

namespace lemon {

  class FullDigraphBase {
  public:

    typedef FullDigraphBase Digraph;

    class Node;
    class Arc;

  protected:

    int _node_num;
    int _arc_num;

    FullDigraphBase() {}

    void construct(int n) { _node_num = n; _arc_num = n * n; }

  public:

    typedef True NodeNumTag;
    typedef True ArcNumTag;

    Node operator()(int ix) const { return Node(ix); }
    int index(const Node& node) const { return node._id; }

    Arc arc(const Node& s, const Node& t) const {
      return Arc(s._id * _node_num + t._id);
    }

    int nodeNum() const { return _node_num; }
    int arcNum() const { return _arc_num; }

    int maxNodeId() const { return _node_num - 1; }
    int maxArcId() const { return _arc_num - 1; }

    Node source(Arc arc) const { return arc._id / _node_num; }
    Node target(Arc arc) const { return arc._id % _node_num; }

    static int id(Node node) { return node._id; }
    static int id(Arc arc) { return arc._id; }

    static Node nodeFromId(int id) { return Node(id);}
    static Arc arcFromId(int id) { return Arc(id);}

    typedef True FindArcTag;

    Arc findArc(Node s, Node t, Arc prev = INVALID) const {
      return prev == INVALID ? arc(s, t) : INVALID;
    }

    class Node {
      friend class FullDigraphBase;

    protected:
      int _id;
      Node(int id) : _id(id) {}
    public:
      Node() {}
      Node (Invalid) : _id(-1) {}
      bool operator==(const Node node) const {return _id == node._id;}
      bool operator!=(const Node node) const {return _id != node._id;}
      bool operator<(const Node node) const {return _id < node._id;}
    };

    class Arc {
      friend class FullDigraphBase;

    protected:
      int _id;  // _node_num * source + target;

      Arc(int id) : _id(id) {}

    public:
      Arc() { }
      Arc (Invalid) { _id = -1; }
      bool operator==(const Arc arc) const {return _id == arc._id;}
      bool operator!=(const Arc arc) const {return _id != arc._id;}
      bool operator<(const Arc arc) const {return _id < arc._id;}
    };

    void first(Node& node) const {
      node._id = _node_num - 1;
    }

    static void next(Node& node) {
      --node._id;
    }

    void first(Arc& arc) const {
      arc._id = _arc_num - 1;
    }

    static void next(Arc& arc) {
      --arc._id;
    }

    void firstOut(Arc& arc, const Node& node) const {
      arc._id = (node._id + 1) * _node_num - 1;
    }

    void nextOut(Arc& arc) const {
      if (arc._id % _node_num == 0) arc._id = 0;
      --arc._id;
    }

    void firstIn(Arc& arc, const Node& node) const {
      arc._id = _arc_num + node._id - _node_num;
    }

    void nextIn(Arc& arc) const {
      arc._id -= _node_num;
      if (arc._id < 0) arc._id = -1;
    }

  };

  typedef DigraphExtender<FullDigraphBase> ExtendedFullDigraphBase;

  /// \ingroup graphs
  ///
  /// \brief A full digraph class.
  ///
  /// This is a simple and fast directed full graph implementation.
  /// From each node go arcs to each node (including the source node),
  /// therefore the number of the arcs in the digraph is the square of
  /// the node number. This digraph type is completely static, so you
  /// can neither add nor delete either arcs or nodes, and it needs
  /// constant space in memory.
  ///
  /// This class fully conforms to the \ref concepts::Digraph
  /// "Digraph concept".
  ///
  /// The \c FullDigraph and \c FullGraph classes are very similar,
  /// but there are two differences. While this class conforms only
  /// to the \ref concepts::Digraph "Digraph" concept, the \c FullGraph
  /// class conforms to the \ref concepts::Graph "Graph" concept,
  /// moreover \c FullGraph does not contain a loop arc for each
  /// node as \c FullDigraph does.
  ///
  /// \sa FullGraph
  class FullDigraph : public ExtendedFullDigraphBase {
    typedef ExtendedFullDigraphBase Parent;

  public:

    /// \brief Constructor
    FullDigraph() { construct(0); }

    /// \brief Constructor
    ///
    /// Constructor.
    /// \param n The number of the nodes.
    FullDigraph(int n) { construct(n); }

    /// \brief Resizes the digraph
    ///
    /// Resizes the digraph. The function will fully destroy and
    /// rebuild the digraph. This cause that the maps of the digraph will
    /// reallocated automatically and the previous values will be lost.
    void resize(int n) {
      Parent::notifier(Arc()).clear();
      Parent::notifier(Node()).clear();
      construct(n);
      Parent::notifier(Node()).build();
      Parent::notifier(Arc()).build();
    }

    /// \brief Returns the node with the given index.
    ///
    /// Returns the node with the given index. Since it is a static
    /// digraph its nodes can be indexed with integers from the range
    /// <tt>[0..nodeNum()-1]</tt>.
    /// \sa index()
    Node operator()(int ix) const { return Parent::operator()(ix); }

    /// \brief Returns the index of the given node.
    ///
    /// Returns the index of the given node. Since it is a static
    /// digraph its nodes can be indexed with integers from the range
    /// <tt>[0..nodeNum()-1]</tt>.
    /// \sa operator()
    int index(const Node& node) const { return Parent::index(node); }

    /// \brief Returns the arc connecting the given nodes.
    ///
    /// Returns the arc connecting the given nodes.
    Arc arc(const Node& u, const Node& v) const {
      return Parent::arc(u, v);
    }

    /// \brief Number of nodes.
    int nodeNum() const { return Parent::nodeNum(); }
    /// \brief Number of arcs.
    int arcNum() const { return Parent::arcNum(); }
  };


  class FullGraphBase {
  public:

    typedef FullGraphBase Graph;

    class Node;
    class Arc;
    class Edge;

  protected:

    int _node_num;
    int _edge_num;

    FullGraphBase() {}

    void construct(int n) { _node_num = n; _edge_num = n * (n - 1) / 2; }

    int _uid(int e) const {
      int u = e / _node_num;
      int v = e % _node_num;
      return u < v ? u : _node_num - 2 - u;
    }

    int _vid(int e) const {
      int u = e / _node_num;
      int v = e % _node_num;
      return u < v ? v : _node_num - 1 - v;
    }

    void _uvid(int e, int& u, int& v) const {
      u = e / _node_num;
      v = e % _node_num;
      if  (u >= v) {
        u = _node_num - 2 - u;
        v = _node_num - 1 - v;
      }
    }

    void _stid(int a, int& s, int& t) const {
      if ((a & 1) == 1) {
        _uvid(a >> 1, s, t);
      } else {
        _uvid(a >> 1, t, s);
      }
    }

    int _eid(int u, int v) const {
      if (u < (_node_num - 1) / 2) {
        return u * _node_num + v;
      } else {
        return (_node_num - 1 - u) * _node_num - v - 1;
      }
    }

  public:

    Node operator()(int ix) const { return Node(ix); }
    int index(const Node& node) const { return node._id; }

    Edge edge(const Node& u, const Node& v) const {
      if (u._id < v._id) {
        return Edge(_eid(u._id, v._id));
      } else if (u._id != v._id) {
        return Edge(_eid(v._id, u._id));
      } else {
        return INVALID;
      }
    }

    Arc arc(const Node& s, const Node& t) const {
      if (s._id < t._id) {
        return Arc((_eid(s._id, t._id) << 1) | 1);
      } else if (s._id != t._id) {
        return Arc(_eid(t._id, s._id) << 1);
      } else {
        return INVALID;
      }
    }

    typedef True NodeNumTag;
    typedef True ArcNumTag;
    typedef True EdgeNumTag;

    int nodeNum() const { return _node_num; }
    int arcNum() const { return 2 * _edge_num; }
    int edgeNum() const { return _edge_num; }

    static int id(Node node) { return node._id; }
    static int id(Arc arc) { return arc._id; }
    static int id(Edge edge) { return edge._id; }

    int maxNodeId() const { return _node_num-1; }
    int maxArcId() const { return 2 * _edge_num-1; }
    int maxEdgeId() const { return _edge_num-1; }

    static Node nodeFromId(int id) { return Node(id);}
    static Arc arcFromId(int id) { return Arc(id);}
    static Edge edgeFromId(int id) { return Edge(id);}

    Node u(Edge edge) const {
      return Node(_uid(edge._id));
    }

    Node v(Edge edge) const {
      return Node(_vid(edge._id));
    }

    Node source(Arc arc) const {
      return Node((arc._id & 1) == 1 ?
                  _uid(arc._id >> 1) : _vid(arc._id >> 1));
    }

    Node target(Arc arc) const {
      return Node((arc._id & 1) == 1 ?
                  _vid(arc._id >> 1) : _uid(arc._id >> 1));
    }

    typedef True FindEdgeTag;
    typedef True FindArcTag;

    Edge findEdge(Node u, Node v, Edge prev = INVALID) const {
      return prev != INVALID ? INVALID : edge(u, v);
    }

    Arc findArc(Node s, Node t, Arc prev = INVALID) const {
      return prev != INVALID ? INVALID : arc(s, t);
    }

    class Node {
      friend class FullGraphBase;

    protected:
      int _id;
      Node(int id) : _id(id) {}
    public:
      Node() {}
      Node (Invalid) { _id = -1; }
      bool operator==(const Node node) const {return _id == node._id;}
      bool operator!=(const Node node) const {return _id != node._id;}
      bool operator<(const Node node) const {return _id < node._id;}
    };

    class Edge {
      friend class FullGraphBase;
      friend class Arc;

    protected:
      int _id;

      Edge(int id) : _id(id) {}

    public:
      Edge() { }
      Edge (Invalid) { _id = -1; }

      bool operator==(const Edge edge) const {return _id == edge._id;}
      bool operator!=(const Edge edge) const {return _id != edge._id;}
      bool operator<(const Edge edge) const {return _id < edge._id;}
    };

    class Arc {
      friend class FullGraphBase;

    protected:
      int _id;

      Arc(int id) : _id(id) {}

    public:
      Arc() { }
      Arc (Invalid) { _id = -1; }

      operator Edge() const { return Edge(_id != -1 ? (_id >> 1) : -1); }

      bool operator==(const Arc arc) const {return _id == arc._id;}
      bool operator!=(const Arc arc) const {return _id != arc._id;}
      bool operator<(const Arc arc) const {return _id < arc._id;}
    };

    static bool direction(Arc arc) {
      return (arc._id & 1) == 1;
    }

    static Arc direct(Edge edge, bool dir) {
      return Arc((edge._id << 1) | (dir ? 1 : 0));
    }

    void first(Node& node) const {
      node._id = _node_num - 1;
    }

    static void next(Node& node) {
      --node._id;
    }

    void first(Arc& arc) const {
      arc._id = (_edge_num << 1) - 1;
    }

    static void next(Arc& arc) {
      --arc._id;
    }

    void first(Edge& edge) const {
      edge._id = _edge_num - 1;
    }

    static void next(Edge& edge) {
      --edge._id;
    }

    void firstOut(Arc& arc, const Node& node) const {
      int s = node._id, t = _node_num - 1;
      if (s < t) {
        arc._id = (_eid(s, t) << 1) | 1;
      } else {
        --t;
        arc._id = (t != -1 ? (_eid(t, s) << 1) : -1);
      }
    }

    void nextOut(Arc& arc) const {
      int s, t;
      _stid(arc._id, s, t);
      --t;
      if (s < t) {
        arc._id = (_eid(s, t) << 1) | 1;
      } else {
        if (s == t) --t;
        arc._id = (t != -1 ? (_eid(t, s) << 1) : -1);
      }
    }

    void firstIn(Arc& arc, const Node& node) const {
      int s = _node_num - 1, t = node._id;
      if (s > t) {
        arc._id = (_eid(t, s) << 1);
      } else {
        --s;
        arc._id = (s != -1 ? (_eid(s, t) << 1) | 1 : -1);
      }
    }

    void nextIn(Arc& arc) const {
      int s, t;
      _stid(arc._id, s, t);
      --s;
      if (s > t) {
        arc._id = (_eid(t, s) << 1);
      } else {
        if (s == t) --s;
        arc._id = (s != -1 ? (_eid(s, t) << 1) | 1 : -1);
      }
    }

    void firstInc(Edge& edge, bool& dir, const Node& node) const {
      int u = node._id, v = _node_num - 1;
      if (u < v) {
        edge._id = _eid(u, v);
        dir = true;
      } else {
        --v;
        edge._id = (v != -1 ? _eid(v, u) : -1);
        dir = false;
      }
    }

    void nextInc(Edge& edge, bool& dir) const {
      int u, v;
      if (dir) {
        _uvid(edge._id, u, v);
        --v;
        if (u < v) {
          edge._id = _eid(u, v);
        } else {
          --v;
          edge._id = (v != -1 ? _eid(v, u) : -1);
          dir = false;
        }
      } else {
        _uvid(edge._id, v, u);
        --v;
        edge._id = (v != -1 ? _eid(v, u) : -1);
      }
    }

  };

  typedef GraphExtender<FullGraphBase> ExtendedFullGraphBase;

  /// \ingroup graphs
  ///
  /// \brief An undirected full graph class.
  ///
  /// This is a simple and fast undirected full graph
  /// implementation. From each node go edge to each other node,
  /// therefore the number of edges in the graph is \f$n(n-1)/2\f$.
  /// This graph type is completely static, so you can neither
  /// add nor delete either edges or nodes, and it needs constant
  /// space in memory.
  ///
  /// This class fully conforms to the \ref concepts::Graph "Graph concept".
  ///
  /// The \c FullGraph and \c FullDigraph classes are very similar,
  /// but there are two differences. While the \c FullDigraph class
  /// conforms only to the \ref concepts::Digraph "Digraph" concept,
  /// this class conforms to the \ref concepts::Graph "Graph" concept,
  /// moreover \c FullGraph does not contain a loop arc for each
  /// node as \c FullDigraph does.
  ///
  /// \sa FullDigraph
  class FullGraph : public ExtendedFullGraphBase {
    typedef ExtendedFullGraphBase Parent;

  public:

    /// \brief Constructor
    FullGraph() { construct(0); }

    /// \brief Constructor
    ///
    /// Constructor.
    /// \param n The number of the nodes.
    FullGraph(int n) { construct(n); }

    /// \brief Resizes the graph
    ///
    /// Resizes the graph. The function will fully destroy and
    /// rebuild the graph. This cause that the maps of the graph will
    /// reallocated automatically and the previous values will be lost.
    void resize(int n) {
      Parent::notifier(Arc()).clear();
      Parent::notifier(Edge()).clear();
      Parent::notifier(Node()).clear();
      construct(n);
      Parent::notifier(Node()).build();
      Parent::notifier(Edge()).build();
      Parent::notifier(Arc()).build();
    }

    /// \brief Returns the node with the given index.
    ///
    /// Returns the node with the given index. Since it is a static
    /// graph its nodes can be indexed with integers from the range
    /// <tt>[0..nodeNum()-1]</tt>.
    /// \sa index()
    Node operator()(int ix) const { return Parent::operator()(ix); }

    /// \brief Returns the index of the given node.
    ///
    /// Returns the index of the given node. Since it is a static
    /// graph its nodes can be indexed with integers from the range
    /// <tt>[0..nodeNum()-1]</tt>.
    /// \sa operator()
    int index(const Node& node) const { return Parent::index(node); }

    /// \brief Returns the arc connecting the given nodes.
    ///
    /// Returns the arc connecting the given nodes.
    Arc arc(const Node& s, const Node& t) const {
      return Parent::arc(s, t);
    }

    /// \brief Returns the edge connects the given nodes.
    ///
    /// Returns the edge connects the given nodes.
    Edge edge(const Node& u, const Node& v) const {
      return Parent::edge(u, v);
    }

    /// \brief Number of nodes.
    int nodeNum() const { return Parent::nodeNum(); }
    /// \brief Number of arcs.
    int arcNum() const { return Parent::arcNum(); }
    /// \brief Number of edges.
    int edgeNum() const { return Parent::edgeNum(); }

  };


} //namespace lemon


#endif //LEMON_FULL_GRAPH_H
