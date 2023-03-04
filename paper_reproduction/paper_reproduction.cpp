#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"

int ask_for_int(std::string name)
{
    std::cout << "Input the " + name << std::endl;
    int x;
    std::cin >> x;
    return x;
}

std::pair<int, int> ask_for_pair_of_int(std::string name)
{
    std::cout << "Input " + name + " as numberator and denominator (n d)" << std::endl;
    std::pair<int, int> p;
    std::cin >> p.first >> p.second;
    return p;
}

int gcd(int a, int b)
{
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

void simulate_avg_degree(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string &filename)
{
    std::ofstream results_file(filename);
    std::pair<int, int> min_step = {gcd(alpha.first, r.first), lcm(alpha.second, r.second)};
    int alpha_period = (alpha.first * min_step.second) / (alpha.second * min_step.first);
    int r_period = (r.first * min_step.second) / (r.second * min_step.first);

    for (int step = 0; step < max_simulation_steps; step++)
    {
        float avg_degree = float(2 * g->number_of_edges()) / float(g->number_of_nodes());
        results_file << avg_degree << " \n";
        if (step % alpha_period == 0)
        {
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

void simulate_degree_distribution(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string &filename)
{
    std::ofstream results_file(filename);
    // 1 / gcd
    std::pair<int, int> min_step = {gcd(alpha.second, r.second), lcm(alpha.first, r.first)};
    // (1/alpha)/(1/gcd)
    int alpha_period = (alpha.second * min_step.second) / (alpha.first * min_step.first);
    // (1/r)/(1/gcd)
    int r_period = (r.second * min_step.second) / (r.first * min_step.first);

    std::cout << alpha_period << ' ' << r_period << std::endl;

    std::vector<int> degree_distribution(g->number_of_nodes());
    for (int step = 0; step < max_simulation_steps; step++)
    {
        g->get_degree_distribution(degree_distribution);
        for (int degree : degree_distribution)
        {
            results_file << degree << ' ';
        }
        results_file << "\n";

        if (step % alpha_period == 0)
        {
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

int main()
{
    int number_of_nodes = ask_for_int("number of nodes");
    int max_simulation_steps = ask_for_int("simulation steps");

    std::pair<int, int> alpha = ask_for_pair_of_int("alpha");
    std::pair<int, int> r = ask_for_pair_of_int("r");

    Graph *g = new Graph(number_of_nodes);

    std::string filename = "nodes_" + std::to_string(number_of_nodes) +
                           "_steps_" + std::to_string(max_simulation_steps) +
                           "_alpha_" + std::to_string(alpha.first) + "d" + std::to_string(alpha.second) +
                           "_r_" + std::to_string(r.first) + "d" + std::to_string(r.second) +
                           ".txt";

    simulate_degree_distribution(g, max_simulation_steps, alpha, r, filename);
    // simulate_avg_degree(g, max_simulation_steps, alpha, r, filename);
}
