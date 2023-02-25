#include <iostream>
#include <random>
#include <vector>
#include <tuple>

std::random_device rd;  // Only used once to initialise (seed) engine
std::mt19937 rng(rd()); // Random-number engine used (Mersenne-Twister in this case)

struct Graph
{
    std::vector<std::vector<int>> edges;
    std::vector<std::vector<int>> non_edges;
    int edge_count;
    Graph(int node_count) : edge_count(0), edges(node_count, std::vector<int>()), non_edges(node_count, std::vector<int>(node_count - 1))
    {
        for (int node = 0; node < node_count; node++)
        {
            int edge = 0;
            for (int connection = 0; connection < node_count - 1; connection++)
            {
                if (edge != node)
                {
                    non_edges[node][connection] = edge;
                }
                else
                {
                    edge++;
                    non_edges[node][connection] = edge;
                }
                edge++;
            }
        }
    }
};

std::tuple<int, int> find_link(std::vector<std::vector<int>> &edges, int possibilities)
{
    std::uniform_int_distribution<int> uni(0, possibilities); // Guaranteed unbiased
    int index_to_delete = uni(rng);
    int node = 0;
    int link_count = 0;
    while (link_count + edges[node].size() < index_to_delete)
    {
        link_count += edges[node].size();
        node++;
    };

    return std::make_tuple(node, edges[node][index_to_delete - link_count]);
}

std::tuple<int, int> choose_random_existing_link(Graph *g)
{
    int possibilities = 2 * g->edge_count;
    return find_link(g->edges, possibilities);
}

std::tuple<int, int> choose_random_non_existing_link(Graph *g)
{
    int n = g->edges.size();
    int possibilities = (n * (n - 1) / 2) - (2 * g->edge_count);
    return find_link(g->non_edges, possibilities);
}

void add_link(std::vector<std::vector<int>> &edges, int from, int to)
{
    edges[from].push_back(to);
    edges[to].push_back(from);
}

void remove_link(std::vector<std::vector<int>> &edges, int from, int to)
{
    edges[from].erase(edges[from].begin() + to);
    edges[to].erase(edges[to].begin() + from);
}

void add_random_link(Graph *g)
{
    auto [from, to] = choose_random_non_existing_link(g);

    add_link(g->edges, from, to);
    remove_link(g->non_edges, from, to);
}

void remove_random_link(Graph *g)
{
    auto [from, to] = choose_random_existing_link(g);

    add_link(g->non_edges, from, to);
    remove_link(g->edges, from, to);
}

void remove_all_node_links(Graph *g, int node)
{
    for (int to : g->edges[node])
    {
        add_link(g->non_edges, node, to);
        g->edges[to].erase(g->edges[to].begin() + node);
    }
    g->edges[node].clear();
}

void remove_all_links_from_random_node(Graph *g)
{
    std::uniform_int_distribution<int> uni(0, g->edges.size()); // Guaranteed unbiased
    int node = uni(rng);
    remove_all_node_links(g, node);
}

int main()
{
    int node_count;
    std::cout << "Input number of nodes" << std::endl;
    std::cin >> node_count;
    Graph *g = new Graph(node_count);

    int simulation_steps;
    std::cout << "Input number of simulation_steps" << std::endl;
    std::cin >> simulation_steps;

    for (int step = 0; step < simulation_steps; step++)
    {
        if (step % alpha_freq == 0)
        {
            add_random_link(g);
        }
        if (step % r_freq == 0)
        {
            remove_all_links_from_random_node(g);
        }
    }
}
