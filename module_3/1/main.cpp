#include <vector>
#include <queue>
#include <iostream>
#include <utility>
#include <cassert>
#include <unordered_set>

#include <gtest/gtest.h>
#include <functional>

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
    explicit ListGraph(int vertices_count)
    : out_adjancency_lists_(vertices_count),
      in_adjancency_lists_(vertices_count) {
    }

    explicit ListGraph(IGraph* graph)
    : out_adjancency_lists_(graph->vertices_count()),
      in_adjancency_lists_(graph->vertices_count()) {
        for (unsigned i = 0; i < graph->vertices_count(); ++i) {
            graph->get_prev_vertices(i, in_adjancency_lists_[i]);
            graph->get_next_vertices(i, out_adjancency_lists_[i]);
        }
    }

    void add_edge(int from, int to);

    int vertices_count() const;

    void get_next_vertices(int vertex, vector<int>& vertices) const;
    void get_prev_vertices(int vertex, vector<int>& vertices) const;

  private:
    vector<vector<int>> out_adjancency_lists_;
    vector<vector<int>> in_adjancency_lists_;
};


void ListGraph::add_edge(int from, int to) {
    assert(from >= 0 & from < vertices_count());
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


/*----------------------------------------*/


class MatrixGraph : public IGraph {
  public:
    explicit MatrixGraph(int vertices_count)
    : matrix_(vertices_count) {
        for (int i = 0; i < vertices_count; ++i) {
            matrix_[i].assign(vertices_count, false);
        }
    }

    explicit MatrixGraph(IGraph* graph)
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

    void add_edge(int from, int to) {
        assert(from >= 0 & from < vertices_count());
        assert(to >= 0 && to < vertices_count());
        matrix_[from][to] = true;
    }

    int vertices_count() const {
        return matrix_.size();
    }

    void get_next_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        for (int i = 0; i < matrix_[vertex].size(); ++i) {
            if (matrix_[vertex][i]) {
                vertices.push_back(i);
            }
        }
    }

    void get_prev_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        for (int i = 0; i < matrix_[vertex].size(); ++i) {
            if (matrix_[i][vertex]) {
                vertices.push_back(i);
            }
        }
    }

  private:
    vector<vector<bool>> matrix_;
};


/*----------------------------------------*/


class ArcGraph : public IGraph {
  public:
    explicit ArcGraph(int vertices_count)
    : vertices_count_(vertices_count) {
    }

    explicit ArcGraph(IGraph* graph)
    : vertices_count_(graph->vertices_count()) {
        for (int i = 0; i < graph->vertices_count(); ++i) {
            vector<int> tmp_vector;
            graph->get_next_vertices(i, tmp_vector);
            for (int vertex : tmp_vector) {
                add_edge(i, vertex);
            }
        }
    }

    void add_edge(int from, int to) {
        assert(from >= 0 & from < vertices_count());
        assert(to >= 0 && to < vertices_count());
        for (auto pair : edges_) {
            if (pair.first == from && pair.second == to) {
                return;
            }
        }
        edges_.emplace_back(from, to);
    }

    int vertices_count() const {
        return vertices_count_;
    }

    void get_next_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        for (auto edge : edges_) {
            if (edge.first == vertex) {
                vertices.push_back(edge.second);
            }
        }
    }

    void get_prev_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        for (auto edge : edges_) {
            if (edge.second == vertex) {
                vertices.push_back(edge.first);
            }
        }
    }

  private:
    int vertices_count_;
    vector<pair<int, int>> edges_;
};


/*----------------------------------------*/


class SetGraph : public IGraph {
  public:
    explicit SetGraph(int vertices_count)
    : in_(vertices_count),
      out_(vertices_count) {
    }

    explicit SetGraph(IGraph* graph)
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

    void add_edge(int from, int to) {
        assert(from >= 0 & from < vertices_count());
        assert(to >= 0 && to < vertices_count());
        in_[to].insert(from);
        out_[from].insert(to);
    }

    int vertices_count() const {
        return in_.size();
    }

