#include <cstring>
#include <iostream>
#include <vector>
#include <unordered_map>

#include <Python.h>

#include "ReadData.cpp"

#if PY_MAJOR_VERSION >= 3
#define PY3K
#endif

class Runner;

struct Program
{
    int (Runner::*pointer)(Program **);
    int n_args;
    struct Program **args;

    Program() : pointer(NULL),
                n_args(0),
                args(NULL)
    {
    }
};

/*
struct Instance
{
    std::vector<std::vector<int **> > training;
    int **input;
    int **output;
};*/

class Runner
{
public:
    Runner() { reset(); }

    Instance *data;

    // Status
    int status_end(Program **)
    {
        status = -1;
        return 0;
    }

    int set_status(Program **p)
    {

        int value = (this->*p[0]->pointer)(p[0]->args);
        status = value;
        return 0;
    }

    int check_status(Program **p)
    {
        int value = (this->*p[0]->pointer)(p[0]->args);
        return status == value;
    }

    int get_status(Program **) { return status; }

    // Integers
    int get0(Program **) { return 0; }
    int get1(Program **) { return 1; }
    int get2(Program **) { return 2; }
    int get3(Program **) { return 3; }
    int get4(Program **) { return 4; }
    int get5(Program **) { return 5; }
    int get6(Program **) { return 6; }
    int get7(Program **) { return 7; }
    int get8(Program **) { return 8; }
    int get9(Program **) { return 9; }
    int get_1(Program **) { return -1; }

    // Training
    int input_end(Program **)
    {
        return input_position_x == (get_length_input_x(NULL) - 1);
    }

    int input_beginning(Program **)
    {
        return input_position_x == 0;
    }

    int input_down_end(Program **)
    {
        return input_position_y == (get_length_input_y(NULL) - 1);
    }

    int output_end(Program **)
    {
        return output_position_x == (get_length_output_x(NULL) - 1);
    }

    int output_beginning(Program **)
    {
        return output_position_x == 0;
    }

    int output_down_end(Program **)
    {
        return output_position_y == (get_length_output_y(NULL) - 1);
    }

    int output_move_left(Program **)
    {
        if (output_position_x > 0)
            output_position_x--;

        return 0;
    }

    int output_move_right(Program **)
    {
        if (output_position_x < (get_length_output_x(NULL) - 1))
            output_position_x++;

        return 0;
    }

    int output_move_down(Program **)
    {
        if (output_position_y < (get_length_output_y(NULL) - 1))
            output_position_y++;

        return 0;
    }

    int output_move_up(Program **)
    {
        if (output_position_y > 0)
            output_position_y--;

        return 0;
    }

    int get_input_position_x(Program **)
    {
        return input_position_x;
    }

    int get_input_position_y(Program **)
    {
        return input_position_y;
    }

    int get_output_position_x(Program **)
    {
        return output_position_x;
    }

    int get_output_position_y(Program **)
    {
        return output_position_y;
    }

    int get_length_input_x(Program **)
    {
        Array m = data->training.at(input_id).at(0);
        return m.cols;
    }

    int get_length_input_y(Program **)
    {
        Array m = data->training.at(input_id).at(0);
        return m.rows;
    }

    int get_length_output_x(Program **)
    {
        Array m = data->training.at(input_id).at(1);
        return m.cols;
    }

    int get_length_output_y(Program **)
    {
        Array m = data->training.at(input_id).at(1);
        return m.rows;
    }

    // Training Input
    int input_next(Program **)
    {
        if (input_id < (data->training.size() - 1))
        {
            input_id += 1;
            input_position_x = 0;
            input_position_y = 0;
            output_position_x = 0;
            output_position_y = 0;
        }

        return 0;
    }

    int input_previous(Program **)
    {
        if (input_id > 0)
        {
            input_id--;
            input_position_x = 0;
            input_position_y = 0;
            output_position_x = 0;
            output_position_y = 0;
        }

        return 0;
    }

    int input_move_left(Program **)
    {
        if (input_position_x > 0)
            input_position_x--;

        return 0;
    }

    int input_move_right(Program **)
    {
        if (input_position_x < (get_length_input_x(NULL) - 1))
            input_position_x++;

        return 0;
    }

    int input_move_down(Program **)
    {
        if (input_position_y < (get_length_input_y(NULL) - 1))
            input_position_y++;

        return 0;
    }

