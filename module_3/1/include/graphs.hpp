#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <utility>
#include <cassert>
#include <unordered_set>

using std::vector;
using std::queue;
using std::pair;
using std::unordered_set;

struct IGraph {
  public: 
    virtual ~IGraph() = default;

    virtual void add_edge(int from, int to) = 0;

    virtual int vertices_count() const = 0;

    virtual void get_next_vertices(int vertex, vector<int>& vertices) const = 0;
    
    virtual void get_prev_vertices(int vertex, vector<int>& vertices) const = 0;
};


// ListGraph

class ListGraph : public IGraph {
  public:
    explicit ListGraph(int vertices_count);

    explicit ListGraph(IGraph* graph);

    void add_edge(int from, int to);

    int vertices_count() const;

    void get_next_vertices(int vertex, vector<int>& vertices) const;
    
    void get_prev_vertices(int vertex, vector<int>& vertices) const;

  private:
    vector<vector<int>> out_adjancency_lists_;
    vector<vector<int>> in_adjancency_lists_;
};

/*----------------------------------------*/


class MatrixGraph : public IGraph {
  public:

    explicit MatrixGraph(int vertices_count);

    explicit MatrixGraph(IGraph* graph);

    void add_edge(int from, int to);

    int vertices_count() const;

    void get_next_vertices(int vertex, vector<int>& vertices) const;

    void get_prev_vertices(int vertex, vector<int>& vertices) const;

  private:
    vector<vector<bool>> matrix_;
};


/*----------------------------------------*/


class ArcGraph : public IGraph {
  public:
    explicit ArcGraph(int vertices_count);

    explicit ArcGraph(IGraph* graph);

    void add_edge(int from, int to);

    int vertices_count() const;

    void get_next_vertices(int vertex, vector<int>& vertices) const;

    void get_prev_vertices(int vertex, vector<int>& vertices) const;

  private:
    int vertices_count_;
    vector<pair<int, int>> edges_;
};


/*----------------------------------------*/


class SetGraph : public IGraph {
  public:
    explicit SetGraph(int vertices_count);

    explicit SetGraph(IGraph* graph);

    void add_edge(int from, int to);

    int vertices_count() const;

    void get_next_vertices(int vertex, vector<int>& vertices) const;

    void get_prev_vertices(int vertex, vector<int>& vertices) const;

  private:
    vector<unordered_set<int>> in_;
    vector<unordered_set<int>> out_;
};