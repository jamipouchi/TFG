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

    std::pair<int, int> find_link(int possibilities)
    {
        int index_to_delete = generate_random_number(possibilities);
        int node = 0;
        int link_count = 0;
        while (link_count + this->edges[node].size() <= index_to_delete)
        {
            link_count += this->edges[node].size();
            node++;
        };

        return {node, *std::next(this->edges[node].begin(), index_to_delete - link_count)};
    }

    std::pair<int, int> find_missing_link(int possibilities)
    {
        int index_to_add = generate_random_number(possibilities);
        int node = 0;
        int link_count = 0;
        while (link_count + this->number_of_nodes() - 1 - this->edges[node].size() <= index_to_add)
        {
            link_count += this->number_of_nodes() - 1 - this->edges[node].size();
            node++;
        };

        int link_index = index_to_add - link_count;

        this->edges[node].insert(node);

        int curr = -1;
        auto next = this->edges[node].begin();
        int empty = 0;
        int next_space = *next;
        while (empty + next_space <= link_index)
        {
            empty += next_space;
            curr = *next;
            next++;
            next_space = next != this->edges[node].end() ? *next - curr - 1 : this->number_of_nodes();
        }

        int to = curr + (link_index + 1 - empty);

        this->edges[node].erase(node);

        return {node, to};
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

    std::pair<int, int> choose_random_non_existing_link()
    {
        int n = this->edges.size();
        int possibilities = 2 * ((n * (n - 1) / 2) - (this->edge_count));
        if (possibilities == 0)
        {
            throw std::runtime_error("There's no missing links in a complete network");
        }
        return this->find_missing_link(possibilities);
    }

    std::pair<int, int> choose_random_existing_link()
    {
        int possibilities = 2 * this->edge_count;
        if (possibilities == 0)
        {
            throw std::runtime_error("There's no links in an empty network");
        }
        return find_link(possibilities);
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
    Graph(int node_count) : edge_count(0), edges(node_count, std::set<unsigned short int>())
    {
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

    void add_random_link()
    {
        auto [from, to] = this->choose_random_non_existing_link();

        this->edge_count += 1;
        add_link(this->edges, from, to);
    }

    void remove_random_link()
    {
        auto [from, to] = this->choose_random_existing_link();

        this->edge_count -= 1;
        remove_link(this->edges, from, to);
    }

    void remove_all_links_from_random_node()
    {
        int node = generate_random_number(this->number_of_nodes());
        this->remove_all_node_links(node);
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
};

#endif