    int input_move_up(Program **)
    {
        if (input_position_y > 0)
            input_position_y--;

        return 0;
    }

    int reset_input_position(Program **)
    {
        input_position_x = 0;
        return 0;
    }

    int reset_input_down_position(Program **)
    {
        input_position_y = 0;
        return 0;
    }

    int input_max(Program **)
    {
        //std:cout << "input_max\n";
        int *arr = data->training.at(input_id).at(0).array[input_position_y];
        int max = arr[0];
        for (int i = 1; i < get_length_input_x(NULL); i++)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
        }
        return max;
    }

    int input_min(Program **)
    {        //std:cout << "input_min\n";

        int *arr = data->training.at(input_id).at(0).array[input_position_y];
        int min = arr[0];
        for (int i = 1; i < get_length_input_x(NULL); i++)
        {
            if (arr[i] < min)
            {
                min = arr[i];
            }
        }
        return min;
    }

    int input_read(Program **)
    {
        int *test = data->training.at(input_id).at(0).array[input_position_y];
        return test[input_position_x];
    }

    int output_read(Program **)
    {
        return data->training.at(input_id).at(1).array[output_position_y][output_position_x];
    }

    int reset_output_position(Program **)
    {
        output_position_x = 0;
        return 0;
    }

    int reset_output_down_position(Program **)
    {
        output_position_y = 0;
        return 0;
    }

    // Testing
    int get_testing_length_input_x(Program **)
    {
        return data->input.cols;
    }

    int get_testing_length_input_y(Program **)
    {
        return data->input.rows;
    }

    int get_testing_length_output_x(Program **)
    {
        return data->output.cols;
    }

    int get_testing_length_output_y(Program **)
    {
        return data->output.rows;
    }

    int get_testing_input_position_y(Program **)
    {
        return testing_output_position_y;
    }

    int get_testing_input_position_x(Program **)
    {
        return testing_input_position_x;
    }

    int get_testing_output_position_y(Program **)
    {
        return testing_output_position_y;
    }

    int get_testing_output_position_x(Program **)
    {
        return testing_output_position_x;
    }

    int testing_input_max(Program **)
    {
                //std::cout << "testing_input_max\n";

        int *arr = data->input.array[testing_input_position_y];
        int max = arr[0];
        for (int i = 1; i < get_testing_length_input_x(NULL); i++)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
        }
        return max;
    }

    int testing_input_min(Program **)
    {
                //std:cout << "testing_input_min\n";

        int *arr = data->input.array[testing_input_position_y];
        int min = arr[0];
        for (int i = 1; i < get_testing_length_input_x(NULL); i++)
        {
            if (arr[i] < min)
            {
                min = arr[i];
            }
        }
        return min;
    }

    int testing_input_read(Program **)
    {
        return data->input.array[testing_input_position_y][testing_input_position_x];
    }

    int testing_output_read_previous(Program **)
    {
        if (testing_output_position_x > 0)
            return data->output.array[testing_output_position_y][testing_output_position_x - 1];

        return -1;
    }

    int testing_output_read(Program **)
    {
        return data->output.array[testing_output_position_y][testing_output_position_x];
    }

    int testing_reset_input_position(Program **)
    {
        testing_input_position_x = 0;
        return 0;
    }

    int testing_reset_input_down_position(Program **)
    {
        testing_input_position_y = 0;
        return 0;
    }

    int testing_output_write_previous(Program **p)
    {
        try
        {
            if (testing_output_position_x > 0)
            { // std::cout << "previous " << testing_output_position_y << " " << testing_output_position_x << endl;
                // std::cout << data->output.rows << " " << data->output.cols << "\n";
                int value = (this->*p[0]->pointer)(p[0]->args);
                data->output.array[testing_output_position_y][testing_output_position_x - 1] = value;
            }
        }
        catch (std::exception &e)
        {
            std::cerr << "previous " << testing_output_position_y << " " << testing_output_position_x << endl;
            std::cerr << data->output.rows << " " << data->output.cols << "\n";
            std::cerr << "Exception caught : " << e.what() << "testing_output_write_previous" << std::endl;
        }

        return 0;
    }

    int testing_output_write(Program **p)
    {
        try
        {
            // std::cout << "current " << testing_output_position_y << " " << testing_output_position_x << endl;
            // std::cout << data->output.rows << " " << data->output.cols << "\n";
            int value = (this->*p[0]->pointer)(p[0]->args);
            data->output.array[testing_output_position_y][testing_output_position_x] = value;
        }
        catch (std::exception &e)
        {
            std::cerr << "previous " << testing_output_position_y << " " << testing_output_position_x << endl;
            std::cerr << data->output.rows << " " << data->output.cols << "\n";
            std::cerr << "Exception caught : " << e.what() << "testing_output_write_previous" << std::endl;
        }

        return 0;
    }

    int testing_reset_output_position(Program **)
    {
        testing_output_position_x = 0;
        return 0;
    }

    int testing_reset_output_down_position(Program **)
    {
        testing_output_position_y = 0;
        return 0;
    }

    int testing_output_move_left(Program **)
    {
        if (testing_output_position_x > 0)
            testing_output_position_x--;

        return 0;
    }

    int testing_output_move_right(Program **)
    {
        if (testing_output_position_x < (get_testing_length_output_x(NULL) - 1))
            testing_output_position_x++;

        return 0;
    }

    int testing_output_move_down(Program **)
    {
        if (testing_output_position_y < (get_testing_length_output_y(NULL) - 1))
            testing_output_position_y++;
        else
            status = -1;
        return 0;
    }

    int testing_output_move_up(Program **)
    {
        if (testing_output_position_y > 0)
            testing_output_position_y--;

        return 0;
    }

    int testing_is_output_end(Program **)
    {
        return testing_output_position_x == (get_testing_length_output_x(NULL) - 1);
    }

    int testing_is_output_down(Program **)
    {
        return testing_output_position_y == (get_testing_length_output_y(NULL) - 1);
    }

    int testing_input_move_left(Program **)
    {
        if (testing_input_position_x > 0)
            testing_input_position_x--;

        return 0;
    }

    int testing_input_move_right(Program **)
    {
        if (testing_input_position_x < (get_testing_length_input_x(NULL) - 1))
            testing_input_position_x++;

        return 0;
    }

    int testing_input_move_down(Program **)
    {
        if (testing_input_position_y < (get_testing_length_input_y(NULL) - 1))
            testing_input_position_y++;

        return 0;
    }

    int testing_input_move_up(Program **)
    {
        if (testing_input_position_y > 0)
            testing_input_position_y--;

        return 0;
    }

    int comparison(Program **p)
    {
        if ((this->*p[0]->pointer)(p[0]->args) == 1)
        {
            (this->*p[1]->pointer)(p[1]->args);
        }
        else
        {
            (this->*p[2]->pointer)(p[2]->args);
        }

        return 0;
    }

    int bigger_than_output_next(Program **p)
    {
        if (output_position_x < (get_length_output_x(NULL) - 1))
        {
            return data->training.at(input_id).at(1).array[output_position_y][output_position_x] >
                   data->training.at(input_id).at(1).array[output_position_y][output_position_x + 1];
        }

        return 0;
    }

    int bigger_than_testing_output_next(Program **p)
    {
        if (testing_output_position_x < (get_testing_length_output_x(NULL) - 1))
        {
            return data->output.array[testing_output_position_y][testing_output_position_x] >
                   data->output.array[testing_output_position_y][testing_output_position_x + 1];
        }

        return 0;
    }

    int swap_testing_output_next(Program **p)
    {
        if (!testing_is_output_end(NULL))
        {
            int tmp = data->output.array[testing_output_position_y][testing_output_position_x];
            // std::cout << "Before:" << data->output.array[testing_output_position_y][testing_output_position_x] << "|" << data->output.array[testing_output_position_y][testing_output_position_x+1] << "\n";
            data->output.array[testing_output_position_y][testing_output_position_x] = data->output.array[testing_output_position_y][testing_output_position_x + 1];
            data->output.array[testing_output_position_y][testing_output_position_x + 1] = tmp;
            // std::cout << "After:" << data->output.array[testing_output_position_y][testing_output_position_x] << "|" << data->output.array[testing_output_position_y][testing_output_position_x+1] << "\n";
        }

        return 0;
    }

    int bigger_than(Program **p)
    {
        int output1 = (this->*p[0]->pointer)(p[0]->args);
        int output2 = (this->*p[1]->pointer)(p[1]->args);
        return output1 > output2;
    }

    int equal(Program **p)
    {
        int output1 = (this->*p[0]->pointer)(p[0]->args);
        int output2 = (this->*p[1]->pointer)(p[1]->args);
        return output1 == output2;
    }

    int no(Program **p)
    {
        int output1 = (this->*p[0]->pointer)(p[0]->args);
        return !output1;
    }

    int prog2(Program **p)
    {
        // inner_prog++;

        // if (inner_prog < 15)
        {
            (this->*p[0]->pointer)(p[0]->args);
            (this->*p[1]->pointer)(p[1]->args);
        }
        // else
        //{
        //     status = -2;
        // }

        // inner_prog--;

        return 0;
    }

    int prog3(Program **p)
    {
        // inner_prog++;

        // if (inner_prog < 15)
        {
            (this->*p[0]->pointer)(p[0]->args);
            (this->*p[1]->pointer)(p[1]->args);
            (this->*p[2]->pointer)(p[2]->args);
        }
        // else
        //{
        //     status = -2;
        // }

        // inner_prog--;

        return 0;
    }

    int loop(Program **p)
    {
        int v = (this->*p[0]->pointer)(p[0]->args);

        inner_loop++;

        if (inner_loop < 5 && v > 0 && v <= 30)
        {
            for (int i = 0; i < v; i++)
            {
                (this->*p[1]->pointer)(p[1]->args);
            }
        }
        else
        {
            status = -2;
        }

        inner_loop--;

        return 0;
    }

    int dowhile(Program **p)
    {
        int c = 0;

        inner_loop++;

        if (inner_loop < 3)
        {

            while (!(this->*p[0]->pointer)(p[0]->args) && c < 10)
            {
                c++;
                (this->*p[1]->pointer)(p[1]->args);
            }
        }
        else
        {
            status = -2;
        }

        inner_loop--;

        return 0;
    }

    int read_memory(Program **p)
    {
        return memory;
    }

    int write_memory(Program **p)
    {
        int value = (this->*p[0]->pointer)(p[0]->args);

        memory = value;

        return 0;
    }

    void run(Program *p)
    {
        reset();

        while (step < 100 && status >= 0)
        {
            step++;
            (this->*p->pointer)(p->args);
        }

        // std::cout << status << "\n";
    }

