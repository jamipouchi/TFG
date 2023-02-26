#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <tuple>

std::random_device rd;  // Only used once to initialise (seed) engine
std::mt19937 rng(rd()); // Random-number engine used (Mersenne-Twister in this case)

// TODO: MOVE TO PRIVATE EDGES AND NON_EDGES

class Graph
{
private:
    int edge_count;

    static std::tuple<int, int> find_link(std::vector<std::unordered_set<int>> &edges, int possibilities)
    {
        std::uniform_int_distribution<int> uni(0, possibilities - 1); // Guaranteed unbiased
        int index_to_delete = uni(rng);
        int node = 0;
        int link_count = 0;
        while (link_count + edges[node].size() <= index_to_delete)
        {
            link_count += edges[node].size();
            node++;
        };

        return std::make_tuple(node, *std::next(edges[node].begin(), index_to_delete - link_count)); // edges[node][index_to_delete - link_count]);
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

    std::tuple<int, int> choose_random_non_existing_link()
    {
        int n = this->edges.size();
        int possibilities = 2 * ((n * (n - 1) / 2) - (this->edge_count));
        if (possibilities == 0)
        {
            throw std::runtime_error("There's no missing links in a complete network");
        }
        return find_link(this->non_edges, possibilities);
    }

    std::tuple<int, int> choose_random_existing_link()
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
    std::vector<std::unordered_set<int>> edges;
    std::vector<std::unordered_set<int>> non_edges;
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
        std::uniform_int_distribution<int> uni(0, this->edges.size() - 1); // Guaranteed unbiased
        int node = uni(rng);
        this->remove_all_node_links(node);
    }
};

int main()
{
    int node_count;
    std::cin >> node_count;
    Graph *g = new Graph(node_count);

    /**
    int simulation_steps;
    std::cin >> simulation_steps;
    for (int step = 0; step < simulation_steps; step++)
    {
        if (step % alpha_period == 0)
        {
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            g->remove_all_links_from_random_node();
        }
    }
    **/
}
