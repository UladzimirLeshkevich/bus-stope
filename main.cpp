#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const std::string GROTTY_COMPANY = "Grotty";
const std::string POSH_COMPANY   = "Posh";

struct time
{
    int hours;
    int minutes;
};

struct route
{
    std::string company;
    time        departure;
    time        arrival;
};

bool operator==(const time& t1, const time& t2)
{
    return (t1.hours == t2.hours && t1.minutes == t2.minutes);
}

bool operator<(const time& t1, const time& t2)
{
    if ((t1.hours == t2.hours && t1.minutes < t2.minutes) ||
        t1.hours < t2.hours)
    {
        return true;
    }

    return false;
}

int travel_time(route r)
{
    int travel_time_in_minutes = 0;

    if ((r.arrival.hours - r.departure.hours) == 1)
    {
        travel_time_in_minutes = (60 - r.departure.minutes) + r.arrival.minutes;
    }
    if ((r.arrival.hours - r.departure.hours) > 1)
    {
        travel_time_in_minutes =
            ((r.arrival.hours - r.departure.hours) * 60 - 60) +
            (60 - r.departure.minutes) + r.arrival.minutes;
    }
    if (r.arrival.hours == r.departure.hours)
    {
        travel_time_in_minutes = r.arrival.minutes - r.departure.minutes;

        if (travel_time_in_minutes <= 0)
        {
            std::cerr
                << "\nfrom travel_time(): \nwrong input data: arrival time is "
                   "<= than the "
                   "time of departure\n\n";
        }
    }
    return travel_time_in_minutes;
}

void string_to_collection(const std::string& str, std::vector<route>& result)
{
    std::string t1;
    std::string t2;
    route       r;

    std::stringstream stream(str);

    stream >> r.company >> t1 >> t2;

    if (r.company != POSH_COMPANY && r.company != GROTTY_COMPANY)
    {
        std::cerr << "\nfrom string_to_vector():\ninvalid company name: "
                  << r.company << "\nexpected Posh or Grotty" << std::endl;
    }

    r.departure.hours   = stoi(t1.substr(0, 2));
    r.departure.minutes = stoi(t1.substr(3, 2));

    r.arrival.hours   = stoi(t2.substr(0, 2));
    r.arrival.minutes = stoi(t2.substr(3, 2));

    // expected that the arrival and departure on the same day
    if (r.arrival < r.departure)
    {
        std::cerr << "\nfrom string_to_vector():\ninvalid input data: arrival "
                     "time < departure time"
                  << std::endl;
    }
    if (r.arrival == r.departure)
    {
        std::cerr << "\nfrom string_to_vector():\ninvalid input data: arrival "
                     "time == departure time"
                  << std::endl;
    }

    // Any service longer than an hour shall not be included
    if (travel_time(r) <= 60)
    {
        result.push_back(r);
    }
}

bool equal_by_time(route r1, route r2)
{
    if (r1.departure == r2.departure && r1.arrival == r2.arrival)
        return true;

    return false;
}

std::vector<route> delete_duplicate(std::vector<route>& input)
{
    std::vector<route> result;

    //=== sort by departure before delete Grotty!! ===
    sort(input.begin(), input.end(),
         [](route r1, route r2) { return r1.departure < r2.departure; });

    size_t i = 1;
    if (equal_by_time(input[0], input[1]))
    {
        if (input[0].company != GROTTY_COMPANY)
        {
            result.push_back(input[0]);
            i = 2;
        }
    }
    else
    {
        result.push_back(input[0]);
    }
    while (i < input.size())
    {
        if (equal_by_time(input[i - 1], input[i]))
        {
            if (input[i].company != GROTTY_COMPANY)
            {
                result.push_back(input[i]);
            }
            if (input[i - 1].company != GROTTY_COMPANY)
            {
                result.push_back(input[i - 1]);
            }
        }
        else
        {
            result.push_back(input[i]);
        }
        ++i;
    }
    return result;
}