private:
    int step;

    int input_id;
    int output_position_x;
    int output_position_y;
    int input_position_x;
    int input_position_y;

    int testing_output_position_x;
    int testing_output_position_y;
    int testing_input_position_x;
    int testing_input_position_y;

    int status;

    int inner_loop;

    int inner_prog;

    int memory;

    void reset()
    {
        step = 0;

        input_id = 0;
        output_position_x = 0;
        output_position_y = 0;
        input_position_x = 0;
        input_position_y = 0;

        testing_output_position_x = 0;
        testing_output_position_y = 0;
        testing_input_position_x = 0;
        testing_input_position_y = 0;

        status = 0;

        inner_loop = 0;

        inner_prog = 0;

        memory = -1;
    }
};

std::unordered_map<std::string, int (Runner::*)(Program **)> getFunctionMap()
{
    std::unordered_map<std::string, int (Runner::*)(Program **)> map;

    map["status_end"] = &Runner::status_end;
    map["set_status"] = &Runner::set_status;
    map["check_status"] = &Runner::check_status;
    map["get_status"] = &Runner::get_status;

    map["get0"] = &Runner::get0;
    map["get1"] = &Runner::get1;
    map["get2"] = &Runner::get2;
    map["get3"] = &Runner::get3;
    map["get4"] = &Runner::get4;
    map["get5"] = &Runner::get5;
    map["get6"] = &Runner::get6;
    map["get7"] = &Runner::get7;
    map["get8"] = &Runner::get8;
    map["get9"] = &Runner::get9;
    map["get_1"] = &Runner::get_1;

    map["input_end"] = &Runner::input_end;
    map["input_beginning"] = &Runner::input_beginning;
    map["input_down_end"] = &Runner::input_down_end;
    map["output_end"] = &Runner::output_end;
    map["output_beginning"] = &Runner::output_beginning;
    map["output_down_end"] = &Runner::output_down_end;
    map["output_move_left"] = &Runner::output_move_left;
    map["output_move_right"] = &Runner::output_move_right;
    map["output_move_down"] = &Runner::output_move_down;
    map["output_move_up"] = &Runner::output_move_up;
    map["get_input_position_x"] = &Runner::get_input_position_x;
    map["get_input_position_y"] = &Runner::get_input_position_y;
    map["get_output_position_x"] = &Runner::get_output_position_x;
    map["get_output_position_y"] = &Runner::get_output_position_y;
    map["get_length_input_x"] = &Runner::get_length_input_x;
    map["get_length_input_y"] = &Runner::get_length_input_y;
    map["get_length_output_x"] = &Runner::get_length_output_x;
    map["get_length_output_y"] = &Runner::get_length_output_y;
    map["input_next"] = &Runner::input_next;
    map["input_previous"] = &Runner::input_previous;
    map["input_move_left"] = &Runner::input_move_left;
    map["input_move_right"] = &Runner::input_move_right;
    map["input_move_down"] = &Runner::input_move_down;
    map["input_move_up"] = &Runner::input_move_up;
    map["input_max"] = &Runner::input_max;
    map["input_min"] = &Runner::input_min;
    map["input_read"] = &Runner::input_read;
    map["output_read"] = &Runner::output_read;
    map["reset_output_position"] = &Runner::reset_output_position;
    map["reset_output_down_position"] = &Runner::reset_output_down_position;
    map["reset_input_position"] = &Runner::reset_input_position;
    map["reset_input_down_position"] = &Runner::reset_input_down_position;

    map["get_testing_length_input_x"] = &Runner::get_testing_length_input_x;
    map["get_testing_length_input_y"] = &Runner::get_testing_length_input_y;
    map["get_testing_length_output_x"] = &Runner::get_testing_length_output_x;
    map["get_testing_length_output_y"] = &Runner::get_testing_length_output_y;
    map["get_testing_input_position_y"] = &Runner::get_testing_input_position_y;
    map["get_testing_input_position_x"] = &Runner::get_testing_input_position_x;
    map["get_testing_output_position_y"] = &Runner::get_testing_output_position_y;
    map["get_testing_output_position_x"] = &Runner::get_testing_output_position_x;
    map["testing_input_max"] = &Runner::testing_input_max;
    map["testing_input_min"] = &Runner::testing_input_min;
    map["testing_input_read"] = &Runner::testing_input_read;
    map["testing_output_read_previous"] = &Runner::testing_output_read_previous;
    map["testing_output_read"] = &Runner::testing_output_read;
    map["testing_output_write_previous"] = &Runner::testing_output_write_previous;
    map["testing_output_write"] = &Runner::testing_output_write;
    map["testing_reset_output_position"] = &Runner::testing_reset_output_position;
    map["testing_reset_output_down_position"] = &Runner::testing_reset_output_down_position;
    map["testing_reset_input_position"] = &Runner::testing_reset_input_position;
    map["testing_reset_input_down_position"] = &Runner::testing_reset_input_down_position;
    map["testing_output_move_left"] = &Runner::testing_output_move_left;
    map["testing_output_move_right"] = &Runner::testing_output_move_right;
    map["testing_output_move_down"] = &Runner::testing_output_move_down;
    map["testing_output_move_up"] = &Runner::testing_output_move_up;
    map["testing_is_output_end"] = &Runner::testing_is_output_end;
    map["testing_is_output_down"] = &Runner::testing_is_output_down;
    map["testing_input_move_left"] = &Runner::testing_input_move_left;
    map["testing_input_move_right"] = &Runner::testing_input_move_right;
    map["testing_input_move_down"] = &Runner::testing_input_move_down;
    map["testing_input_move_up"] = &Runner::testing_input_move_up;

    map["comparison"] = &Runner::comparison;
    map["bigger_than"] = &Runner::bigger_than;
    map["bigger_thanW"] = &Runner::bigger_than;
    map["bigger_thanR"] = &Runner::bigger_than;
    map["bigger_than_output_next"] = &Runner::bigger_than_output_next;
    map["bigger_than_testing_output_next"] = &Runner::bigger_than_testing_output_next;
    map["swap_testing_output_next"] = &Runner::swap_testing_output_next;
    map["equal"] = &Runner::equal;
    map["equalW"] = &Runner::equal;
    map["equalR"] = &Runner::equal;
    map["no"] = &Runner::no;

    map["prog2"] = &Runner::prog2;
    map["prog3"] = &Runner::prog3;
    map["loop"] = &Runner::loop;
    map["dowhile"] = &Runner::dowhile;

    map["read_memory"] = &Runner::read_memory;
    map["write_memory"] = &Runner::write_memory;

    return map;
}

