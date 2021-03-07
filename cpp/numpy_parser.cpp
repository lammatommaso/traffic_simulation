#include"numpy_parser.h"

namespace Numpy_Parser{
std::string numpy()
{
    return "import numpy\n";
}

template<class T> std::string print_array(std::string varname, std::vector<T> array)
{
    std::string local = varname + " = numpy.array([";
    for(int i=0; i<array.size()-1; i++)
    {
        local += std::to_string(array[i]) + ", ";
    }
    local += std::to_string(array[array.size()-1]) + "])"+"\n";
    return local;
}
template<class T> std::string print_matrix(std::string varname, std::vector<T> matrix, short n_coloumns)
{
    std::string local = varname + " = numpy.matrix([[";
    for(int i=0; i<matrix.size()-1; i++)
    {
        if((i+1)%n_coloumns==0)
        {
            local += std::to_string(matrix[i]) + "], [";
        }
        else
        {
            local += std::to_string(matrix[i]) + ", ";
        }
    }
    local += std::to_string(matrix[matrix.size()-1]) + "]])"+"\n";
    return local;
}
template<class T> std::string print_var(std::string varname, T var)
{
    return varname + " = " + std::to_string(var) + "\n";
}
}