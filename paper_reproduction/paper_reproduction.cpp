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
    std::cout << "Input " + name + " as numerator and denominator (n d)" << std::endl;
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

void simulate_avg_degree(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string filename)
{
    std::ofstream results_file(filename);
    // 1 / gcd
    std::pair<int, int> min_step = {gcd(alpha.second, r.second), lcm(alpha.first, r.first)};
    results_file << float(min_step.first) / float(min_step.second) << " \n";
    // (1/alpha)/(1/gcd)
    int alpha_period = (alpha.second * min_step.second) / (alpha.first * min_step.first);
    // (1/r)/(1/gcd)
    int r_period = (r.second * min_step.second) / (r.first * min_step.first);

    std::cout << "alpha_period: " << alpha_period << " r_period: " << r_period << std::endl;

    bool some_change = true;
    for (int step = 0; step < max_simulation_steps; step++)
    {
        if (step % 100 == 0)
        {
            std::cout << "step: " << step << std::endl;
        }
        if (some_change)
        {
            float avg_degree = float(2 * g->number_of_edges()) / float(g->number_of_nodes());
            results_file << avg_degree << "\n";
            some_change = false;
        }
        if (step % alpha_period == 0)
        {
            some_change = true;
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            some_change = true;
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

void simulate_degree_distribution(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string filename)
{
    std::ofstream results_file(filename);
    // 1 / gcd
    std::pair<int, int> min_step = {gcd(alpha.second, r.second), lcm(alpha.first, r.first)};
    results_file << float(min_step.first) / float(min_step.second) << " \n";
    // (1/alpha)/(1/gcd)
    int alpha_period = (alpha.second * min_step.second) / (alpha.first * min_step.first);
    // (1/r)/(1/gcd)
    int r_period = (r.second * min_step.second) / (r.first * min_step.first);

    std::cout << "alpha_period: " << alpha_period << " r_period: " << r_period << std::endl;

    std::vector<unsigned short int> degree_distribution(g->number_of_nodes());

    bool some_change = true;
    for (int step = 0; step < max_simulation_steps; step++)
    {
        if (step % 100 == 0)
        {
            std::cout << "step: " << step << std::endl;
        }
        if (some_change)
        {
            g->get_degree_distribution(degree_distribution);
            bool first = true;
            for (int degree : degree_distribution)
            {
                if (!first)
                {
                    results_file << ' ';
                }
                else
                {
                    first = false;
                }
                results_file << degree;
            }
            results_file << "\n";
            some_change = false;
        }

        if (step % alpha_period == 0)
        {
            some_change = true;
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            some_change = true;
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

void simulate_size_of_giant_component(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string filename)
{
    std::ofstream results_file(filename);
    // 1 / gcd
    std::pair<int, int> min_step = {gcd(alpha.second, r.second), lcm(alpha.first, r.first)};
    results_file << float(min_step.first) / float(min_step.second) << " \n";
    // (1/alpha)/(1/gcd)
    int alpha_period = (alpha.second * min_step.second) / (alpha.first * min_step.first);
    // (1/r)/(1/gcd)
    int r_period = (r.second * min_step.second) / (r.first * min_step.first);

    std::cout << "alpha_period: " << alpha_period << " r_period: " << r_period << std::endl;

    bool some_change = true;
    for (int step = 0; step < max_simulation_steps; step++)
    {
        if (step % 100 == 0)
        {
            std::cout << "step: " << step << std::endl;
        }
        if (some_change)
        {
            int size_of_giant_component = g->get_size_of_biggest_component();
            results_file << size_of_giant_component << "\n";
            some_change = false;
        }
        if (step % alpha_period == 0)
        {
            some_change = true;
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            some_change = true;
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

void simulate_sizes_of_components(Graph *g, int max_simulation_steps, std::pair<int, int> alpha, std::pair<int, int> r, std::string filename)
{
    std::ofstream results_file(filename);
    // 1 / gcd
    std::pair<int, int> min_step = {gcd(alpha.second, r.second), lcm(alpha.first, r.first)};
    results_file << float(min_step.first) / float(min_step.second) << " \n";
    // (1/alpha)/(1/gcd)
    int alpha_period = (alpha.second * min_step.second) / (alpha.first * min_step.first);
    // (1/r)/(1/gcd)
    int r_period = (r.second * min_step.second) / (r.first * min_step.first);

    std::cout << "alpha_period: " << alpha_period << " r_period: " << r_period << std::endl;

    bool some_change = true;
    for (int step = 0; step < max_simulation_steps; step++)
    {
        if (step % 100 == 0)
        {
            std::cout << "step: " << step << std::endl;
        }
        if (some_change)
        {
            std::vector<int> sizes_of_components = g->get_sizes_of_components();
            for (int size : sizes_of_components)
            {
                results_file << size << " ";
            }
            results_file << "\n";
            some_change = false;
        }
        if (step % alpha_period == 0)
        {
            some_change = true;
            g->add_random_link();
        }
        if (step % r_period == 0)
        {
            some_change = true;
            g->remove_all_links_from_random_node();
        }
    }
    results_file.close();
}

int ask_for_num_shots()
{
    std::cout << "How many shots do you want to run?" << std::endl;
    int num_shots;
    std::cin >> num_shots;
    return num_shots;
}

enum Simulation
{
    avg_degree,
    degree_distribution,
    giant_component,
    sizes_of_components,
};

Simulation ask_method()
{
    std::cout << "Which simulation do you want to run?" << std::endl;
    std::cout << "  1. Avg degree" << std::endl;
    std::cout << "  2. Degree distribution" << std::endl;
    std::cout << "  3. Giant component" << std::endl;
    std::cout << "  4. Sizes of Components" << std::endl;

    int choice;
    std::cin >> choice;
    switch (choice)
    {
    case 1:
        return avg_degree;
    case 2:
        return degree_distribution;
    case 3:
        return giant_component;
    case 4:
        return sizes_of_components;
    default:
        return ask_method();
    }
}

int main()
{
    int number_of_nodes = ask_for_int("number of nodes");
    int max_simulation_steps = ask_for_int("simulation steps");

    std::pair<int, int> alpha = ask_for_pair_of_int("alpha");
    std::pair<int, int> r = ask_for_pair_of_int("r");

    std::string pre_filename = "nodes_" + std::to_string(number_of_nodes) +
                               "_steps_" + std::to_string(max_simulation_steps) +
                               "_alpha_" + std::to_string(alpha.first) + "d" + std::to_string(alpha.second) +
                               "_r_" + std::to_string(r.first) + "d" + std::to_string(r.second);

    alpha.second *= 2; // theory.
    int num_shots = ask_for_num_shots();

    switch (ask_method())
    {
    case avg_degree:
        pre_filename += "_avg_degree";
        for (int shot = 0; shot < num_shots; shot++)
        {
            std::string filename = pre_filename + "(" + std::to_string(shot) + ").txt";
            Graph *g = new Graph(number_of_nodes);
            simulate_avg_degree(g, max_simulation_steps, alpha, r, filename);
            delete g;
        }
        break;
    case degree_distribution:
        pre_filename += "_degree_distribution";
        for (int shot = 0; shot < num_shots; shot++)
        {
            std::string filename = pre_filename + "(" + std::to_string(shot) + ").txt";
            Graph *g = new Graph(number_of_nodes);
            simulate_degree_distribution(g, max_simulation_steps, alpha, r, filename);
            delete g;
        }
        break;
    case giant_component:
        pre_filename += "_giant_component";
        for (int shot = 0; shot < num_shots; shot++)
        {
            std::string filename = pre_filename + "(" + std::to_string(shot) + ").txt";
            Graph *g = new Graph(number_of_nodes);
            simulate_size_of_giant_component(g, max_simulation_steps, alpha, r, filename);
            delete g;
        }
        break;
    case sizes_of_components:
        pre_filename += "_sizes_of_components";
        for (int shot = 0; shot < num_shots; shot++)
        {
            std::string filename = pre_filename + "(" + std::to_string(shot) + ").txt";
            Graph *g = new Graph(number_of_nodes);
            simulate_sizes_of_components(g, max_simulation_steps, alpha, r, filename);
            delete g;
        }
        break;
    }
}