void freeProgram(Program *p)
{
    if (p == NULL)
    {
        return;
    }

    if (p->n_args > 0)
    {
        for (int i = 0; i < p->n_args; i++)
        {
            freeProgram(p->args[i]);
        }

        delete p->args;
    }

    delete p;
}

Program *getProgram(std::string string, std::unordered_map<std::string, int (Runner::*)(Program **)> map, int &position)
{
    struct Program *program = NULL;

    std::vector<struct Program *> subprograms;

    int initial_position = position;

    while (initial_position < string.length() && string[initial_position] == ' ')
        initial_position++;

    // Traverse string into tree
    while (position < string.length())
    {
        if (string[position] == '(')
        {
            // Create new program entry
            program = new struct Program();
            program->pointer = map[string.substr(initial_position, position - initial_position)];

            position++;

            if (string[position] == ')')
            {
                position++;
                return program;
            }
            else
            {
                Program *sub = getProgram(string, map, position);
                subprograms.push_back(sub);
            }
        }
        else if (string[position] == ')')
        {
            // Close and return
            position++;
            program->n_args = subprograms.size();
            program->args = new struct Program *[subprograms.size()];
            std::copy(subprograms.begin(), subprograms.end(), program->args);

            return program;
        }
        else if (string[position] == ',')
        {
            // After this, there is a new program
            position++;
            subprograms.push_back(getProgram(string, map, position));
        }
        else
        {
            position++;
        }
    }

    return program;
}

