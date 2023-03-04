#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <time.h>

class Graph
{
private:
    int edge_count;
    std::vector<std::unordered_set<int>> edges;
    std::vector<std::unordered_set<int>> non_edges;

    static std::pair<int, int> find_link(std::vector<std::unordered_set<int>> &edges, int possibilities)
    {
        static std::mt19937 rng(time(nullptr));
        std::uniform_int_distribution<int> uni(0, possibilities - 1); // Guaranteed unbiased
        int index_to_delete = uni(rng);
        int node = 0;
        int link_count = 0;
        while (link_count + edges[node].size() <= index_to_delete)
        {
            link_count += edges[node].size();
            node++;
        };

        return {node, *std::next(edges[node].begin(), index_to_delete - link_count)};
    }

    static void add_link(std::vector<std::unordered_set<int>> &edges, int from, int to)
    {
        edges[from].insert(to);
        edges[to].insert(from);
    }

    static void remove_link(std::vector<std::unordered_set<int>> &edges, int from, int to)
    {
        edges[from].erase(to);
        edges[to].erase(from);
    }

    std::pair<int, int> choose_random_non_existing_link()
    {
        int n = this->edges.size();
        int possibilities = 2 * ((n * (n - 1) / 2) - (this->edge_count));
        if (possibilities == 0)
        {
            throw std::runtime_error("There's no missing links in a complete network");
        }
        return find_link(this->non_edges, possibilities);
    }

    std::pair<int, int> choose_random_existing_link()
    {
        int possibilities = 2 * this->edge_count;
        if (possibilities == 0)
        {
            throw std::runtime_error("There's no links in an empty network");
        }
        return find_link(this->edges, possibilities);
    }

    void remove_all_node_links(int node)
    {
        this->edge_count -= this->edges[node].size();
        for (int to : this->edges[node])
        {
            add_link(this->non_edges, node, to);
            this->edges[to].erase(node);
        }
        this->edges[node].clear();
    }

public:
    Graph(int node_count) : edge_count(0), edges(node_count, std::unordered_set<int>()), non_edges(node_count, std::unordered_set<int>(node_count - 1))
    {
        for (int node = 0; node < node_count; node++)
        {
            int edge = 0;
            for (int connection = 0; connection < node_count - 1; connection++)
            {
                if (edge != node)
                {
                    non_edges[node].insert(edge);
                }
                else
                {
                    edge++;
                    non_edges[node].insert(edge);
                }
                edge++;
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

    void add_random_link()
    {
        auto [from, to] = this->choose_random_non_existing_link();

        this->edge_count += 1;
        add_link(this->edges, from, to);
        remove_link(this->non_edges, from, to);
    }

    void remove_random_link()
    {
        auto [from, to] = this->choose_random_existing_link();

        this->edge_count -= 1;
        add_link(this->non_edges, from, to);
        remove_link(this->edges, from, to);
    }

    void remove_all_links_from_random_node()
    {
        static std::mt19937 rng(time(nullptr));
        std::uniform_int_distribution<int> uni(0, this->edges.size() - 1); // Guaranteed unbiased
        int node = uni(rng);
        this->remove_all_node_links(node);
    }

    void get_degree_distribution(std::vector<int> &degree_distribution)
    {
        for (int node = 0; node < this->number_of_nodes(); node++)
        {
            degree_distribution[node] = this->edges[node].size();
        }
    }
};

#endif