    void get_next_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        vertices = std::move(vector<int>(out_[vertex].begin(), out_[vertex].end()));
    }

    void get_prev_vertices(int vertex, vector<int>& vertices) const {
        assert(vertex >= 0 && vertex < vertices_count());
        vertices.clear();
        vertices = std::move(vector<int>(in_[vertex].begin(), in_[vertex].end()));
    }

  private:
    vector<unordered_set<int>> in_;
    vector<unordered_set<int>> out_;
};


/*----------------------------------------*/


void BFS(const IGraph* graph, int vertex, std::function<void(int)> visit) {
    vector<bool> isQueued(graph->vertices_count());
    queue<int> greyVertices;
    isQueued[vertex] = true;
    greyVertices.push(vertex);
    while (!greyVertices.empty()) {
        int currentVertex = greyVertices.front();
        visit(currentVertex);
        greyVertices.pop();
        vector<int> outVertices;
        graph->get_next_vertices(currentVertex, outVertices);
        for (int i = 0; i < outVertices.size(); ++i) {
            if (!isQueued[outVertices[i]]) {
                isQueued[outVertices[i]] = true;
                greyVertices.push(outVertices[i]);
            }
        }
    }
}

void IGraphTestAddGetNext(IGraph* graph) {
    graph->add_edge(0, 1);
    graph->add_edge(0, 3);
    graph->add_edge(1, 3);
    vector<int> tmp_vector;

    graph->get_next_vertices(0, tmp_vector);
    unordered_set<int> expected_out{1, 3};
    unordered_set<int> actual_out(tmp_vector.begin(), tmp_vector.end());
    ASSERT_EQ(expected_out, actual_out);
}

void IGraphTestAddGetPrev(IGraph* graph) {
    graph->add_edge(0, 1);
    graph->add_edge(0, 3);
    graph->add_edge(1, 3);
    vector<int> tmp_vector;

    graph->get_prev_vertices(3, tmp_vector);
    unordered_set<int> expected_in{0, 1};
    unordered_set<int> actual_in(tmp_vector.begin(), tmp_vector.end());
    ASSERT_EQ(expected_in, actual_in);
}

void test_visit(int vertex,
                std::unordered_set<int>& first_layer,
                std::unordered_set<int>& second_layer,
                std::unordered_set<int>& third_layer) {
    if (first_layer.size() < 1) {
        first_layer.insert(vertex);
    } else if (second_layer.size() < 3) {
        second_layer.insert(vertex);
    } else {
        third_layer.insert(vertex);
    }
}

void IGraphTestBFS(IGraph* graph) {
    graph->add_edge(1, 2);
    graph->add_edge(1, 3);
    graph->add_edge(1, 4);
    graph->add_edge(2, 5);
    graph->add_edge(3, 6);
    graph->add_edge(3, 7);
    graph->add_edge(3, 8);

    std::unordered_set<int> expected_first{1};
    std::unordered_set<int> expected_second{2, 3, 4};
    std::unordered_set<int> expected_third{5, 6, 7, 8};

    std::unordered_set<int> actual_first;
    std::unordered_set<int> actual_second;
    std::unordered_set<int> actual_third;

    BFS(graph, 1, std::bind(test_visit,
                            std::placeholders::_1,
                            std::ref(actual_first),
                            std::ref(actual_second),
                            std::ref(actual_third)));

    ASSERT_EQ(expected_first, actual_first);
    ASSERT_EQ(expected_second, actual_second);
    ASSERT_EQ(expected_third, actual_third);
}

TEST(ListGraphTest, AddGetNextTest) {
    ListGraph graph(5);
    IGraphTestAddGetNext(&graph);
}

TEST(ListGraphTest, AddGetPrevTest) {
    ListGraph graph(5);
    IGraphTestAddGetPrev(&graph);
}

TEST(ListGraphTest, BFSTest) {
    ListGraph graph(9);
    IGraphTestBFS(&graph);
}

TEST(MatrixGraphTest, AddGetNextTest) {
    MatrixGraph graph(5);
    IGraphTestAddGetNext(&graph);
}

TEST(MatrixGraphTest, AddGetPrevTest) {
    MatrixGraph graph(5);
    IGraphTestAddGetPrev(&graph);
}