Program *getProgram(std::string string, std::unordered_map<std::string, int (Runner::*)(Program **)> map)
{
    int position = 0;
    return getProgram(string, map, position);
}

/*
int main()
{
    // Load data

    // Load program map
    std::unordered_map<std::string, int (Runner::*)(Program **)> map = getFunctionMap();

    // Obtain the program
    std::string program_string = "comparison(bigger_than(get7(),get0()),turn_left(),write_position(get8()))";

    // for (int i= 0; i < 100000; i++)
    //     getProgram(program_string, map);

    // Run the program
    // Runner b;

    // for (int i = 0; i < 100000000; i++)
    //     b.run(program);
}
*/
/*
 *
 * Python wrappers
 *
 *
 */
typedef struct RunnerSimulatorWrapper
{
    PyObject_HEAD
        Runner *mInnerClass;
    Program *program;
    std::unordered_map<std::string, int (Runner::*)(Program **)> map;
    Instance *data;

    RunnerSimulatorWrapper() : program(NULL){};
} RunnerSimulatorWrapper;

static int wrapRunnerSimulatorConstructor(RunnerSimulatorWrapper *self, PyObject *args, PyObject *kwargs)
{
    PyObject *obj = PyTuple_GetItem(args, 0);
    PyObject *repr = PyObject_Str(obj); // Alternatively use PyObject_Repr, but it adds single quotes
    PyObject *str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
    const char *bytes = PyBytes_AS_STRING(str);

    self->mInnerClass = new Runner();
    self->map = getFunctionMap();
    // self->program;

    // self->data = read_dir("/Users/E114560/Documents/research/arc-runner/data");
    self->data = read_dir(bytes);
    // self->data[0] = read("example.txt");
    return 0;
}