// If it starts at the same time and reaches earlier
std::vector<route> select_by_departure(std::vector<route>& input)
{
    //=== sort by departure before select !! ===
    sort(input.begin(), input.end(),
         [](route r1, route r2) { return r1.departure < r2.departure; });

    std::vector<route> tmp;
    tmp.reserve(input.size());
    size_t             i = 0;
    std::vector<route> result;

    while (i < input.size())
    {
        tmp.push_back(input[i]);
        while (input[i].departure == input[i + 1].departure)
        {
            tmp.push_back(input[i + 1]);
            ++i;
            if (i == input.size() - 1)
                break;
        }

        sort(tmp.begin(), tmp.end(),
             [](route r1, route r2) { return r1.arrival < r2.arrival; });
        result.push_back(tmp[0]);
        tmp.clear();
        ++i;
    }

    return result;
}

// If it starts later and reaches at the same time
std::vector<route> select_by_arrival(std::vector<route>& input)
{
    //=== sort by arrival before select !! ===
    sort(input.begin(), input.end(),
         [](route r1, route r2) { return r1.arrival < r2.arrival; });

    std::vector<route> tmp;
    tmp.reserve(input.size());
    size_t             i = 0;
    std::vector<route> result;

    while (i < input.size())
    {
        tmp.push_back(input[i]);
        while (input[i].arrival == input[i + 1].arrival)
        {
            tmp.push_back(input[i + 1]);
            ++i;
            if (i == input.size() - 1)
                break;
        }
        sort(tmp.begin(), tmp.end(),
             [](route r1, route r2) { return !(r1.departure < r2.departure); });
        result.push_back(tmp[0]);
        tmp.clear();
        ++i;
    }

    return result;
}

void split(std::vector<route>& input, std::vector<route>& posh,
           std::vector<route>& grotty)
{
    //=== sort by departure before split !! ===
    sort(input.begin(), input.end(),
         [](route r1, route r2) { return r1.departure < r2.departure; });

    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i].company == GROTTY_COMPANY)
        {
            grotty.push_back(input[i]);
        }
        else
        {
            posh.push_back(input[i]);
        }
    }
}

// 1:20 => 01:20
std::string time_to_string(time input)
{
    std::string result;
    std::string hours;
    std::string minutes;

    input.hours < 9 ? hours = "0" + std::to_string(input.hours)
                    : hours = std::to_string(input.hours);

    input.minutes < 9 ? minutes = "0" + std::to_string(input.minutes)
                      : minutes = std::to_string(input.minutes);

    return result = hours + ":" + minutes;
}

void print_to_file(const std::vector<route>& input, std::ofstream& file)
{
    for (size_t i = 0; i < input.size() - 1; ++i)
    {
        file << input[i].company + " " + time_to_string(input[i].departure) +
                    " " + time_to_string(input[i].arrival)
             << std::endl;
    }
    // don't need endl at the last string of the file
    file << input[input.size() - 1].company + " " +
                time_to_string(input[input.size() - 1].departure) + " " +
                time_to_string(input[input.size() - 1].arrival);
}

//======================   MAIN   ======================
int main(int /*argc*/, char** /*argv*/)
{
    std::vector<route> routes_from_file;
    std::vector<route> routes_without_duplicates;
    std::vector<route> routes_by_departure;
    std::vector<route> routes_by_arrival;
    std::vector<route> posh_routes;
    std::vector<route> grotty_routes;

    std::string file_name_and_adress = "D:\\test.txt";  // PUT YOUR FILE HERE
    std::string file_copy            = "D:\\test_.txt"; // YOUR RESULT
    std::string string_from_file;

    std::ifstream input_file(file_name_and_adress);
    if (!input_file)
    {
        std::cerr << "\nfrom main():\ncan't open file " << file_name_and_adress
                  << "\n";
        return EXIT_FAILURE;
    }

    std::ofstream result_file(file_copy);
    if (!result_file)
    {
        std::cerr << "\nfrom main():\ncan't open file " << file_copy << "\n";
        return EXIT_FAILURE;
    }

    while (getline(input_file, string_from_file))
    {
        string_to_collection(string_from_file, routes_from_file);
    }

    routes_without_duplicates = delete_duplicate(routes_from_file);
    routes_by_departure       = select_by_departure(routes_without_duplicates);
    routes_by_arrival         = select_by_arrival(routes_by_departure);

    split(routes_by_arrival, posh_routes, grotty_routes);

    print_to_file(posh_routes, result_file);
    result_file << std::endl << std::endl;
    print_to_file(grotty_routes, result_file);

    input_file.close();
    result_file.close();

    return EXIT_SUCCESS;
}
