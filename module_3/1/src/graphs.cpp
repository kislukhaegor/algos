#include "graphs.hpp"

ListGraph::ListGraph(int vertices_count)
: out_adjancency_lists_(vertices_count),
  in_adjancency_lists_(vertices_count) {
}

ListGraph::ListGraph(IGraph* graph)
: out_adjancency_lists_(graph->vertices_count()),
  in_adjancency_lists_(graph->vertices_count()) {
    for (unsigned i = 0; i < graph->vertices_count(); ++i) {
        graph->get_prev_vertices(i, in_adjancency_lists_[i]);
        graph->get_next_vertices(i, out_adjancency_lists_[i]);
    }
}

void ListGraph::add_edge(int from, int to) {
    assert(from >= 0 && from < vertices_count());
    assert(to >= 0 && to < vertices_count());
    in_adjancency_lists_[to].push_back(from);
    out_adjancency_lists_[from].push_back(to);
}

int ListGraph::vertices_count() const {
    return in_adjancency_lists_.size();
}

void ListGraph::get_next_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    vertices = out_adjancency_lists_[vertex];
}

void ListGraph::get_prev_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    vertices = in_adjancency_lists_[vertex];
}


MatrixGraph::MatrixGraph(int vertices_count)
: matrix_(vertices_count) {
    for (int i = 0; i < vertices_count; ++i) {
        matrix_[i].assign(vertices_count, false);
    }
}

MatrixGraph::MatrixGraph(IGraph* graph) 
: matrix_(graph->vertices_count()) {
    for (int i = 0; i < graph->vertices_count(); ++i) {
        matrix_[i].assign(graph->vertices_count(), false);
        vector<int> tmp_vector;
        graph->get_next_vertices(i, tmp_vector);
        for (int vertex : tmp_vector) {
            matrix_[i][vertex] = true;
        }
    }
}

void MatrixGraph::add_edge(int from, int to) {
    assert(from >= 0 && from < vertices_count());
    assert(to >= 0 && to < vertices_count());
    matrix_[from][to] = true;
}

int MatrixGraph::vertices_count() const {
    return matrix_.size();
}

void MatrixGraph::get_next_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    for (int i = 0; i < matrix_[vertex].size(); ++i) {
        if (matrix_[vertex][i]) {
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::get_prev_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    for (int i = 0; i < matrix_[vertex].size(); ++i) {
        if (matrix_[i][vertex]) {
            vertices.push_back(i);
        }
    }
}


ArcGraph::ArcGraph(int vertices_count)
: vertices_count_(vertices_count) {
}

ArcGraph::ArcGraph(IGraph* graph)
: vertices_count_(graph->vertices_count()) {
    for (int i = 0; i < graph->vertices_count(); ++i) {
        vector<int> tmp_vector;
        graph->get_next_vertices(i, tmp_vector);
        for (int vertex : tmp_vector) {
            add_edge(i, vertex);
        }
    }
}

void ArcGraph::add_edge(int from, int to) {
    assert(from >= 0 & from < vertices_count());
    assert(to >= 0 && to < vertices_count());
    for (auto pair : edges_) {
        if (pair.first == from && pair.second == to) {
            return;
        }
    }
    edges_.emplace_back(from, to);
}

int ArcGraph::vertices_count() const {
    return vertices_count_;
}

void ArcGraph::get_next_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    for (auto edge : edges_) {
        if (edge.first == vertex) {
            vertices.push_back(edge.second);
        }
    }
}

void ArcGraph::get_prev_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    for (auto edge : edges_) {
        if (edge.second == vertex) {
            vertices.push_back(edge.first);
        }
    }
}


SetGraph::SetGraph(int vertices_count)
: in_(vertices_count),
  out_(vertices_count) {
}

SetGraph::SetGraph(IGraph* graph)
 : in_(graph->vertices_count()),
   out_(graph->vertices_count()) {
    for (int i = 0; i < graph->vertices_count(); ++i) {
        vector<int> tmp_vector;
        graph->get_next_vertices(i, tmp_vector);
        for (int vertex : tmp_vector) {
            add_edge(i, vertex);
        }
    }
}

void SetGraph::add_edge(int from, int to) {
    assert(from >= 0 && from < vertices_count());
    assert(to >= 0 && to < vertices_count());
    in_[to].insert(from);
    out_[from].insert(to);
}

int SetGraph::vertices_count() const {
    return in_.size();
}

void SetGraph::get_next_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    vertices = std::move(vector<int>(out_[vertex].begin(), out_[vertex].end()));
}

void SetGraph::get_prev_vertices(int vertex, vector<int>& vertices) const {
    assert(vertex >= 0 && vertex < vertices_count());
    vertices.clear();
    vertices = std::move(vector<int>(in_[vertex].begin(), in_[vertex].end()));
}