/*
static double evaluate(Array output, Array gt)
{
    double result = 0.0;

    int count[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int total = 0;

    double sum = 0;

    for (int i = 0; i < gt.rows; i++)
    {
        for (int j = 0; j < gt.cols; j++)
        {
            count[gt.array[i][j]]++;
            total++;
        }
    }

    if (output.rows != gt.rows || output.cols != gt.cols)
    {

        std::cout << "Different" << std::endl;
    }

    for (int i = 0; i < output.rows; i++)
        for (int j = 0; j < output.cols; j++)
        {
            sum += (total / (double)count[gt.array[i][j]]);
        }


    for (int i = 0; i < output.rows; i++)
        for (int j = 0; j < output.cols; j++)
        {
            if (output.array[i][j] == gt.array[i][j])
                //result++;
                result += (total / (double)count[gt.array[i][j]]);
        }

    //return result / (output.rows * output.cols);
    return result/sum;
}
*/

static double evaluate(Array output, Array gt)
{
    double result = 0.0;

    if (output.rows != gt.rows || output.cols != gt.cols)
    {
        std::cout << "Different" << std::endl;
    }

    for (int i = 0; i < output.rows; i++)
        for (int j = 0; j < output.cols; j++)
        {
            // std::cout << output.array[i][j] << "|" << gt.array[i][j] << "\n";
            if (output.array[i][j] == gt.array[i][j])
                result++;
        }

    return result / (output.rows * output.cols);
}

