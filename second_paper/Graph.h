#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <queue>
#include <time.h>

class Graph
{
private:
    int edge_count;
    std::vector<std::set<unsigned short int>> edges;

    static int generate_random_number(int last_not_included)
    {
        static std::mt19937 rng(time(nullptr));
        std::uniform_int_distribution<int> uni(0, last_not_included - 1); // Guaranteed unbiased
        return uni(rng);
    }

    static void add_link(std::vector<std::set<unsigned short int>> &edges, int from, int to)
    {
        edges[from].insert(to);
        edges[to].insert(from);
    }

    static void remove_link(std::vector<std::set<unsigned short int>> &edges, int from, int to)
    {
        edges[from].erase(to);
        edges[to].erase(from);
    }

    void remove_all_node_links(int node)
    {
        this->edge_count -= this->edges[node].size();
        std::vector<int> to_remove;
        for (int to : this->edges[node])
        {
            to_remove.push_back(to);
        }
        for (int to : to_remove)
        {
            remove_link(this->edges, node, to);
        }
    }

    static int find_corresponding_node(std::vector<std::set<unsigned short int>> &edges, int edge_num)
    {
        int node = 0;
        while (edge_num >= edges[node].size())
        {
            edge_num -= int(edges[node].size());
            node++;
        }
        return node;
    }

    static int bfs(std::vector<std::set<unsigned short int>> &edges, int start_node, std::vector<bool> &visited)
    {
        int component_size = 0;

        std::queue<int> marked_to_visit;
        visited[start_node] = true;
        component_size++;
        marked_to_visit.push(start_node);

        while (!marked_to_visit.empty())
        {
            int node_visited = marked_to_visit.front();
            marked_to_visit.pop();
            for (int adjacent : edges[node_visited])
            {
                if (!visited[adjacent])
                {
                    visited[adjacent] = true;
                    component_size++;
                    marked_to_visit.push(adjacent);
                }
            }
        }
        return component_size;
    }

public:
    Graph(int node_count, bool clique) : edge_count(0), edges(node_count, std::set<unsigned short int>())
    {
        if (clique)
        {
            for (int i = 0; i < node_count; i++)
            {
                for (int j = i + 1; j < node_count; j++)
                {
                    add_link(this->edges, i, j);
                    this->edge_count++;
                }
            }
        }
    }

    int number_of_edges()
    {
        return this->edge_count;
    }

    int number_of_nodes()
    {
        return this->edges.size();
    }

    void print()
    {
        for (auto node : this->edges)
        {
            for (auto edge : node)
            {
            }
        }
    }

    void add_vertex_uniformly_with_given_degree(int degree)
    {
        this->edges.push_back(std::set<unsigned short int>());
        this->edge_count += degree;
        std::set<int> used;
        for (int i = 0; i < degree; i++)
        {
            int to = generate_random_number(this->number_of_nodes() - 1);
            while (used.find(to) != used.end())
            {
                to = generate_random_number(this->number_of_nodes() - 1);
            }
            add_link(this->edges, this->number_of_nodes() - 1, to);
        }
    }

    void add_vertex_preferentially_with_given_degree(int degree)
    {
        // if num_nodes_amb edge < degree no podem... mirar com fer aixo. suposo que el millor es fer un try catch?? Think about that
        if (2*this->number_of_edges() < degree) {
            std::cout << "Error: degree is too high, adding uniformly..." << std::endl;
            this->add_vertex_uniformly_with_given_degree(degree);
        }
        this->edges.push_back(std::set<unsigned short int>());
        std::set<int> used;
        used.insert(this->number_of_nodes() - 1);
        for (int i = 0; i < degree; i++)
        {
            // We are creating a random number from [0, 2 * number_of_edges]. Each node has a probability of being chosen equal to its degree.
            int edge_num = generate_random_number(2 * this->number_of_edges());
            int to = find_corresponding_node(this->edges, edge_num);
            while (used.find(to) != used.end())
            {
                edge_num = generate_random_number(2 * this->number_of_edges());
                to = find_corresponding_node(this->edges, edge_num);
            }
            add_link(this->edges, this->number_of_nodes() - 1, to);
            this->edge_count++;
            used.insert(to);
        }
    }

    void delete_random_vertex()
    {
        int node = generate_random_number(this->number_of_nodes());
        this->remove_all_node_links(node);
        this->edges.erase(this->edges.begin() + node);
    }

    void get_degree_distribution(std::vector<unsigned short int> &degree_distribution)
    {
        for (int node = 0; node < this->number_of_nodes(); node++)
        {
            degree_distribution[node] = this->edges[node].size();
        }
    }

    int get_size_of_biggest_component()
    {
        int size_of_biggest_component = 0;
        std::vector<bool> visited(this->number_of_nodes(), false);
        for (int node = 0; node < this->number_of_nodes(); node++)
        {
            if (!visited[node])
            {
                int node_component_size = bfs(this->edges, node, visited);
                if (node_component_size > size_of_biggest_component)
                {
                    size_of_biggest_component = node_component_size;
                }
            }
        }
        return size_of_biggest_component;
    }

    std::vector<int> get_sizes_of_components()
    {
        std::vector<int> sizes_of_components;
        std::vector<bool> visited(this->number_of_nodes(), false);
        for (int node = 0; node < this->number_of_nodes(); node++)
        {
            if (!visited[node])
            {
                sizes_of_components.push_back(bfs(this->edges, node, visited));
            }
        }
        return sizes_of_components;
    }
};

#endif