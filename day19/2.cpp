#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#define ull unsigned long long

class Part {
  public:
    // parses a part string and returns a new part
    static Part from(const std::string &part_str) {
        Part               p;
        std::istringstream iss(part_str.substr(1, part_str.size() - 2));

        while (iss) {
            char var_name;
            ull  value;

            iss >> var_name;
            iss.ignore(1); // =
            iss >> value;
            iss.ignore(1); // ,

            p[var_name] = value;
        }

        return p;
    }

    ull      &operator[](char variable_name) { return m_variables[variable_name]; }
    const ull operator[](char variable_name) const {
        return m_variables.find(variable_name)->second;
    }

    ull sum() const {
        return std::accumulate(m_variables.begin(), m_variables.end(), 0,
                               [](const ull prev, const auto &p) { return prev + p.second; });
    }

  private:
    std::map<char, ull> m_variables;
};

// Op is either < or >
struct Condition {
    char        var_token;
    char        op;
    ull         compare_value;
    std::string destination;

    // template specializations for the op tokens
    inline bool operator()(ull variable) const {
        // tried to make it work with non-type template parameters
        // but ultimately failed, this is the next best thing
        if ('>' == op)
            return variable > compare_value;
        if ('<' == op)
            return variable < compare_value;

        std::cerr << "Oopsie, op was '" << op << "' which is not '<' or '>'" << std::endl;
        std::exit(1);
    }

    // convenience operator() so that variables do not have to be filtered outside
    inline bool operator()(const Part &part) const { return this->operator()(part[var_token]); }

    static Condition from(const std::string &condition_str) {
        Condition          c;
        std::istringstream iss(condition_str);
        iss >> c.var_token >> c.op >> c.compare_value;
        iss.ignore(1); // ignore ':'
        iss >> c.destination;

        return c;
    }
};

struct Workflow {
    // the evaluation that is attained when no condition matches
    std::string            name;
    std::string            default_eval; // either A or R
    std::vector<Condition> conditions;

    // runs the workflow on part and returns the next workflow to be ran
    // when returning A or R the procedure is done
    std::vector<std::string> run_on(const Part &part) {
        std::vector<std::string> ret;
        for (const Condition &condition : conditions) {
            if (condition(part))
                ret.push_back(condition.destination);
        }
        return ret;
    }
};

int main() {
    std::ifstream infile("./input");

    // build all workflows
    std::map<std::string, Workflow> workflows;
    std::string                     line;
    while (std::getline(infile, line)) {
        if (line == "")
            break; // start of part list
        // parse the line
        std::istringstream iss(line);
        Workflow           temp;

        std::getline(iss, temp.name, '{');

        std::string cond;
        while (std::getline(iss, cond, ',')) {
            if ('}' == cond.back()) {
                // the last this we have will be "DEFAULT}"
                temp.default_eval = cond.substr(0, cond.length() - 1);
            } else
                temp.conditions.push_back(Condition::from(cond));
        }

        workflows[temp.name] = temp;
    }

    // build all parts
    std::vector<Part> parts;
    while (std::getline(infile, line)) {
        parts.push_back(Part::from(line));
    }

    ull sum = 0;
    for (const Part &part : parts) {
        // we start at the 'in' workflow
        std::string next = "in";
        do {
            next = workflows[next].run_on(part);
        } while (next != "A" && next != "R");

        if (next == "A")
            sum += part.sum();
    }

    infile.close();

    std::cout << "Sum of parts is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