static PyObject *wrapRun(RunnerSimulatorWrapper *self, PyObject *args)
{
    int item;
    if (!PyArg_ParseTuple(args, "i", &item))
        return NULL;

    for (int i = 0; i < self->data[item].initial.rows; i++)
    {
        for (int j = 0; j < self->data[item].initial.cols; j++)
        {
            self->data[item].output.array[i][j] = self->data[item].initial.array[i][j];
        }
    }

    self->mInnerClass->data = &self->data[item];

    try
    {
        if (self->program != NULL)
            self->mInnerClass->run(self->program);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    PyObject *py_retval;

    // std::cout << "Evaluating" << std::endl;
    try
    {
        py_retval = Py_BuildValue((char *)"d", evaluate(self->mInnerClass->data->output, self->mInnerClass->data->gt));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // std::cout << "Done" << std::endl;

    return py_retval;
}

static PyObject *wrapCompile(RunnerSimulatorWrapper *self, PyObject *args)
{

    PyObject *obj = PyTuple_GetItem(args, 0);
    PyObject *repr = PyObject_Str(obj); // Alternatively use PyObject_Repr, but it adds single quotes
    PyObject *str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
    const char *bytes = PyBytes_AS_STRING(str);

    freeProgram(self->program);
    self->program = getProgram(bytes, self->map);

    // Py_DECREF(str);
    Py_XDECREF(repr);
    Py_XDECREF(str);

    Py_INCREF(Py_None);
    return Py_None;
}

// Getters and setters (here only for the 'eaten' attribute)
static PyGetSetDef RunnerSimulatorWrapper_getsets[] = {
    /*{
        (char *)"max_moves",      // attribute name
        (getter)wrapGetNMaxMoves, // C function to get the attribute
        NULL,                     // C function to set the attribute
        NULL,                     // optional doc string
        NULL                      // optional additional data for getter and setter
    },*/
    {NULL, NULL, NULL, NULL, NULL}};

// Class method declarations
static PyMethodDef RunnerSimulatorWrapper_methods[] = {
    {(char *)"run", (PyCFunction)wrapRun, METH_VARARGS, NULL},
    {(char *)"compile", (PyCFunction)wrapCompile, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static PyModuleDef RunnerSimulatorWrapper_module = {
    PyModuleDef_HEAD_INIT,
    "RunnerSimulatorWrapper",              // Module name to use with Python import statements
    "Provides some functions, but faster", // Module description
    0,
    RunnerSimulatorWrapper_methods // Structure that defines the methods of the module
};

static void RunnerSimulatorWrapperDealloc(RunnerSimulatorWrapper *self)
{
    delete self->mInnerClass;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyTypeObject PyRunnerSimulatorWrapper_Type = {
    PyVarObject_HEAD_INIT(NULL, 0) "Simulator.Runner" /* tp_name */
};

PyMODINIT_FUNC PyInit_Simulator()
{
    PyRunnerSimulatorWrapper_Type.tp_new = PyType_GenericNew;
    PyRunnerSimulatorWrapper_Type.tp_basicsize = sizeof(RunnerSimulatorWrapper);
    PyRunnerSimulatorWrapper_Type.tp_dealloc = (destructor)RunnerSimulatorWrapperDealloc;
    PyRunnerSimulatorWrapper_Type.tp_flags = Py_TPFLAGS_DEFAULT;
    PyRunnerSimulatorWrapper_Type.tp_doc = "Runner simulator";
    PyRunnerSimulatorWrapper_Type.tp_methods = RunnerSimulatorWrapper_methods;
    PyRunnerSimulatorWrapper_Type.tp_getset = RunnerSimulatorWrapper_getsets;
    // PyRunnerSimulatorWrapper_Type.tp_richcompare=RunnerSimulatorWrapperRichcompare;
    //~ PyVoiceType.tp_members=Noddy_members;
    PyRunnerSimulatorWrapper_Type.tp_init = (initproc)wrapRunnerSimulatorConstructor;

    PyObject *m = PyModule_Create(&RunnerSimulatorWrapper_module);
    if (m == NULL)
    {
        return NULL;
    }
    if (PyType_Ready(&PyRunnerSimulatorWrapper_Type) < 0)
    {
        return NULL;
    }
    PyModule_AddObject(m, (char *)"Runner", (PyObject *)&PyRunnerSimulatorWrapper_Type);
    return m;
}