TEST(MatrixGraphTest, BFSTest) {
    MatrixGraph graph(9);
    IGraphTestBFS(&graph);
}

TEST(ArcGraphTest, AddGetNextTest) {
    ArcGraph graph(5);
    IGraphTestAddGetNext(&graph);
}

TEST(ArcGraphTest, AddGetPrevTest) {
    ArcGraph graph(5);
    IGraphTestAddGetPrev(&graph);
}

TEST(ArcGraphTest, BFSTest) {
    ArcGraph graph(9);
    IGraphTestBFS(&graph);
}

TEST(SetGraphTest, AddGetNextTest) {
    SetGraph graph(5);
    IGraphTestAddGetNext(&graph);
}

TEST(SetGraphTest, AddGetPrevTest) {
    SetGraph graph(5);
    IGraphTestAddGetPrev(&graph);
}

TEST(SetGraphTest, BFSTest) {
    SetGraph graph(9);
    IGraphTestBFS(&graph);
}

template<typename Graph, typename TestGraph>
void CopyConstructTest() {
    Graph graph(9);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 5);
    graph.add_edge(3, 6);
    graph.add_edge(3, 7);
    graph.add_edge(3, 8);

    TestGraph test_graph(&graph);

    vector<int> expected_tmp;
    vector<int> actual_tmp;
    for (int i = 0; i < graph.vertices_count(); ++i) {
        graph.get_next_vertices(i, expected_tmp);
        test_graph.get_next_vertices(i, actual_tmp);
        ASSERT_EQ(std::unordered_set<int>(expected_tmp.begin(), expected_tmp.end()),
                  std::unordered_set<int>(actual_tmp.begin(), actual_tmp.end())
        ) << "next " << i;

        graph.get_prev_vertices(i, expected_tmp);
        test_graph.get_prev_vertices(i, actual_tmp);
        ASSERT_EQ(expected_tmp, actual_tmp) << "prev " << i;
    }
}


TEST(ListGraph, CopyConstructListTest) {
    CopyConstructTest<ListGraph, ListGraph>();    
}

TEST(ListGraph, CopyConstructMatrixTest) {
    CopyConstructTest<ListGraph, MatrixGraph>();    
}

TEST(ListGraph, CopyConstructArcTest) {
    CopyConstructTest<ListGraph, ArcGraph>();    
}

TEST(ListGraph, CopyConstructSetTest) {
    CopyConstructTest<ListGraph, SetGraph>();    
}


TEST(MatrixGraph, CopyConstructListTest) {
    CopyConstructTest<MatrixGraph, ListGraph>();    
}

TEST(MatrixGraph, CopyConstructMatrixTest) {
    CopyConstructTest<MatrixGraph, MatrixGraph>();    
}

TEST(MatrixGraph, CopyConstructArcTest) {
    CopyConstructTest<MatrixGraph, ArcGraph>();    
}

TEST(MatrixGraph, CopyConstructSetTest) {
    CopyConstructTest<MatrixGraph, SetGraph>();
}


TEST(ArcGraph, CopyConstructListTest) {
    CopyConstructTest<ArcGraph, ListGraph>();    
}

TEST(ArcGraph, CopyConstructMatrixTest) {
    CopyConstructTest<ArcGraph, MatrixGraph>();    
}

TEST(ArcGraph, CopyConstructArcTest) {
    CopyConstructTest<ArcGraph, ArcGraph>();    
}

TEST(ArcGraph, CopyConstructSetTest) {
    CopyConstructTest<ArcGraph, SetGraph>();    
}


TEST(SetGraph, CopyConstructListTest) {
    CopyConstructTest<SetGraph, ListGraph>();    
}

TEST(SetGraph, CopyConstructMatrixTest) {
    CopyConstructTest<SetGraph, MatrixGraph>();    
}

TEST(SetGraph, CopyConstructArcTest) {
    CopyConstructTest<SetGraph, ArcGraph>();    
}

TEST(SetGraph, CopyConstructSetTest) {
    CopyConstructTest<SetGraph, SetGraph>();    
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}